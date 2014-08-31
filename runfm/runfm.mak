# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=runfm - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to runfm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "runfm - Win32 Release" && "$(CFG)" != "runfm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "runfm.mak" CFG="runfm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "runfm - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "runfm - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "runfm - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "runfm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../exe/Release"
# PROP Intermediate_Dir "../exe/Release"
# PROP Target_Dir ""
OUTDIR=.\../exe/Release
INTDIR=.\../exe/Release
# Begin Custom Macros
TargetName=runfm
# End Custom Macros

ALL : "$(OUTDIR)\runfm.exe" "$(OUTDIR)\runfm.bsc" "c:\winnt\system32\runfm.dll"

CLEAN : 
	-@erase "$(INTDIR)\runfm.obj"
	-@erase "$(INTDIR)\runfm.res"
	-@erase "$(INTDIR)\runfm.sbr"
	-@erase "$(OUTDIR)\runfm.bsc"
	-@erase "$(OUTDIR)\runfm.exe"
	-@erase "$(OUTDIR)\runfm.ilk"
	-@erase "c:\winnt\system32\runfm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/runfm.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../exe/Release/
CPP_SBRS=.\../exe/Release/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/runfm.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/runfm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\runfm.sbr"

"$(OUTDIR)\runfm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib /nologo /subsystem:console /incremental:yes /machine:I386
# SUBTRACT LINK32 /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/runfm.pdb" /machine:I386 /out:"$(OUTDIR)/runfm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\runfm.obj" \
	"$(INTDIR)\runfm.res"

"$(OUTDIR)\runfm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\exe\Release\runfm.exe
TargetName=runfm
InputPath=\cvs\exe\Release\runfm.exe
SOURCE=$(InputPath)

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "runfm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../exe/Debug"
# PROP Intermediate_Dir "../exe/Debug"
# PROP Target_Dir ""
OUTDIR=.\../exe/Debug
INTDIR=.\../exe/Debug
# Begin Custom Macros
TargetName=runfm
# End Custom Macros

ALL : "$(OUTDIR)\runfm.exe" "$(OUTDIR)\runfm.bsc" "c:\winnt\system32\runfm.dll"

CLEAN : 
	-@erase "$(INTDIR)\runfm.obj"
	-@erase "$(INTDIR)\runfm.res"
	-@erase "$(INTDIR)\runfm.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\runfm.bsc"
	-@erase "$(OUTDIR)\runfm.exe"
	-@erase "$(OUTDIR)\runfm.pdb"
	-@erase "c:\winnt\system32\runfm.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/runfm.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 
CPP_OBJS=.\../exe/Debug/
CPP_SBRS=.\../exe/Debug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/runfm.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/runfm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\runfm.sbr"

"$(OUTDIR)\runfm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"libc.lib"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/runfm.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib"\
 /out:"$(OUTDIR)/runfm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\runfm.obj" \
	"$(INTDIR)\runfm.res"

"$(OUTDIR)\runfm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\exe\Debug\runfm.exe
TargetName=runfm
InputPath=\cvs\exe\Debug\runfm.exe
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

# Name "runfm - Win32 Release"
# Name "runfm - Win32 Debug"

!IF  "$(CFG)" == "runfm - Win32 Release"

!ELSEIF  "$(CFG)" == "runfm - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\runfm.c
DEP_CPP_RUNFM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fmemory.h"\
	"..\..\FDK6\include\fpath.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fchannel.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\runfm.obj" : $(SOURCE) $(DEP_CPP_RUNFM) "$(INTDIR)"

"$(INTDIR)\runfm.sbr" : $(SOURCE) $(DEP_CPP_RUNFM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\runfm.rc

"$(INTDIR)\runfm.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
