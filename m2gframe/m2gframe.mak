# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=m2gframe - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to m2gframe - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "m2gframe - Win32 Release" && "$(CFG)" !=\
 "m2gframe - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "m2gframe.mak" CFG="m2gframe - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "m2gframe - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2gframe - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "m2gframe - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "m2gframe - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../fdk/Release"
# PROP Intermediate_Dir "../fdk/Release"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Release
INTDIR=.\../fdk/Release
# Begin Custom Macros
TargetName=m2gframe
# End Custom Macros

ALL : "$(OUTDIR)\m2gframe.dll" "$(OUTDIR)\m2gframe.bsc"\
 "..\..\FM7.1\fminit\plugins\m2gframe.dll" "..\..\FM7.1\fminit\plugins\.cache"

CLEAN : 
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
	-@erase "$(INTDIR)\m2gdlg.obj"
	-@erase "$(INTDIR)\m2gdlg.sbr"
	-@erase "$(INTDIR)\m2gframe.obj"
	-@erase "$(INTDIR)\m2gframe.res"
	-@erase "$(INTDIR)\m2gframe.sbr"
	-@erase "$(INTDIR)\m2gini.obj"
	-@erase "$(INTDIR)\m2gini.sbr"
	-@erase "$(INTDIR)\m2gmacr.obj"
	-@erase "$(INTDIR)\m2gmacr.sbr"
	-@erase "$(INTDIR)\m2gmenu.obj"
	-@erase "$(INTDIR)\m2gmenu.sbr"
	-@erase "$(OUTDIR)\m2gframe.bsc"
	-@erase "$(OUTDIR)\m2gframe.dll"
	-@erase "$(OUTDIR)\m2gframe.exp"
	-@erase "$(OUTDIR)\m2gframe.ilk"
	-@erase "$(OUTDIR)\m2gframe.lib"
	-@erase "..\..\FM7.1\fminit\plugins\.cache"
	-@erase "..\..\FM7.1\fminit\plugins\m2gframe.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "M2GFRAME" /FR /YX"m2gframe.h" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "M2GFRAME" /FR"$(INTDIR)/" /Fp"$(INTDIR)/m2gframe.pch" /YX"m2gframe.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Release/
CPP_SBRS=.\../fdk/Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2gframe.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2gframe.bsc" 
BSC32_SBRS= \
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
	"$(INTDIR)\m2gdlg.sbr" \
	"$(INTDIR)\m2gframe.sbr" \
	"$(INTDIR)\m2gini.sbr" \
	"$(INTDIR)\m2gmacr.sbr" \
	"$(INTDIR)\m2gmenu.sbr"

"$(OUTDIR)\m2gframe.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2gframe.pdb" /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2gframe.dll" /implib:"$(OUTDIR)/m2gframe.lib" 
LINK32_OBJS= \
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
	"$(INTDIR)\m2gdlg.obj" \
	"$(INTDIR)\m2gframe.obj" \
	"$(INTDIR)\m2gframe.res" \
	"$(INTDIR)\m2gini.obj" \
	"$(INTDIR)\m2gmacr.obj" \
	"$(INTDIR)\m2gmenu.obj"

"$(OUTDIR)\m2gframe.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Release\m2gframe.dll
TargetName=m2gframe
InputPath=\cvs\fdk\Release\m2gframe.dll
SOURCE=$(InputPath)

BuildCmds= \
	cmd /c copy $(TargetPath) d:\FM7.1\fminit\plugins \
	touch d:\FM7.1\fminit\plugins\.cache \
	cmd /c del /q d:\FM7.1\fminit\plugins\.cache \
	

"d:\FM7.1\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"d:\FM7.1\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "m2gframe - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../fdk/Debug"
# PROP Intermediate_Dir "../fdk/Debug"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Debug
INTDIR=.\../fdk/Debug
# Begin Custom Macros
TargetName=m2gframe
# End Custom Macros

ALL : "$(OUTDIR)\m2gframe.dll" "$(OUTDIR)\m2gframe.bsc"\
 "..\..\FM7.1\fminit\plugins\m2gframe.dll" "..\..\FM7.1\fminit\plugins\.cache"

CLEAN : 
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
	-@erase "$(INTDIR)\m2gdlg.obj"
	-@erase "$(INTDIR)\m2gdlg.sbr"
	-@erase "$(INTDIR)\m2gframe.obj"
	-@erase "$(INTDIR)\m2gframe.res"
	-@erase "$(INTDIR)\m2gframe.sbr"
	-@erase "$(INTDIR)\m2gini.obj"
	-@erase "$(INTDIR)\m2gini.sbr"
	-@erase "$(INTDIR)\m2gmacr.obj"
	-@erase "$(INTDIR)\m2gmacr.sbr"
	-@erase "$(INTDIR)\m2gmenu.obj"
	-@erase "$(INTDIR)\m2gmenu.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\m2gframe.bsc"
	-@erase "$(OUTDIR)\m2gframe.dll"
	-@erase "$(OUTDIR)\m2gframe.exp"
	-@erase "$(OUTDIR)\m2gframe.ilk"
	-@erase "$(OUTDIR)\m2gframe.lib"
	-@erase "$(OUTDIR)\m2gframe.pdb"
	-@erase "..\..\FM7.1\fminit\plugins\.cache"
	-@erase "..\..\FM7.1\fminit\plugins\m2gframe.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2GFRAME" /FR /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "M2GFRAME" /FR"$(INTDIR)/" /Fp"$(INTDIR)/m2gframe.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Debug/
CPP_SBRS=.\../fdk/Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2gframe.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2gframe.bsc" 
BSC32_SBRS= \
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
	"$(INTDIR)\m2gdlg.sbr" \
	"$(INTDIR)\m2gframe.sbr" \
	"$(INTDIR)\m2gini.sbr" \
	"$(INTDIR)\m2gmacr.sbr" \
	"$(INTDIR)\m2gmenu.sbr"

"$(OUTDIR)\m2gframe.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2gframe.pdb" /debug /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2gframe.dll" /implib:"$(OUTDIR)/m2gframe.lib" 
LINK32_OBJS= \
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
	"$(INTDIR)\m2gdlg.obj" \
	"$(INTDIR)\m2gframe.obj" \
	"$(INTDIR)\m2gframe.res" \
	"$(INTDIR)\m2gini.obj" \
	"$(INTDIR)\m2gmacr.obj" \
	"$(INTDIR)\m2gmenu.obj"

"$(OUTDIR)\m2gframe.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Debug\m2gframe.dll
TargetName=m2gframe
InputPath=\cvs\fdk\Debug\m2gframe.dll
SOURCE=$(InputPath)

BuildCmds= \
	cmd /c copy $(TargetPath) d:\FM7.1\fminit\plugins \
	touch d:\FM7.1\fminit\plugins\.cache \
	cmd /c del /q d:\FM7.1\fminit\plugins\.cache \
	

"d:\FM7.1\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"d:\FM7.1\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
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

# Name "m2gframe - Win32 Release"
# Name "m2gframe - Win32 Debug"

!IF  "$(CFG)" == "m2gframe - Win32 Release"

!ELSEIF  "$(CFG)" == "m2gframe - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\m2gframe.cpp
DEP_CPP_M2GFR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	"..\base\dcport.h"\
	".\m2gframe.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_M2GFR=\
	".\m2rhids.h"\
	

"$(INTDIR)\m2gframe.obj" : $(SOURCE) $(DEP_CPP_M2GFR) "$(INTDIR)"

"$(INTDIR)\m2gframe.sbr" : $(SOURCE) $(DEP_CPP_M2GFR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2gframe.rc

"$(INTDIR)\m2gframe.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2gmenu.cpp
DEP_CPP_M2GME=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	"..\base\dcport.h"\
	".\m2gframe.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_M2GME=\
	".\m2rhids.h"\
	

"$(INTDIR)\m2gmenu.obj" : $(SOURCE) $(DEP_CPP_M2GME) "$(INTDIR)"

"$(INTDIR)\m2gmenu.sbr" : $(SOURCE) $(DEP_CPP_M2GME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcctlg.cpp
DEP_CPP_DCCTL=\
	"..\base\dcctlg.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcctlg.obj" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcctlg.sbr" : $(SOURCE) $(DEP_CPP_DCCTL) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfile.cpp
DEP_CPP_DCFIL=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfili.cpp
DEP_CPP_DCFILI=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfili.obj" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfili.sbr" : $(SOURCE) $(DEP_CPP_DCFILI) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilp.cpp
DEP_CPP_DCFILP=\
	"..\base\dcfilp.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilp.obj" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilp.sbr" : $(SOURCE) $(DEP_CPP_DCFILP) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilr.cpp
DEP_CPP_DCFILR=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcfilw.cpp
DEP_CPP_DCFILW=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFILW) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFILW) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcglob.cpp
DEP_CPP_DCGLO=\
	"..\base\dcmem.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcini.cpp

!IF  "$(CFG)" == "m2gframe - Win32 Release"

DEP_CPP_DCINI=\
	"..\base\dcappl.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcini.obj" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcini.sbr" : $(SOURCE) $(DEP_CPP_DCINI) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "m2gframe - Win32 Debug"

DEP_CPP_DCINI=\
	"..\base\dcappl.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcnode.h"\
	

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

SOURCE=\cvs\base\dclist.cpp
DEP_CPP_DCLIS=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmacr.cpp
DEP_CPP_DCMAC=\
	"..\base\dcappl.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmacr.obj" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmacr.sbr" : $(SOURCE) $(DEP_CPP_DCMAC) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmem.cpp
DEP_CPP_DCMEM=\
	"..\base\dcmem.h"\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnode.cpp
DEP_CPP_DCNOD=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnodt.cpp
DEP_CPP_DCNODT=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNODT) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNODT) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcbase.cpp
DEP_CPP_DCBAS=\
	"..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2gini.cpp
DEP_CPP_M2GIN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	"..\base\dcport.h"\
	".\m2gframe.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_M2GIN=\
	".\m2rhids.h"\
	

"$(INTDIR)\m2gini.obj" : $(SOURCE) $(DEP_CPP_M2GIN) "$(INTDIR)"

"$(INTDIR)\m2gini.sbr" : $(SOURCE) $(DEP_CPP_M2GIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2gmacr.cpp
DEP_CPP_M2GMA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	"..\base\dcport.h"\
	".\m2gframe.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_M2GMA=\
	".\m2rhids.h"\
	

"$(INTDIR)\m2gmacr.obj" : $(SOURCE) $(DEP_CPP_M2GMA) "$(INTDIR)"

"$(INTDIR)\m2gmacr.sbr" : $(SOURCE) $(DEP_CPP_M2GMA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2gdlg.cpp
DEP_CPP_M2GDL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	"..\base\dcport.h"\
	".\m2gframe.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilr.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcini.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\dcmacr.h"\
	{$(INCLUDE)}"\dcnode.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_M2GDL=\
	".\m2rhids.h"\
	

"$(INTDIR)\m2gdlg.obj" : $(SOURCE) $(DEP_CPP_M2GDL) "$(INTDIR)"

"$(INTDIR)\m2gdlg.sbr" : $(SOURCE) $(DEP_CPP_M2GDL) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
