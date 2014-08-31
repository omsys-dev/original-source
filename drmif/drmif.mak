# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=drmif - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to drmif - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "drmif - Win32 Release" && "$(CFG)" != "drmif - Win32 Debug" &&\
 "$(CFG)" != "drmif - Win32 Sample"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "drmif.mak" CFG="drmif - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "drmif - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drmif - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drmif - Win32 Sample" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "drmif - Win32 Release"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "drmif - Win32 Release"

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
TargetName=drmif
# End Custom Macros

ALL : "$(OUTDIR)\drmif.dll" "$(OUTDIR)\drmif.bsc" "c:\Winnt\System32\drmif"

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
	-@erase "$(INTDIR)\dclog.obj"
	-@erase "$(INTDIR)\dclog.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\drmif.obj"
	-@erase "$(INTDIR)\drmif.res"
	-@erase "$(INTDIR)\drmif.sbr"
	-@erase "$(INTDIR)\drmifan.obj"
	-@erase "$(INTDIR)\drmifan.sbr"
	-@erase "$(INTDIR)\drmiffl.obj"
	-@erase "$(INTDIR)\drmiffl.sbr"
	-@erase "$(INTDIR)\drmifgr.obj"
	-@erase "$(INTDIR)\drmifgr.sbr"
	-@erase "$(INTDIR)\drmifin.obj"
	-@erase "$(INTDIR)\drmifin.sbr"
	-@erase "$(INTDIR)\drmifsm.obj"
	-@erase "$(INTDIR)\drmifsm.sbr"
	-@erase "$(INTDIR)\drmiftb.obj"
	-@erase "$(INTDIR)\drmiftb.sbr"
	-@erase "$(INTDIR)\drmiftx.obj"
	-@erase "$(INTDIR)\drmiftx.sbr"
	-@erase "$(OUTDIR)\drmif.bsc"
	-@erase "$(OUTDIR)\drmif.dll"
	-@erase "$(OUTDIR)\drmif.exp"
	-@erase "$(OUTDIR)\drmif.lib"
	-@erase "c:\Winnt\System32\drmif"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/drmif.pch" /YX /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\../dll/WinRel/
CPP_SBRS=.\../dll/WinRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/drmif.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/drmif.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dclog.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\drmif.sbr" \
	"$(INTDIR)\drmifan.sbr" \
	"$(INTDIR)\drmiffl.sbr" \
	"$(INTDIR)\drmifgr.sbr" \
	"$(INTDIR)\drmifin.sbr" \
	"$(INTDIR)\drmifsm.sbr" \
	"$(INTDIR)\drmiftb.sbr" \
	"$(INTDIR)\drmiftx.sbr"

"$(OUTDIR)\drmif.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/drmif.pdb" /machine:I386\
 /out:"$(OUTDIR)/drmif.dll" /implib:"$(OUTDIR)/drmif.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dclog.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\drmif.obj" \
	"$(INTDIR)\drmif.res" \
	"$(INTDIR)\drmifan.obj" \
	"$(INTDIR)\drmiffl.obj" \
	"$(INTDIR)\drmifgr.obj" \
	"$(INTDIR)\drmifin.obj" \
	"$(INTDIR)\drmifsm.obj" \
	"$(INTDIR)\drmiftb.obj" \
	"$(INTDIR)\drmiftx.obj"

"$(OUTDIR)\drmif.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinRel\drmif.dll
TargetName=drmif
InputPath=\cvs\dll\WinRel\drmif.dll
SOURCE=$(InputPath)

"c:\Winnt\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\Winnt\System32

# End Custom Build

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"

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
TargetName=drmif
# End Custom Macros

ALL : "$(OUTDIR)\drmif.dll" "$(OUTDIR)\drmif.bsc" "c:\Winnt\System32\drmif"

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
	-@erase "$(INTDIR)\dclog.obj"
	-@erase "$(INTDIR)\dclog.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\drmif.obj"
	-@erase "$(INTDIR)\drmif.res"
	-@erase "$(INTDIR)\drmif.sbr"
	-@erase "$(INTDIR)\drmifan.obj"
	-@erase "$(INTDIR)\drmifan.sbr"
	-@erase "$(INTDIR)\drmiffl.obj"
	-@erase "$(INTDIR)\drmiffl.sbr"
	-@erase "$(INTDIR)\drmifgr.obj"
	-@erase "$(INTDIR)\drmifgr.sbr"
	-@erase "$(INTDIR)\drmifin.obj"
	-@erase "$(INTDIR)\drmifin.sbr"
	-@erase "$(INTDIR)\drmifsm.obj"
	-@erase "$(INTDIR)\drmifsm.sbr"
	-@erase "$(INTDIR)\drmiftb.obj"
	-@erase "$(INTDIR)\drmiftb.sbr"
	-@erase "$(INTDIR)\drmiftx.obj"
	-@erase "$(INTDIR)\drmiftx.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\drmif.bsc"
	-@erase "$(OUTDIR)\drmif.dll"
	-@erase "$(OUTDIR)\drmif.exp"
	-@erase "$(OUTDIR)\drmif.lib"
	-@erase "$(OUTDIR)\drmif.pdb"
	-@erase "c:\Winnt\System32\drmif"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /Fr /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WIN32" /D "_DLL" /Fr"$(INTDIR)/" /Fp"$(INTDIR)/drmif.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDebug/
CPP_SBRS=.\../dll/WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/drmif.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/drmif.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dclog.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\drmif.sbr" \
	"$(INTDIR)\drmifan.sbr" \
	"$(INTDIR)\drmiffl.sbr" \
	"$(INTDIR)\drmifgr.sbr" \
	"$(INTDIR)\drmifin.sbr" \
	"$(INTDIR)\drmifsm.sbr" \
	"$(INTDIR)\drmiftb.sbr" \
	"$(INTDIR)\drmiftx.sbr"

"$(OUTDIR)\drmif.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/drmif.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/drmif.dll" /implib:"$(OUTDIR)/drmif.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dclog.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\drmif.obj" \
	"$(INTDIR)\drmif.res" \
	"$(INTDIR)\drmifan.obj" \
	"$(INTDIR)\drmiffl.obj" \
	"$(INTDIR)\drmifgr.obj" \
	"$(INTDIR)\drmifin.obj" \
	"$(INTDIR)\drmifsm.obj" \
	"$(INTDIR)\drmiftb.obj" \
	"$(INTDIR)\drmiftx.obj"

"$(OUTDIR)\drmif.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinDebug\drmif.dll
TargetName=drmif
InputPath=\cvs\dll\WinDebug\drmif.dll
SOURCE=$(InputPath)

"c:\Winnt\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\Winnt\System32

# End Custom Build

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drmif___"
# PROP BASE Intermediate_Dir "drmif___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../dll/Sample"
# PROP Intermediate_Dir "../dll/Sample"
# PROP Target_Dir ""
OUTDIR=.\../dll/Sample
INTDIR=.\../dll/Sample
# Begin Custom Macros
TargetName=drmif
# End Custom Macros

ALL : "$(OUTDIR)\drmif.dll" "$(OUTDIR)\drmif.bsc" "c:\Winnt\System32\drmif"

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
	-@erase "$(INTDIR)\dclog.obj"
	-@erase "$(INTDIR)\dclog.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\drmif.obj"
	-@erase "$(INTDIR)\drmif.res"
	-@erase "$(INTDIR)\drmif.sbr"
	-@erase "$(INTDIR)\drmifan.obj"
	-@erase "$(INTDIR)\drmifan.sbr"
	-@erase "$(INTDIR)\drmiffl.obj"
	-@erase "$(INTDIR)\drmiffl.sbr"
	-@erase "$(INTDIR)\drmifgr.obj"
	-@erase "$(INTDIR)\drmifgr.sbr"
	-@erase "$(INTDIR)\drmifin.obj"
	-@erase "$(INTDIR)\drmifin.sbr"
	-@erase "$(INTDIR)\drmifsm.obj"
	-@erase "$(INTDIR)\drmifsm.sbr"
	-@erase "$(INTDIR)\drmiftb.obj"
	-@erase "$(INTDIR)\drmiftb.sbr"
	-@erase "$(INTDIR)\drmiftx.obj"
	-@erase "$(INTDIR)\drmiftx.sbr"
	-@erase "$(OUTDIR)\drmif.bsc"
	-@erase "$(OUTDIR)\drmif.dll"
	-@erase "$(OUTDIR)\drmif.exp"
	-@erase "$(OUTDIR)\drmif.lib"
	-@erase "c:\Winnt\System32\drmif"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "SAMPLE" /Fr /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /D "SAMPLE" /Fr"$(INTDIR)/" /Fp"$(INTDIR)/drmif.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/Sample/
CPP_SBRS=.\../dll/Sample/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/drmif.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/drmif.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcini.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dclog.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\drmif.sbr" \
	"$(INTDIR)\drmifan.sbr" \
	"$(INTDIR)\drmiffl.sbr" \
	"$(INTDIR)\drmifgr.sbr" \
	"$(INTDIR)\drmifin.sbr" \
	"$(INTDIR)\drmifsm.sbr" \
	"$(INTDIR)\drmiftb.sbr" \
	"$(INTDIR)\drmiftx.sbr"

"$(OUTDIR)\drmif.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll /pdb:none /machine:I386
# SUBTRACT LINK32 /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x67000000 /version:2.1 /subsystem:windows /dll\
 /pdb:none /machine:I386 /out:"$(OUTDIR)/drmif.dll"\
 /implib:"$(OUTDIR)/drmif.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcini.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dclog.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\drmif.obj" \
	"$(INTDIR)\drmif.res" \
	"$(INTDIR)\drmifan.obj" \
	"$(INTDIR)\drmiffl.obj" \
	"$(INTDIR)\drmifgr.obj" \
	"$(INTDIR)\drmifin.obj" \
	"$(INTDIR)\drmifsm.obj" \
	"$(INTDIR)\drmiftb.obj" \
	"$(INTDIR)\drmiftx.obj"

"$(OUTDIR)\drmif.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\Sample\drmif.dll
TargetName=drmif
InputPath=\cvs\dll\Sample\drmif.dll
SOURCE=$(InputPath)

"c:\Winnt\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\Winnt\System32

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

# Name "drmif - Win32 Release"
# Name "drmif - Win32 Debug"
# Name "drmif - Win32 Sample"

!IF  "$(CFG)" == "drmif - Win32 Release"

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\drmif.rc

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmif.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmif.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmif.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmiftx.cpp
DEP_CPP_DRMIF=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmiftx.obj" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"

"$(INTDIR)\drmiftx.sbr" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmiftx.obj" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"

"$(INTDIR)\drmiftx.sbr" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmiftx.obj" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"

"$(INTDIR)\drmiftx.sbr" : $(SOURCE) $(DEP_CPP_DRMIF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmiftb.cpp
DEP_CPP_DRMIFT=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmiftb.obj" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"

"$(INTDIR)\drmiftb.sbr" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmiftb.obj" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"

"$(INTDIR)\drmiftb.sbr" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmiftb.obj" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"

"$(INTDIR)\drmiftb.sbr" : $(SOURCE) $(DEP_CPP_DRMIFT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmifgr.cpp
DEP_CPP_DRMIFG=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmifgr.obj" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"

"$(INTDIR)\drmifgr.sbr" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmifgr.obj" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"

"$(INTDIR)\drmifgr.sbr" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmifgr.obj" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"

"$(INTDIR)\drmifgr.sbr" : $(SOURCE) $(DEP_CPP_DRMIFG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmiffl.cpp
DEP_CPP_DRMIFF=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmiffl.obj" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"

"$(INTDIR)\drmiffl.sbr" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmiffl.obj" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"

"$(INTDIR)\drmiffl.sbr" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmiffl.obj" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"

"$(INTDIR)\drmiffl.sbr" : $(SOURCE) $(DEP_CPP_DRMIFF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmifan.cpp
DEP_CPP_DRMIFA=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmifan.obj" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"

"$(INTDIR)\drmifan.sbr" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmifan.obj" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"

"$(INTDIR)\drmifan.sbr" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmifan.obj" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"

"$(INTDIR)\drmifan.sbr" : $(SOURCE) $(DEP_CPP_DRMIFA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmif.cpp
DEP_CPP_DRMIF_=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmif.obj" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"

"$(INTDIR)\drmif.sbr" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmif.obj" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"

"$(INTDIR)\drmif.sbr" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmif.obj" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"

"$(INTDIR)\drmif.sbr" : $(SOURCE) $(DEP_CPP_DRMIF_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	".\..\base\dcmem.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	".\..\base\dcmem.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


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

SOURCE=.\drmifsm.cpp
DEP_CPP_DRMIFS=\
	".\drmif.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmifsm.obj" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"

"$(INTDIR)\drmifsm.sbr" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmifsm.obj" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"

"$(INTDIR)\drmifsm.sbr" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmifsm.obj" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"

"$(INTDIR)\drmifsm.sbr" : $(SOURCE) $(DEP_CPP_DRMIFS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\drmifin.cpp
DEP_CPP_DRMIFI=\
	".\drmif.h"\
	".\drmiffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

!IF  "$(CFG)" == "drmif - Win32 Release"


"$(INTDIR)\drmifin.obj" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"

"$(INTDIR)\drmifin.sbr" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"


"$(INTDIR)\drmifin.obj" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"

"$(INTDIR)\drmifin.sbr" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"


"$(INTDIR)\drmifin.obj" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"

"$(INTDIR)\drmifin.sbr" : $(SOURCE) $(DEP_CPP_DRMIFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcini.cpp

!IF  "$(CFG)" == "drmif - Win32 Release"

DEP_CPP_DCINI=\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcini.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"

DEP_CPP_DCINI=\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"

DEP_CPP_DCINI=\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\dcport.h"\
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

SOURCE=\cvs\base\dclog.cpp

!IF  "$(CFG)" == "drmif - Win32 Release"

DEP_CPP_DCLOG=\
	"..\base\dclog.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DCLOG=\
	"..\base\DClog"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclog.obj" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclog.sbr" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Debug"

DEP_CPP_DCLOG=\
	"..\base\dclog.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclog.obj" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclog.sbr" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "drmif - Win32 Sample"

DEP_CPP_DCLOG=\
	"..\base\dclog.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclog.obj" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclog.sbr" : $(SOURCE) $(DEP_CPP_DCLOG) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
