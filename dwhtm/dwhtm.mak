# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=dwhtm - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to dwhtm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dwhtm - Win32 Release" && "$(CFG)" != "dwhtm - Win32 Debug" &&\
 "$(CFG)" != "dwhtm - Win32 DbgRel"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dwhtm.mak" CFG="dwhtm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dwhtm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwhtm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwhtm - Win32 DbgRel" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "dwhtm - Win32 DbgRel"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "dwhtm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../dll/WinRel"
# PROP Intermediate_Dir "../dll/WinRel"
OUTDIR=.\../dll/WinRel
INTDIR=.\../dll/WinRel
# Begin Custom Macros
TargetName=dwhtm
# End Custom Macros

ALL : "$(OUTDIR)\dwhtm.dll" "$(OUTDIR)\dwhtm.bsc" "c:\winnt\system32\dwhtm.dll"

CLEAN : 
	-@erase "$(INTDIR)\dcappl.obj"
	-@erase "$(INTDIR)\dcappl.sbr"
	-@erase "$(INTDIR)\dcbase.obj"
	-@erase "$(INTDIR)\dcbase.sbr"
	-@erase "$(INTDIR)\dcctlg.obj"
	-@erase "$(INTDIR)\dcctlg.sbr"
	-@erase "$(INTDIR)\dcfile.obj"
	-@erase "$(INTDIR)\dcfile.sbr"
	-@erase "$(INTDIR)\dcfili.obj"
	-@erase "$(INTDIR)\dcfili.sbr"
	-@erase "$(INTDIR)\dcfilp.obj"
	-@erase "$(INTDIR)\dcfilp.sbr"
	-@erase "$(INTDIR)\dcfilr.obj"
	-@erase "$(INTDIR)\dcfilr.sbr"
	-@erase "$(INTDIR)\dcfilw.obj"
	-@erase "$(INTDIR)\dcfilw.sbr"
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dcini.obj"
	-@erase "$(INTDIR)\dcini.sbr"
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmacr.obj"
	-@erase "$(INTDIR)\dcmacr.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\dwhtm.obj"
	-@erase "$(INTDIR)\dwhtm.res"
	-@erase "$(INTDIR)\dwhtm.sbr"
	-@erase "$(INTDIR)\dwhtman.obj"
	-@erase "$(INTDIR)\dwhtman.sbr"
	-@erase "$(INTDIR)\dwhtmd0.obj"
	-@erase "$(INTDIR)\dwhtmd0.sbr"
	-@erase "$(INTDIR)\dwhtmd1.obj"
	-@erase "$(INTDIR)\dwhtmd1.sbr"
	-@erase "$(INTDIR)\dwhtmdb.obj"
	-@erase "$(INTDIR)\dwhtmdb.sbr"
	-@erase "$(INTDIR)\dwhtmdd.obj"
	-@erase "$(INTDIR)\dwhtmdd.sbr"
	-@erase "$(INTDIR)\dwhtmdm.obj"
	-@erase "$(INTDIR)\dwhtmdm.sbr"
	-@erase "$(INTDIR)\dwhtmds.obj"
	-@erase "$(INTDIR)\dwhtmds.sbr"
	-@erase "$(INTDIR)\dwhtmdt.obj"
	-@erase "$(INTDIR)\dwhtmdt.sbr"
	-@erase "$(INTDIR)\dwhtmfl.obj"
	-@erase "$(INTDIR)\dwhtmfl.sbr"
	-@erase "$(INTDIR)\dwhtmgr.obj"
	-@erase "$(INTDIR)\dwhtmgr.sbr"
	-@erase "$(INTDIR)\dwhtmhr.obj"
	-@erase "$(INTDIR)\dwhtmhr.sbr"
	-@erase "$(INTDIR)\dwhtmin.obj"
	-@erase "$(INTDIR)\dwhtmin.sbr"
	-@erase "$(INTDIR)\dwhtmpp.obj"
	-@erase "$(INTDIR)\dwhtmpp.sbr"
	-@erase "$(INTDIR)\dwhtmrf.obj"
	-@erase "$(INTDIR)\dwhtmrf.sbr"
	-@erase "$(INTDIR)\dwhtmsp.obj"
	-@erase "$(INTDIR)\dwhtmsp.sbr"
	-@erase "$(INTDIR)\dwhtmtb.obj"
	-@erase "$(INTDIR)\dwhtmtb.sbr"
	-@erase "$(INTDIR)\dwhtmtx.obj"
	-@erase "$(INTDIR)\dwhtmtx.sbr"
	-@erase "$(INTDIR)\dwhtmxf.obj"
	-@erase "$(INTDIR)\dwhtmxf.sbr"
	-@erase "$(OUTDIR)\dwhtm.bsc"
	-@erase "$(OUTDIR)\dwhtm.dll"
	-@erase "$(OUTDIR)\dwhtm.exp"
	-@erase "$(OUTDIR)\dwhtm.lib"
	-@erase "c:\winnt\system32\dwhtm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwhtm.pch" /YX /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\../dll/WinRel/
CPP_SBRS=.\../dll/WinRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwhtm.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwhtm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilp.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmacr.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\dwhtm.sbr" \
	"$(INTDIR)\dwhtman.sbr" \
	"$(INTDIR)\dwhtmd0.sbr" \
	"$(INTDIR)\dwhtmd1.sbr" \
	"$(INTDIR)\dwhtmdb.sbr" \
	"$(INTDIR)\dwhtmdd.sbr" \
	"$(INTDIR)\dwhtmdm.sbr" \
	"$(INTDIR)\dwhtmds.sbr" \
	"$(INTDIR)\dwhtmdt.sbr" \
	"$(INTDIR)\dwhtmfl.sbr" \
	"$(INTDIR)\dwhtmgr.sbr" \
	"$(INTDIR)\dwhtmhr.sbr" \
	"$(INTDIR)\dwhtmin.sbr" \
	"$(INTDIR)\dwhtmpp.sbr" \
	"$(INTDIR)\dwhtmrf.sbr" \
	"$(INTDIR)\dwhtmsp.sbr" \
	"$(INTDIR)\dwhtmtb.sbr" \
	"$(INTDIR)\dwhtmtx.sbr" \
	"$(INTDIR)\dwhtmxf.sbr"

"$(OUTDIR)\dwhtm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/dwhtm.pdb" /machine:I386 /out:"$(OUTDIR)/dwhtm.dll"\
 /implib:"$(OUTDIR)/dwhtm.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilp.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmacr.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\dwhtm.obj" \
	"$(INTDIR)\dwhtm.res" \
	"$(INTDIR)\dwhtman.obj" \
	"$(INTDIR)\dwhtmd0.obj" \
	"$(INTDIR)\dwhtmd1.obj" \
	"$(INTDIR)\dwhtmdb.obj" \
	"$(INTDIR)\dwhtmdd.obj" \
	"$(INTDIR)\dwhtmdm.obj" \
	"$(INTDIR)\dwhtmds.obj" \
	"$(INTDIR)\dwhtmdt.obj" \
	"$(INTDIR)\dwhtmfl.obj" \
	"$(INTDIR)\dwhtmgr.obj" \
	"$(INTDIR)\dwhtmhr.obj" \
	"$(INTDIR)\dwhtmin.obj" \
	"$(INTDIR)\dwhtmpp.obj" \
	"$(INTDIR)\dwhtmrf.obj" \
	"$(INTDIR)\dwhtmsp.obj" \
	"$(INTDIR)\dwhtmtb.obj" \
	"$(INTDIR)\dwhtmtx.obj" \
	"$(INTDIR)\dwhtmxf.obj"

"$(OUTDIR)\dwhtm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinRel\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinRel\dwhtm.dll
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../dll/WinDebug"
# PROP Intermediate_Dir "../dll/WinDebug"
OUTDIR=.\../dll/WinDebug
INTDIR=.\../dll/WinDebug
# Begin Custom Macros
TargetName=dwhtm
# End Custom Macros

ALL : "$(OUTDIR)\dwhtm.dll" "$(OUTDIR)\dwhtm.bsc" "c:\winnt\system32\dwhtm.dll"

CLEAN : 
	-@erase "$(INTDIR)\dcappl.obj"
	-@erase "$(INTDIR)\dcappl.sbr"
	-@erase "$(INTDIR)\dcbase.obj"
	-@erase "$(INTDIR)\dcbase.sbr"
	-@erase "$(INTDIR)\dcctlg.obj"
	-@erase "$(INTDIR)\dcctlg.sbr"
	-@erase "$(INTDIR)\dcfile.obj"
	-@erase "$(INTDIR)\dcfile.sbr"
	-@erase "$(INTDIR)\dcfili.obj"
	-@erase "$(INTDIR)\dcfili.sbr"
	-@erase "$(INTDIR)\dcfilp.obj"
	-@erase "$(INTDIR)\dcfilp.sbr"
	-@erase "$(INTDIR)\dcfilr.obj"
	-@erase "$(INTDIR)\dcfilr.sbr"
	-@erase "$(INTDIR)\dcfilw.obj"
	-@erase "$(INTDIR)\dcfilw.sbr"
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dcini.obj"
	-@erase "$(INTDIR)\dcini.sbr"
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmacr.obj"
	-@erase "$(INTDIR)\dcmacr.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\dwhtm.obj"
	-@erase "$(INTDIR)\dwhtm.res"
	-@erase "$(INTDIR)\dwhtm.sbr"
	-@erase "$(INTDIR)\dwhtman.obj"
	-@erase "$(INTDIR)\dwhtman.sbr"
	-@erase "$(INTDIR)\dwhtmd0.obj"
	-@erase "$(INTDIR)\dwhtmd0.sbr"
	-@erase "$(INTDIR)\dwhtmd1.obj"
	-@erase "$(INTDIR)\dwhtmd1.sbr"
	-@erase "$(INTDIR)\dwhtmdb.obj"
	-@erase "$(INTDIR)\dwhtmdb.sbr"
	-@erase "$(INTDIR)\dwhtmdd.obj"
	-@erase "$(INTDIR)\dwhtmdd.sbr"
	-@erase "$(INTDIR)\dwhtmdm.obj"
	-@erase "$(INTDIR)\dwhtmdm.sbr"
	-@erase "$(INTDIR)\dwhtmds.obj"
	-@erase "$(INTDIR)\dwhtmds.sbr"
	-@erase "$(INTDIR)\dwhtmdt.obj"
	-@erase "$(INTDIR)\dwhtmdt.sbr"
	-@erase "$(INTDIR)\dwhtmfl.obj"
	-@erase "$(INTDIR)\dwhtmfl.sbr"
	-@erase "$(INTDIR)\dwhtmgr.obj"
	-@erase "$(INTDIR)\dwhtmgr.sbr"
	-@erase "$(INTDIR)\dwhtmhr.obj"
	-@erase "$(INTDIR)\dwhtmhr.sbr"
	-@erase "$(INTDIR)\dwhtmin.obj"
	-@erase "$(INTDIR)\dwhtmin.sbr"
	-@erase "$(INTDIR)\dwhtmpp.obj"
	-@erase "$(INTDIR)\dwhtmpp.sbr"
	-@erase "$(INTDIR)\dwhtmrf.obj"
	-@erase "$(INTDIR)\dwhtmrf.sbr"
	-@erase "$(INTDIR)\dwhtmsp.obj"
	-@erase "$(INTDIR)\dwhtmsp.sbr"
	-@erase "$(INTDIR)\dwhtmtb.obj"
	-@erase "$(INTDIR)\dwhtmtb.sbr"
	-@erase "$(INTDIR)\dwhtmtx.obj"
	-@erase "$(INTDIR)\dwhtmtx.sbr"
	-@erase "$(INTDIR)\dwhtmxf.obj"
	-@erase "$(INTDIR)\dwhtmxf.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dwhtm.bsc"
	-@erase "$(OUTDIR)\dwhtm.dll"
	-@erase "$(OUTDIR)\dwhtm.exp"
	-@erase "$(OUTDIR)\dwhtm.lib"
	-@erase "$(OUTDIR)\dwhtm.map"
	-@erase "c:\winnt\system32\dwhtm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDebug/
CPP_SBRS=.\../dll/WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwhtm.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwhtm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilp.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmacr.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\dwhtm.sbr" \
	"$(INTDIR)\dwhtman.sbr" \
	"$(INTDIR)\dwhtmd0.sbr" \
	"$(INTDIR)\dwhtmd1.sbr" \
	"$(INTDIR)\dwhtmdb.sbr" \
	"$(INTDIR)\dwhtmdd.sbr" \
	"$(INTDIR)\dwhtmdm.sbr" \
	"$(INTDIR)\dwhtmds.sbr" \
	"$(INTDIR)\dwhtmdt.sbr" \
	"$(INTDIR)\dwhtmfl.sbr" \
	"$(INTDIR)\dwhtmgr.sbr" \
	"$(INTDIR)\dwhtmhr.sbr" \
	"$(INTDIR)\dwhtmin.sbr" \
	"$(INTDIR)\dwhtmpp.sbr" \
	"$(INTDIR)\dwhtmrf.sbr" \
	"$(INTDIR)\dwhtmsp.sbr" \
	"$(INTDIR)\dwhtmtb.sbr" \
	"$(INTDIR)\dwhtmtx.sbr" \
	"$(INTDIR)\dwhtmxf.sbr"

"$(OUTDIR)\dwhtm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /profile /map /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /profile\
 /map:"$(INTDIR)/dwhtm.map" /debug /machine:I386 /out:"$(OUTDIR)/dwhtm.dll"\
 /implib:"$(OUTDIR)/dwhtm.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilp.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmacr.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\dwhtm.obj" \
	"$(INTDIR)\dwhtm.res" \
	"$(INTDIR)\dwhtman.obj" \
	"$(INTDIR)\dwhtmd0.obj" \
	"$(INTDIR)\dwhtmd1.obj" \
	"$(INTDIR)\dwhtmdb.obj" \
	"$(INTDIR)\dwhtmdd.obj" \
	"$(INTDIR)\dwhtmdm.obj" \
	"$(INTDIR)\dwhtmds.obj" \
	"$(INTDIR)\dwhtmdt.obj" \
	"$(INTDIR)\dwhtmfl.obj" \
	"$(INTDIR)\dwhtmgr.obj" \
	"$(INTDIR)\dwhtmhr.obj" \
	"$(INTDIR)\dwhtmin.obj" \
	"$(INTDIR)\dwhtmpp.obj" \
	"$(INTDIR)\dwhtmrf.obj" \
	"$(INTDIR)\dwhtmsp.obj" \
	"$(INTDIR)\dwhtmtb.obj" \
	"$(INTDIR)\dwhtmtx.obj" \
	"$(INTDIR)\dwhtmxf.obj"

"$(OUTDIR)\dwhtm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinDebug\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinDebug\dwhtm.dll
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dwhtm___"
# PROP BASE Intermediate_Dir "dwhtm___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../dll/WinDbRel"
# PROP Intermediate_Dir "../dll/WinDbRel"
# PROP Target_Dir ""
OUTDIR=.\../dll/WinDbRel
INTDIR=.\../dll/WinDbRel
# Begin Custom Macros
TargetName=dwhtm
# End Custom Macros

ALL : "$(OUTDIR)\dwhtm.dll" "$(OUTDIR)\dwhtm.bsc"\
 "..\..\winnt\system32\dwhtm.dll"

CLEAN : 
	-@erase "$(INTDIR)\dcappl.obj"
	-@erase "$(INTDIR)\dcappl.sbr"
	-@erase "$(INTDIR)\dcbase.obj"
	-@erase "$(INTDIR)\dcbase.sbr"
	-@erase "$(INTDIR)\dcctlg.obj"
	-@erase "$(INTDIR)\dcctlg.sbr"
	-@erase "$(INTDIR)\dcfile.obj"
	-@erase "$(INTDIR)\dcfile.sbr"
	-@erase "$(INTDIR)\dcfili.obj"
	-@erase "$(INTDIR)\dcfili.sbr"
	-@erase "$(INTDIR)\dcfilp.obj"
	-@erase "$(INTDIR)\dcfilp.sbr"
	-@erase "$(INTDIR)\dcfilr.obj"
	-@erase "$(INTDIR)\dcfilr.sbr"
	-@erase "$(INTDIR)\dcfilw.obj"
	-@erase "$(INTDIR)\dcfilw.sbr"
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dcini.obj"
	-@erase "$(INTDIR)\dcini.sbr"
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmacr.obj"
	-@erase "$(INTDIR)\dcmacr.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\dwhtm.obj"
	-@erase "$(INTDIR)\dwhtm.res"
	-@erase "$(INTDIR)\dwhtm.sbr"
	-@erase "$(INTDIR)\dwhtman.obj"
	-@erase "$(INTDIR)\dwhtman.sbr"
	-@erase "$(INTDIR)\dwhtmd0.obj"
	-@erase "$(INTDIR)\dwhtmd0.sbr"
	-@erase "$(INTDIR)\dwhtmd1.obj"
	-@erase "$(INTDIR)\dwhtmd1.sbr"
	-@erase "$(INTDIR)\dwhtmdb.obj"
	-@erase "$(INTDIR)\dwhtmdb.sbr"
	-@erase "$(INTDIR)\dwhtmdd.obj"
	-@erase "$(INTDIR)\dwhtmdd.sbr"
	-@erase "$(INTDIR)\dwhtmdm.obj"
	-@erase "$(INTDIR)\dwhtmdm.sbr"
	-@erase "$(INTDIR)\dwhtmds.obj"
	-@erase "$(INTDIR)\dwhtmds.sbr"
	-@erase "$(INTDIR)\dwhtmdt.obj"
	-@erase "$(INTDIR)\dwhtmdt.sbr"
	-@erase "$(INTDIR)\dwhtmfl.obj"
	-@erase "$(INTDIR)\dwhtmfl.sbr"
	-@erase "$(INTDIR)\dwhtmgr.obj"
	-@erase "$(INTDIR)\dwhtmgr.sbr"
	-@erase "$(INTDIR)\dwhtmhr.obj"
	-@erase "$(INTDIR)\dwhtmhr.sbr"
	-@erase "$(INTDIR)\dwhtmin.obj"
	-@erase "$(INTDIR)\dwhtmin.sbr"
	-@erase "$(INTDIR)\dwhtmpp.obj"
	-@erase "$(INTDIR)\dwhtmpp.sbr"
	-@erase "$(INTDIR)\dwhtmrf.obj"
	-@erase "$(INTDIR)\dwhtmrf.sbr"
	-@erase "$(INTDIR)\dwhtmsp.obj"
	-@erase "$(INTDIR)\dwhtmsp.sbr"
	-@erase "$(INTDIR)\dwhtmtb.obj"
	-@erase "$(INTDIR)\dwhtmtb.sbr"
	-@erase "$(INTDIR)\dwhtmtx.obj"
	-@erase "$(INTDIR)\dwhtmtx.sbr"
	-@erase "$(INTDIR)\dwhtmxf.obj"
	-@erase "$(INTDIR)\dwhtmxf.sbr"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dwhtm.bsc"
	-@erase "$(OUTDIR)\dwhtm.dll"
	-@erase "$(OUTDIR)\dwhtm.exp"
	-@erase "$(OUTDIR)\dwhtm.lib"
	-@erase "$(OUTDIR)\dwhtm.pdb"
	-@erase "..\..\winnt\system32\dwhtm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /Zi /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /Zi /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwhtm.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDbRel/
CPP_SBRS=.\../dll/WinDbRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwhtm.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwhtm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilp.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmacr.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\dwhtm.sbr" \
	"$(INTDIR)\dwhtman.sbr" \
	"$(INTDIR)\dwhtmd0.sbr" \
	"$(INTDIR)\dwhtmd1.sbr" \
	"$(INTDIR)\dwhtmdb.sbr" \
	"$(INTDIR)\dwhtmdd.sbr" \
	"$(INTDIR)\dwhtmdm.sbr" \
	"$(INTDIR)\dwhtmds.sbr" \
	"$(INTDIR)\dwhtmdt.sbr" \
	"$(INTDIR)\dwhtmfl.sbr" \
	"$(INTDIR)\dwhtmgr.sbr" \
	"$(INTDIR)\dwhtmhr.sbr" \
	"$(INTDIR)\dwhtmin.sbr" \
	"$(INTDIR)\dwhtmpp.sbr" \
	"$(INTDIR)\dwhtmrf.sbr" \
	"$(INTDIR)\dwhtmsp.sbr" \
	"$(INTDIR)\dwhtmtb.sbr" \
	"$(INTDIR)\dwhtmtx.sbr" \
	"$(INTDIR)\dwhtmxf.sbr"

"$(OUTDIR)\dwhtm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/dwhtm.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dwhtm.dll"\
 /implib:"$(OUTDIR)/dwhtm.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilp.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmacr.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\dwhtm.obj" \
	"$(INTDIR)\dwhtm.res" \
	"$(INTDIR)\dwhtman.obj" \
	"$(INTDIR)\dwhtmd0.obj" \
	"$(INTDIR)\dwhtmd1.obj" \
	"$(INTDIR)\dwhtmdb.obj" \
	"$(INTDIR)\dwhtmdd.obj" \
	"$(INTDIR)\dwhtmdm.obj" \
	"$(INTDIR)\dwhtmds.obj" \
	"$(INTDIR)\dwhtmdt.obj" \
	"$(INTDIR)\dwhtmfl.obj" \
	"$(INTDIR)\dwhtmgr.obj" \
	"$(INTDIR)\dwhtmhr.obj" \
	"$(INTDIR)\dwhtmin.obj" \
	"$(INTDIR)\dwhtmpp.obj" \
	"$(INTDIR)\dwhtmrf.obj" \
	"$(INTDIR)\dwhtmsp.obj" \
	"$(INTDIR)\dwhtmtb.obj" \
	"$(INTDIR)\dwhtmtx.obj" \
	"$(INTDIR)\dwhtmxf.obj"

"$(OUTDIR)\dwhtm.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinDbRel\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinDbRel\dwhtm.dll
SOURCE=$(InputPath)

"d:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) d:\winnt\system32

# End Custom Build

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "dwhtm - Win32 Release"
# Name "dwhtm - Win32 Debug"
# Name "dwhtm - Win32 DbgRel"

!IF  "$(CFG)" == "dwhtm - Win32 Release"

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dwhtm.rc
DEP_RSC_DWHTM=\
	".\dropclose.gif"\
	".\dropopen.gif"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtm.res" : $(SOURCE) $(DEP_RSC_DWHTM) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtm.res" : $(SOURCE) $(DEP_RSC_DWHTM) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtm.res" : $(SOURCE) $(DEP_RSC_DWHTM) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmtx.cpp
DEP_CPP_DWHTMT=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtmtx.obj" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"

"$(INTDIR)\dwhtmtx.sbr" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtmtx.obj" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"

"$(INTDIR)\dwhtmtx.sbr" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtmtx.obj" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"

"$(INTDIR)\dwhtmtx.sbr" : $(SOURCE) $(DEP_CPP_DWHTMT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmtb.cpp
DEP_CPP_DWHTMTB=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtmtb.obj" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"

"$(INTDIR)\dwhtmtb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtmtb.obj" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"

"$(INTDIR)\dwhtmtb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtmtb.obj" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"

"$(INTDIR)\dwhtmtb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMTB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmrf.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMR=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmrf.obj" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"

"$(INTDIR)\dwhtmrf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMR=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmrf.obj" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"

"$(INTDIR)\dwhtmrf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMR=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmrf.obj" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"

"$(INTDIR)\dwhtmrf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmgr.cpp
DEP_CPP_DWHTMG=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtmgr.obj" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"

"$(INTDIR)\dwhtmgr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtmgr.obj" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"

"$(INTDIR)\dwhtmgr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtmgr.obj" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"

"$(INTDIR)\dwhtmgr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmfl.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmfl.obj" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"

"$(INTDIR)\dwhtmfl.sbr" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMF=\
	".\putSpec"\
	

"$(INTDIR)\dwhtmfl.obj" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"

"$(INTDIR)\dwhtmfl.sbr" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmfl.obj" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"

"$(INTDIR)\dwhtmfl.sbr" : $(SOURCE) $(DEP_CPP_DWHTMF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtman.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtman.obj" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"

"$(INTDIR)\dwhtman.sbr" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMA=\
	".\HyperSpaceChar"\
	".\PutDITAIndexEntry"\
	".\SetKeyword"\
	

"$(INTDIR)\dwhtman.obj" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"

"$(INTDIR)\dwhtman.sbr" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtman.obj" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"

"$(INTDIR)\dwhtman.sbr" : $(SOURCE) $(DEP_CPP_DWHTMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtm.cpp
DEP_CPP_DWHTM_=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtm.obj" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"

"$(INTDIR)\dwhtm.sbr" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtm.obj" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"

"$(INTDIR)\dwhtm.sbr" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtm.obj" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"

"$(INTDIR)\dwhtm.sbr" : $(SOURCE) $(DEP_CPP_DWHTM_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnodt.cpp
DEP_CPP_DCNOD=\
	".\..\base\dcbase.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnode.cpp
DEP_CPP_DCNODE=\
	".\..\base\dcbase.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmem.cpp
DEP_CPP_DCMEM=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dclist.cpp
DEP_CPP_DCLIS=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcglob.cpp
DEP_CPP_DCGLO=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilw.cpp
DEP_CPP_DCFIL=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilr.cpp
DEP_CPP_DCFILR=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfili.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DCFILI=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DCFILI=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DCFILI=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfile.cpp
DEP_CPP_DCFILE=\
	".\..\base\dcfile.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcctlg.cpp
DEP_CPP_DCCTL=\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcbase.cpp
DEP_CPP_DCBAS=\
	".\..\base\dcbase.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcappl.cpp
DEP_CPP_DCAPP=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmpp.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmpp.obj" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"

"$(INTDIR)\dwhtmpp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmpp.obj" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"

"$(INTDIR)\dwhtmpp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmpp.obj" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"

"$(INTDIR)\dwhtmpp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmxf.cpp
DEP_CPP_DWHTMX=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtmxf.obj" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"

"$(INTDIR)\dwhtmxf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtmxf.obj" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"

"$(INTDIR)\dwhtmxf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtmxf.obj" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"

"$(INTDIR)\dwhtmxf.sbr" : $(SOURCE) $(DEP_CPP_DWHTMX) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmsp.cpp
DEP_CPP_DWHTMS=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


"$(INTDIR)\dwhtmsp.obj" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"

"$(INTDIR)\dwhtmsp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


"$(INTDIR)\dwhtmsp.obj" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"

"$(INTDIR)\dwhtmsp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


"$(INTDIR)\dwhtmsp.obj" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"

"$(INTDIR)\dwhtmsp.sbr" : $(SOURCE) $(DEP_CPP_DWHTMS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilp.cpp
DEP_CPP_DCFILP=\
	"..\base\dcfilp.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwhtm - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmacr.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	
NODEP_CPP_DCMAC=\
	"..\base\GetPredefinedVar"\
	"..\base\GetToken"\
	"..\base\InitMacroModule"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcini.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	
NODEP_CPP_DCINI=\
	"..\base\ProcOneIniVarSect"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmin.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmin.obj" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"

"$(INTDIR)\dwhtmin.sbr" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMI=\
	".\nobreak"\
	

"$(INTDIR)\dwhtmin.obj" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"

"$(INTDIR)\dwhtmin.sbr" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmin.obj" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"

"$(INTDIR)\dwhtmin.sbr" : $(SOURCE) $(DEP_CPP_DWHTMI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmdt.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMD=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdt.obj" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"

"$(INTDIR)\dwhtmdt.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMD=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMD=\
	".\putStr"\
	

"$(INTDIR)\dwhtmdt.obj" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"

"$(INTDIR)\dwhtmdt.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMD=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdt.obj" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"

"$(INTDIR)\dwhtmdt.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmhr.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMH=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmhr.obj" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"

"$(INTDIR)\dwhtmhr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMH=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMH=\
	".\CurrInline"\
	

"$(INTDIR)\dwhtmhr.obj" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"

"$(INTDIR)\dwhtmhr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMH=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmhr.obj" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"

"$(INTDIR)\dwhtmhr.sbr" : $(SOURCE) $(DEP_CPP_DWHTMH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmds.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMDS=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmds.obj" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"

"$(INTDIR)\dwhtmds.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMDS=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWHTMDS=\
	".\DITALoadSpecial"\
	".\DITATopicFileNamePrefix"\
	".\DITATypeNamePrefixList"\
	".\DTopicTypes"\
	

"$(INTDIR)\dwhtmds.obj" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"

"$(INTDIR)\dwhtmds.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMDS=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmds.obj" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"

"$(INTDIR)\dwhtmds.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmdd.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMDD=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdd.obj" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"

"$(INTDIR)\dwhtmdd.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMDD=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmdd.obj" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"

"$(INTDIR)\dwhtmdd.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMDD=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdd.obj" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"

"$(INTDIR)\dwhtmdd.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmd1.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMD1=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmd1.obj" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"

"$(INTDIR)\dwhtmd1.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMD1=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmd1.obj" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"

"$(INTDIR)\dwhtmd1.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMD1=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmd1.obj" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"

"$(INTDIR)\dwhtmd1.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD1) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmd0.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMD0=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmd0.obj" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"

"$(INTDIR)\dwhtmd0.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMD0=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmd0.obj" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"

"$(INTDIR)\dwhtmd0.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMD0=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmd0.obj" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"

"$(INTDIR)\dwhtmd0.sbr" : $(SOURCE) $(DEP_CPP_DWHTMD0) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmdb.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMDB=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdb.obj" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"

"$(INTDIR)\dwhtmdb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMDB=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmdb.obj" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"

"$(INTDIR)\dwhtmdb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMDB=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdb.obj" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"

"$(INTDIR)\dwhtmdb.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwhtmdm.cpp

!IF  "$(CFG)" == "dwhtm - Win32 Release"

DEP_CPP_DWHTMDM=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdm.obj" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"

"$(INTDIR)\dwhtmdm.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

DEP_CPP_DWHTMDM=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	

"$(INTDIR)\dwhtmdm.obj" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"

"$(INTDIR)\dwhtmdm.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

DEP_CPP_DWHTMDM=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcini.h"\
	".\..\base\dclist.h"\
	".\..\base\dcmacr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwhtm.h"\
	".\dwhtmfl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwhtmdm.obj" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"

"$(INTDIR)\dwhtmdm.sbr" : $(SOURCE) $(DEP_CPP_DWHTMDM) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
