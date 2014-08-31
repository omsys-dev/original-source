# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=pprtf - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to pprtf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "pprtf - Win32 Release" && "$(CFG)" != "pprtf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "pprtf.mak" CFG="pprtf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pprtf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pprtf - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "pprtf - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pprtf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\exe\Crelease"
# PROP Intermediate_Dir "..\exe\Crelease"
# PROP Target_Dir ""
OUTDIR=.\..\exe\Crelease
INTDIR=.\..\exe\Crelease
# Begin Custom Macros
TargetName=pprtf
# End Custom Macros

ALL : "$(OUTDIR)\pprtf.exe" "$(OUTDIR)\pprtf.bsc" "d:\Winnt\System32\pprtf"

CLEAN : 
	-@erase "$(INTDIR)\pprtf.obj"
	-@erase "$(INTDIR)\pprtf.res"
	-@erase "$(INTDIR)\pprtf.sbr"
	-@erase "$(OUTDIR)\pprtf.bsc"
	-@erase "$(OUTDIR)\pprtf.exe"
	-@erase "d:\Winnt\System32\pprtf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/pprtf.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\exe\Crelease/
CPP_SBRS=.\..\exe\Crelease/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/pprtf.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pprtf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\pprtf.sbr"

"$(OUTDIR)\pprtf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/pprtf.pdb" /machine:I386 /out:"$(OUTDIR)/pprtf.exe" 
LINK32_OBJS= \
	"$(INTDIR)\pprtf.obj" \
	"$(INTDIR)\pprtf.res"

"$(OUTDIR)\pprtf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build
TargetPath=\cvs\exe\Crelease\pprtf.exe
TargetName=pprtf
InputPath=\cvs\exe\Crelease\pprtf.exe
SOURCE=$(InputPath)

"d:\Winnt\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) d:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "pprtf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\exe\CDebug"
# PROP Intermediate_Dir "..\exe\CDebug"
# PROP Target_Dir ""
OUTDIR=.\..\exe\CDebug
INTDIR=.\..\exe\CDebug
# Begin Custom Macros
TargetName=pprtf
# End Custom Macros

ALL : "$(OUTDIR)\pprtf.exe" "$(OUTDIR)\pprtf.bsc" "d:\Winnt\System32\pprtf"

CLEAN : 
	-@erase "$(INTDIR)\pprtf.obj"
	-@erase "$(INTDIR)\pprtf.res"
	-@erase "$(INTDIR)\pprtf.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\pprtf.bsc"
	-@erase "$(OUTDIR)\pprtf.exe"
	-@erase "$(OUTDIR)\pprtf.ilk"
	-@erase "$(OUTDIR)\pprtf.pdb"
	-@erase "d:\Winnt\System32\pprtf"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /FR"$(INTDIR)/" /Fp"$(INTDIR)/pprtf.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 
CPP_OBJS=.\..\exe\CDebug/
CPP_SBRS=.\..\exe\CDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/pprtf.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pprtf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\pprtf.sbr"

"$(OUTDIR)\pprtf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/pprtf.pdb" /debug /machine:I386 /out:"$(OUTDIR)/pprtf.exe" 
LINK32_OBJS= \
	"$(INTDIR)\pprtf.obj" \
	"$(INTDIR)\pprtf.res"

"$(OUTDIR)\pprtf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to Windows dir
TargetPath=\cvs\exe\CDebug\pprtf.exe
TargetName=pprtf
InputPath=\cvs\exe\CDebug\pprtf.exe
SOURCE=$(InputPath)

"d:\Winnt\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
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

# Name "pprtf - Win32 Release"
# Name "pprtf - Win32 Debug"

!IF  "$(CFG)" == "pprtf - Win32 Release"

!ELSEIF  "$(CFG)" == "pprtf - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\pprtf.c

"$(INTDIR)\pprtf.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\pprtf.sbr" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\pprtf.rc

"$(INTDIR)\pprtf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
