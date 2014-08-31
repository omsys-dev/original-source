# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=setini - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to setini - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "setini - Win32 Release" && "$(CFG)" != "setini - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "setini.mak" CFG="setini - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "setini - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "setini - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "setini - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "setini - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\exe\CRelease"
# PROP Intermediate_Dir "..\exe\CRelease"
OUTDIR=.\..\exe\CRelease
INTDIR=.\..\exe\CRelease
# Begin Custom Macros
TargetName=setini
# End Custom Macros

ALL : "$(OUTDIR)\setini.exe" "$(OUTDIR)\setini.bsc" "c:\bin\setini.exe"

CLEAN : 
	-@erase "..\exe\CRelease\setini.bsc"
	-@erase "..\exe\CRelease\setini.sbr"
	-@erase "c:\bin\setini.exe"
	-@erase "..\exe\CRelease\setini.exe"
	-@erase "..\exe\CRelease\setini.obj"
	-@erase "..\exe\CRelease\setini.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/setini.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\exe\CRelease/
CPP_SBRS=.\..\exe\CRelease/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/setini.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/setini.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/setini.sbr"

"$(OUTDIR)\setini.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/setini.pdb" /machine:I386 /out:"$(OUTDIR)/setini.exe" 
LINK32_OBJS= \
	"$(INTDIR)/setini.obj" \
	"$(INTDIR)/setini.res"

"$(OUTDIR)\setini.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to binary directory...
TargetPath=\cvs\exe\CRelease\setini.exe
TargetName=setini
InputPath=\cvs\exe\CRelease\setini.exe
SOURCE=$(InputPath)

"c:\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "setini - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\exe\CDebug"
# PROP Intermediate_Dir "..\exe\CDebug"
OUTDIR=.\..\exe\CDebug
INTDIR=.\..\exe\CDebug
# Begin Custom Macros
TargetName=setini
# End Custom Macros

ALL : "$(OUTDIR)\setini.exe" "c:\bin\setini.exe"

CLEAN : 
	-@erase "..\exe\CDebug\vc40.pdb"
	-@erase "..\exe\CDebug\vc40.idb"
	-@erase "..\exe\CDebug\setini.pdb"
	-@erase "..\exe\CDebug\setini.obj"
	-@erase "..\exe\CDebug\setini.res"
	-@erase "c:\bin\setini.exe"
	-@erase "..\exe\CDebug\setini.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/setini.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\..\exe\CDebug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/setini.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/setini.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/setini.pdb" /debug /machine:I386 /out:"$(OUTDIR)/setini.exe" 
LINK32_OBJS= \
	"$(INTDIR)/setini.obj" \
	"$(INTDIR)/setini.res"

"$(OUTDIR)\setini.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to binary directory...
TargetPath=\cvs\exe\CDebug\setini.exe
TargetName=setini
InputPath=\cvs\exe\CDebug\setini.exe
SOURCE=$(InputPath)

"c:\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\bin

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

# Name "setini - Win32 Release"
# Name "setini - Win32 Debug"

!IF  "$(CFG)" == "setini - Win32 Release"

!ELSEIF  "$(CFG)" == "setini - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\setini.c

!IF  "$(CFG)" == "setini - Win32 Release"


"$(INTDIR)\setini.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\setini.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "setini - Win32 Debug"


"$(INTDIR)\setini.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\setini.rc

"$(INTDIR)\setini.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
