# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=exwmf - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to exwmf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "exwmf - Win32 Release" && "$(CFG)" != "exwmf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "exwmf.mak" CFG="exwmf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "exwmf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "exwmf - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "exwmf - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "exwmf - Win32 Release"

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

ALL : "$(OUTDIR)\exwmf.exe" "$(OUTDIR)\exwmf.bsc"

CLEAN : 
	-@erase "..\exe\Release\exwmf.exe"
	-@erase "..\exe\Release\exwmf.res"
	-@erase "..\exe\Release\exwmf.obj"
	-@erase "..\exe\Release\exwmf.bsc"
	-@erase "..\exe\Release\exwmf.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp1 /W3 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /Zp1 /ML /W3 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../exe/Release/
CPP_SBRS=.\../exe/Release/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/exwmf.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/exwmf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/exwmf.sbr"

"$(OUTDIR)\exwmf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /incremental:yes
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/exwmf.pdb" /machine:I386 /out:"$(OUTDIR)/exwmf.exe" 
LINK32_OBJS= \
	"$(INTDIR)/exwmf.obj" \
	"$(INTDIR)/exwmf.res"

"$(OUTDIR)\exwmf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "exwmf - Win32 Debug"

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

ALL : "$(OUTDIR)\exwmf.exe" "$(OUTDIR)\exwmf.bsc"

CLEAN : 
	-@erase "..\exe\Debug\exwmf.exe"
	-@erase "..\exe\Debug\exwmf.res"
	-@erase "..\exe\Debug\exwmf.obj"
	-@erase "..\exe\Debug\exwmf.pdb"
	-@erase "..\exe\Debug\exwmf.bsc"
	-@erase "..\exe\Debug\exwmf.sbr"
	-@erase "..\exe\Debug\vc40.pdb"
	-@erase "..\exe\Debug\vc40.idb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp1 /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /Zp1 /MLd /W3 /Gm /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../exe/Debug/
CPP_SBRS=.\../exe/Debug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/exwmf.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/exwmf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/exwmf.sbr"

"$(OUTDIR)\exwmf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/exwmf.pdb" /debug /machine:I386 /out:"$(OUTDIR)/exwmf.exe" 
LINK32_OBJS= \
	"$(INTDIR)/exwmf.obj" \
	"$(INTDIR)/exwmf.res"

"$(OUTDIR)\exwmf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "exwmf - Win32 Release"
# Name "exwmf - Win32 Debug"

!IF  "$(CFG)" == "exwmf - Win32 Release"

!ELSEIF  "$(CFG)" == "exwmf - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\exwmf.rc

"$(INTDIR)\exwmf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\exwmf.cpp
DEP_CPP_EXWMF=\
	".\exwmf.h"\
	

"$(INTDIR)\exwmf.obj" : $(SOURCE) $(DEP_CPP_EXWMF) "$(INTDIR)"

"$(INTDIR)\exwmf.sbr" : $(SOURCE) $(DEP_CPP_EXWMF) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
