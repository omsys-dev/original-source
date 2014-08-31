# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=dwrtf - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to dwrtf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dwrtf - Win32 Release" && "$(CFG)" != "dwrtf - Win32 Debug" &&\
 "$(CFG)" != "dwrtf - Win32 DbgRel"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dwrtf.mak" CFG="dwrtf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dwrtf - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwrtf - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwrtf - Win32 DbgRel" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "dwrtf - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "dwrtf - Win32 Release"

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
TargetName=dwrtf
# End Custom Macros

ALL : "$(OUTDIR)\dwrtf.dll" "$(OUTDIR)\dwrtf.bsc" "c:\winnt\system32\dwrtf.dll"

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
	-@erase "$(INTDIR)\dwrtf.obj"
	-@erase "$(INTDIR)\dwrtf.res"
	-@erase "$(INTDIR)\dwrtf.sbr"
	-@erase "$(INTDIR)\dwrtfan.obj"
	-@erase "$(INTDIR)\dwrtfan.sbr"
	-@erase "$(INTDIR)\dwrtffl.obj"
	-@erase "$(INTDIR)\dwrtffl.sbr"
	-@erase "$(INTDIR)\dwrtfgr.obj"
	-@erase "$(INTDIR)\dwrtfgr.sbr"
	-@erase "$(INTDIR)\dwrtfgt.obj"
	-@erase "$(INTDIR)\dwrtfgt.sbr"
	-@erase "$(INTDIR)\dwrtfin.obj"
	-@erase "$(INTDIR)\dwrtfin.sbr"
	-@erase "$(INTDIR)\dwrtfmc.obj"
	-@erase "$(INTDIR)\dwrtfmc.sbr"
	-@erase "$(INTDIR)\dwrtfpp.obj"
	-@erase "$(INTDIR)\dwrtfpp.sbr"
	-@erase "$(INTDIR)\dwrtfrf.obj"
	-@erase "$(INTDIR)\dwrtfrf.sbr"
	-@erase "$(INTDIR)\dwrtftb.obj"
	-@erase "$(INTDIR)\dwrtftb.sbr"
	-@erase "$(INTDIR)\dwrtftx.obj"
	-@erase "$(INTDIR)\dwrtftx.sbr"
	-@erase "$(OUTDIR)\dwrtf.bsc"
	-@erase "$(OUTDIR)\dwrtf.dll"
	-@erase "$(OUTDIR)\dwrtf.exp"
	-@erase "$(OUTDIR)\dwrtf.lib"
	-@erase "c:\winnt\system32\dwrtf.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwrtf.pch" /YX /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\../dll/WinRel/
CPP_SBRS=.\../dll/WinRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwrtf.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwrtf.bsc" 
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
	"$(INTDIR)\dwrtf.sbr" \
	"$(INTDIR)\dwrtfan.sbr" \
	"$(INTDIR)\dwrtffl.sbr" \
	"$(INTDIR)\dwrtfgr.sbr" \
	"$(INTDIR)\dwrtfgt.sbr" \
	"$(INTDIR)\dwrtfin.sbr" \
	"$(INTDIR)\dwrtfmc.sbr" \
	"$(INTDIR)\dwrtfpp.sbr" \
	"$(INTDIR)\dwrtfrf.sbr" \
	"$(INTDIR)\dwrtftb.sbr" \
	"$(INTDIR)\dwrtftx.sbr"

"$(OUTDIR)\dwrtf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/dwrtf.pdb" /machine:I386\
 /out:"$(OUTDIR)/dwrtf.dll" /implib:"$(OUTDIR)/dwrtf.lib" 
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
	"$(INTDIR)\dwrtf.obj" \
	"$(INTDIR)\dwrtf.res" \
	"$(INTDIR)\dwrtfan.obj" \
	"$(INTDIR)\dwrtffl.obj" \
	"$(INTDIR)\dwrtfgr.obj" \
	"$(INTDIR)\dwrtfgt.obj" \
	"$(INTDIR)\dwrtfin.obj" \
	"$(INTDIR)\dwrtfmc.obj" \
	"$(INTDIR)\dwrtfpp.obj" \
	"$(INTDIR)\dwrtfrf.obj" \
	"$(INTDIR)\dwrtftb.obj" \
	"$(INTDIR)\dwrtftx.obj"

"$(OUTDIR)\dwrtf.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinRel\dwrtf.dll
TargetName=dwrtf
InputPath=\cvs\dll\WinRel\dwrtf.dll
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

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
TargetName=dwrtf
# End Custom Macros

ALL : "$(OUTDIR)\dwrtf.dll" "$(OUTDIR)\dwrtf.bsc" "c:\winnt\system32\dwrtf.dll"

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
	-@erase "$(INTDIR)\dwrtf.obj"
	-@erase "$(INTDIR)\dwrtf.res"
	-@erase "$(INTDIR)\dwrtf.sbr"
	-@erase "$(INTDIR)\dwrtfan.obj"
	-@erase "$(INTDIR)\dwrtfan.sbr"
	-@erase "$(INTDIR)\dwrtffl.obj"
	-@erase "$(INTDIR)\dwrtffl.sbr"
	-@erase "$(INTDIR)\dwrtfgr.obj"
	-@erase "$(INTDIR)\dwrtfgr.sbr"
	-@erase "$(INTDIR)\dwrtfgt.obj"
	-@erase "$(INTDIR)\dwrtfgt.sbr"
	-@erase "$(INTDIR)\dwrtfin.obj"
	-@erase "$(INTDIR)\dwrtfin.sbr"
	-@erase "$(INTDIR)\dwrtfmc.obj"
	-@erase "$(INTDIR)\dwrtfmc.sbr"
	-@erase "$(INTDIR)\dwrtfpp.obj"
	-@erase "$(INTDIR)\dwrtfpp.sbr"
	-@erase "$(INTDIR)\dwrtfrf.obj"
	-@erase "$(INTDIR)\dwrtfrf.sbr"
	-@erase "$(INTDIR)\dwrtftb.obj"
	-@erase "$(INTDIR)\dwrtftb.sbr"
	-@erase "$(INTDIR)\dwrtftx.obj"
	-@erase "$(INTDIR)\dwrtftx.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dwrtf.bsc"
	-@erase "$(OUTDIR)\dwrtf.dll"
	-@erase "$(OUTDIR)\dwrtf.exp"
	-@erase "$(OUTDIR)\dwrtf.lib"
	-@erase "$(OUTDIR)\dwrtf.pdb"
	-@erase "c:\winnt\system32\dwrtf.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwrtf.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDebug/
CPP_SBRS=.\../dll/WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwrtf.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwrtf.bsc" 
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
	"$(INTDIR)\dwrtf.sbr" \
	"$(INTDIR)\dwrtfan.sbr" \
	"$(INTDIR)\dwrtffl.sbr" \
	"$(INTDIR)\dwrtfgr.sbr" \
	"$(INTDIR)\dwrtfgt.sbr" \
	"$(INTDIR)\dwrtfin.sbr" \
	"$(INTDIR)\dwrtfmc.sbr" \
	"$(INTDIR)\dwrtfpp.sbr" \
	"$(INTDIR)\dwrtfrf.sbr" \
	"$(INTDIR)\dwrtftb.sbr" \
	"$(INTDIR)\dwrtftx.sbr"

"$(OUTDIR)\dwrtf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/dwrtf.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dwrtf.dll" /implib:"$(OUTDIR)/dwrtf.lib" 
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
	"$(INTDIR)\dwrtf.obj" \
	"$(INTDIR)\dwrtf.res" \
	"$(INTDIR)\dwrtfan.obj" \
	"$(INTDIR)\dwrtffl.obj" \
	"$(INTDIR)\dwrtfgr.obj" \
	"$(INTDIR)\dwrtfgt.obj" \
	"$(INTDIR)\dwrtfin.obj" \
	"$(INTDIR)\dwrtfmc.obj" \
	"$(INTDIR)\dwrtfpp.obj" \
	"$(INTDIR)\dwrtfrf.obj" \
	"$(INTDIR)\dwrtftb.obj" \
	"$(INTDIR)\dwrtftx.obj"

"$(OUTDIR)\dwrtf.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinDebug\dwrtf.dll
TargetName=dwrtf
InputPath=\cvs\dll\WinDebug\dwrtf.dll
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dwrtf___"
# PROP BASE Intermediate_Dir "dwrtf___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../dll/DbgRel"
# PROP Intermediate_Dir "../dll/DbgRel"
# PROP Target_Dir ""
OUTDIR=.\../dll/DbgRel
INTDIR=.\../dll/DbgRel
# Begin Custom Macros
TargetName=dwrtf
# End Custom Macros

ALL : "$(OUTDIR)\dwrtf.dll" "$(OUTDIR)\dwrtf.bsc" "c:\winnt\system32\dwrtf.dll"

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
	-@erase "$(INTDIR)\dwrtf.obj"
	-@erase "$(INTDIR)\dwrtf.res"
	-@erase "$(INTDIR)\dwrtf.sbr"
	-@erase "$(INTDIR)\dwrtfan.obj"
	-@erase "$(INTDIR)\dwrtfan.sbr"
	-@erase "$(INTDIR)\dwrtffl.obj"
	-@erase "$(INTDIR)\dwrtffl.sbr"
	-@erase "$(INTDIR)\dwrtfgr.obj"
	-@erase "$(INTDIR)\dwrtfgr.sbr"
	-@erase "$(INTDIR)\dwrtfgt.obj"
	-@erase "$(INTDIR)\dwrtfgt.sbr"
	-@erase "$(INTDIR)\dwrtfin.obj"
	-@erase "$(INTDIR)\dwrtfin.sbr"
	-@erase "$(INTDIR)\dwrtfmc.obj"
	-@erase "$(INTDIR)\dwrtfmc.sbr"
	-@erase "$(INTDIR)\dwrtfpp.obj"
	-@erase "$(INTDIR)\dwrtfpp.sbr"
	-@erase "$(INTDIR)\dwrtfrf.obj"
	-@erase "$(INTDIR)\dwrtfrf.sbr"
	-@erase "$(INTDIR)\dwrtftb.obj"
	-@erase "$(INTDIR)\dwrtftb.sbr"
	-@erase "$(INTDIR)\dwrtftx.obj"
	-@erase "$(INTDIR)\dwrtftx.sbr"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dwrtf.bsc"
	-@erase "$(OUTDIR)\dwrtf.dll"
	-@erase "$(OUTDIR)\dwrtf.exp"
	-@erase "$(OUTDIR)\dwrtf.lib"
	-@erase "$(OUTDIR)\dwrtf.pdb"
	-@erase "c:\winnt\system32\dwrtf.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /Zi /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /Zi /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwrtf.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/DbgRel/
CPP_SBRS=.\../dll/DbgRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwrtf.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwrtf.bsc" 
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
	"$(INTDIR)\dwrtf.sbr" \
	"$(INTDIR)\dwrtfan.sbr" \
	"$(INTDIR)\dwrtffl.sbr" \
	"$(INTDIR)\dwrtfgr.sbr" \
	"$(INTDIR)\dwrtfgt.sbr" \
	"$(INTDIR)\dwrtfin.sbr" \
	"$(INTDIR)\dwrtfmc.sbr" \
	"$(INTDIR)\dwrtfpp.sbr" \
	"$(INTDIR)\dwrtfrf.sbr" \
	"$(INTDIR)\dwrtftb.sbr" \
	"$(INTDIR)\dwrtftx.sbr"

"$(OUTDIR)\dwrtf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67e00000 /version:2.1 /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/dwrtf.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dwrtf.dll" /implib:"$(OUTDIR)/dwrtf.lib" 
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
	"$(INTDIR)\dwrtf.obj" \
	"$(INTDIR)\dwrtf.res" \
	"$(INTDIR)\dwrtfan.obj" \
	"$(INTDIR)\dwrtffl.obj" \
	"$(INTDIR)\dwrtfgr.obj" \
	"$(INTDIR)\dwrtfgt.obj" \
	"$(INTDIR)\dwrtfin.obj" \
	"$(INTDIR)\dwrtfmc.obj" \
	"$(INTDIR)\dwrtfpp.obj" \
	"$(INTDIR)\dwrtfrf.obj" \
	"$(INTDIR)\dwrtftb.obj" \
	"$(INTDIR)\dwrtftx.obj"

"$(OUTDIR)\dwrtf.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\DbgRel\dwrtf.dll
TargetName=dwrtf
InputPath=\cvs\dll\DbgRel\dwrtf.dll
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

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

# Name "dwrtf - Win32 Release"
# Name "dwrtf - Win32 Debug"
# Name "dwrtf - Win32 DbgRel"

!IF  "$(CFG)" == "dwrtf - Win32 Release"

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dwrtf.rc

!IF  "$(CFG)" == "dwrtf - Win32 Release"


"$(INTDIR)\dwrtf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


"$(INTDIR)\dwrtf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


"$(INTDIR)\dwrtf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtftx.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTF=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtftx.obj" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"

"$(INTDIR)\dwrtftx.sbr" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTF=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtftx.obj" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"

"$(INTDIR)\dwrtftx.sbr" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTF=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtftx.obj" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"

"$(INTDIR)\dwrtftx.sbr" : $(SOURCE) $(DEP_CPP_DWRTF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtftb.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFT=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtftb.obj" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"

"$(INTDIR)\dwrtftb.sbr" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFT=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtftb.obj" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"

"$(INTDIR)\dwrtftb.sbr" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFT=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtftb.obj" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"

"$(INTDIR)\dwrtftb.sbr" : $(SOURCE) $(DEP_CPP_DWRTFT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtfrf.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFR=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfrf.obj" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"

"$(INTDIR)\dwrtfrf.sbr" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFR=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfrf.obj" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"

"$(INTDIR)\dwrtfrf.sbr" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFR=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfrf.obj" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"

"$(INTDIR)\dwrtfrf.sbr" : $(SOURCE) $(DEP_CPP_DWRTFR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtfgt.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFG=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfgt.obj" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"

"$(INTDIR)\dwrtfgt.sbr" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFG=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfgt.obj" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"

"$(INTDIR)\dwrtfgt.sbr" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFG=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfgt.obj" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"

"$(INTDIR)\dwrtfgt.sbr" : $(SOURCE) $(DEP_CPP_DWRTFG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtfgr.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFGR=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfgr.obj" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"

"$(INTDIR)\dwrtfgr.sbr" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFGR=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfgr.obj" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"

"$(INTDIR)\dwrtfgr.sbr" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFGR=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfgr.obj" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"

"$(INTDIR)\dwrtfgr.sbr" : $(SOURCE) $(DEP_CPP_DWRTFGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtffl.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtffl.obj" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"

"$(INTDIR)\dwrtffl.sbr" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtffl.obj" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"

"$(INTDIR)\dwrtffl.sbr" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFF=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	".\dwrtffl.h"\
	

"$(INTDIR)\dwrtffl.obj" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"

"$(INTDIR)\dwrtffl.sbr" : $(SOURCE) $(DEP_CPP_DWRTFF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtfan.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfan.obj" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"

"$(INTDIR)\dwrtfan.sbr" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWRTFA=\
	".\CurrAnumValStr"\
	

"$(INTDIR)\dwrtfan.obj" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"

"$(INTDIR)\dwrtfan.sbr" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFA=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfan.obj" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"

"$(INTDIR)\dwrtfan.sbr" : $(SOURCE) $(DEP_CPP_DWRTFA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtf.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTF_=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtf.obj" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"

"$(INTDIR)\dwrtf.sbr" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTF_=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWRTF_=\
	".\this"\
	

"$(INTDIR)\dwrtf.obj" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"

"$(INTDIR)\dwrtf.sbr" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTF_=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtf.obj" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"

"$(INTDIR)\dwrtf.sbr" : $(SOURCE) $(DEP_CPP_DWRTF_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DCGLO=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DCGLO=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DCGLO=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	

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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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
	

!IF  "$(CFG)" == "dwrtf - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"


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

SOURCE=.\dwrtfpp.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfpp.obj" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"

"$(INTDIR)\dwrtfpp.sbr" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfpp.obj" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"

"$(INTDIR)\dwrtfpp.sbr" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFP=\
	"..\base\dcfilp.h"\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfpp.obj" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"

"$(INTDIR)\dwrtfpp.sbr" : $(SOURCE) $(DEP_CPP_DWRTFP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwrtfin.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfin.obj" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"

"$(INTDIR)\dwrtfin.sbr" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFI=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfin.obj" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"

"$(INTDIR)\dwrtfin.sbr" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfin.obj" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"

"$(INTDIR)\dwrtfin.sbr" : $(SOURCE) $(DEP_CPP_DWRTFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmacr.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcfili.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DCMAC=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
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

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DCINI=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcini.h"\
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

SOURCE=.\dwrtfmc.cpp

!IF  "$(CFG)" == "dwrtf - Win32 Release"

DEP_CPP_DWRTFM=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfmc.obj" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"

"$(INTDIR)\dwrtfmc.sbr" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 Debug"

DEP_CPP_DWRTFM=\
	".\..\base\dcappl.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfili.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	

"$(INTDIR)\dwrtfmc.obj" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"

"$(INTDIR)\dwrtfmc.sbr" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "dwrtf - Win32 DbgRel"

DEP_CPP_DWRTFM=\
	".\..\base\dcappl.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcctlg.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfili.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwrtf.h"\
	".\dwrtffl.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwrtfmc.obj" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"

"$(INTDIR)\dwrtfmc.sbr" : $(SOURCE) $(DEP_CPP_DWRTFM) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
