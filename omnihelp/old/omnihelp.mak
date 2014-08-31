# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=omnihelp - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to omnihelp - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "omnihelp - Win32 Release" && "$(CFG)" !=\
 "omnihelp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "omnihelp.mak" CFG="omnihelp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "omnihelp - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "omnihelp - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "omnihelp - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "omnihelp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\omnihelp.exe"

CLEAN : 
	-@erase ".\Release\omnihelp.exe"
	-@erase ".\Release\ohbar.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/omnihelp.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ohbar.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/omnihelp.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/omnihelp.pdb" /machine:I386 /out:"$(OUTDIR)/omnihelp.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ohbar.res"

"$(OUTDIR)\omnihelp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\omnihelp.exe"

CLEAN : 
	-@erase ".\Debug\omnihelp.exe"
	-@erase ".\Debug\ohbar.res"
	-@erase ".\Debug\omnihelp.ilk"
	-@erase ".\Debug\omnihelp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/omnihelp.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ohbar.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/omnihelp.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/omnihelp.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/omnihelp.exe" 
LINK32_OBJS= \
	"$(INTDIR)/ohbar.res"

"$(OUTDIR)\omnihelp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "omnihelp - Win32 Release"
# Name "omnihelp - Win32 Debug"

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\omnihelp.txt

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ohcode.txt

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ohspec.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\third.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\second.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ohbar.rc

"$(INTDIR)\ohbar.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\main.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\local.css

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\index.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fsmacro.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\first.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ctrl.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ctrl.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ix.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gloss.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\omlogo.jpg

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\navix.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\navgloss.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\navctrl.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\navctrl.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\main.css

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ctrl.css

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\toc.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\toc.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\toc.css

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\serif.css

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\search.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\navsearch.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tocdata.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tocdata.htm

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\search.js

!IF  "$(CFG)" == "omnihelp - Win32 Release"

!ELSEIF  "$(CFG)" == "omnihelp - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
