# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=DCL - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to DCL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DCL - Win32 Release" && "$(CFG)" != "DCL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcl.mak" CFG="DCL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DCL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DCL - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "DCL - Win32 Release"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DCL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../exe/WinRel"
# PROP Intermediate_Dir "../exe/WinRel"
OUTDIR=.\../exe/WinRel
INTDIR=.\../exe/WinRel

ALL : "$(OUTDIR)\dcl.exe" "$(OUTDIR)\dcl.bsc"

CLEAN : 
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dcl.obj"
	-@erase "$(INTDIR)\dcl.res"
	-@erase "$(INTDIR)\dcl.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(OUTDIR)\dcl.bsc"
	-@erase "$(OUTDIR)\dcl.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_DCLDLL" /D "_WIN32" /D "VC4" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "_DCLDLL" /D "_WIN32" /D "VC4" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dcl.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../exe/WinRel/
CPP_SBRS=.\../exe/WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcl.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcl.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcport.sbr"

"$(OUTDIR)\dcl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcl.pdb" /machine:I386 /out:"$(OUTDIR)/dcl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcl.obj" \
	"$(INTDIR)\dcl.res" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcport.obj"

"$(OUTDIR)\dcl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DCL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../exe/WinDebug"
# PROP Intermediate_Dir "../exe/WinDebug"
OUTDIR=.\../exe/WinDebug
INTDIR=.\../exe/WinDebug

ALL : "$(OUTDIR)\dcl.exe" "$(OUTDIR)\dcl.bsc"

CLEAN : 
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dcl.obj"
	-@erase "$(INTDIR)\dcl.res"
	-@erase "$(INTDIR)\dcl.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcl.bsc"
	-@erase "$(OUTDIR)\dcl.exe"
	-@erase "$(OUTDIR)\dcl.ilk"
	-@erase "$(OUTDIR)\dcl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_DCLDLL" /D "_WIN32" /D "VC4" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_CONSOLE" /D "_DCLDLL" /D "_WIN32" /D "VC4" /FR"$(INTDIR)/"\
 /Fp"$(INTDIR)/dcl.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../exe/WinDebug/
CPP_SBRS=.\../exe/WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcl.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dcl.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcport.sbr"

"$(OUTDIR)\dcl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcl.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dcl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dcl.obj" \
	"$(INTDIR)\dcl.res" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcport.obj"

"$(OUTDIR)\dcl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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

# Name "DCL - Win32 Release"
# Name "DCL - Win32 Debug"

!IF  "$(CFG)" == "DCL - Win32 Release"

!ELSEIF  "$(CFG)" == "DCL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcl.rc

"$(INTDIR)\dcl.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcl.cpp
DEP_CPP_DCL_C=\
	".\dcl.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcl.obj" : $(SOURCE) $(DEP_CPP_DCL_C) "$(INTDIR)"

"$(INTDIR)\dcl.sbr" : $(SOURCE) $(DEP_CPP_DCL_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	{$(INCLUDE)}"\dcport.h"\
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

SOURCE=\cvs\base\dcmem.cpp
DEP_CPP_DCMEM=\
	".\..\base\dcmem.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
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

SOURCE=\cvs\base\dcglob.cpp
DEP_CPP_DCGLO=\
	".\..\base\dcmem.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

# End Source File
# End Target
# End Project
################################################################################
