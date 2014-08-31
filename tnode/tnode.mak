# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=tnode - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to tnode - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tnode - Win32 Release" && "$(CFG)" != "tnode - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "tnode.mak" CFG="tnode - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tnode - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tnode - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "tnode - Win32 Debug"
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "tnode - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../util/Release"
# PROP Intermediate_Dir "../util/Release"
# PROP Target_Dir ""
OUTDIR=.\../util/Release
INTDIR=.\../util/Release

ALL : "$(OUTDIR)\tnode.exe" "$(OUTDIR)\tnode.bsc"

CLEAN : 
	-@erase "$(INTDIR)\dcappl.obj"
	-@erase "$(INTDIR)\dcappl.sbr"
	-@erase "$(INTDIR)\dcbase.obj"
	-@erase "$(INTDIR)\dcbase.sbr"
	-@erase "$(INTDIR)\dcfile.obj"
	-@erase "$(INTDIR)\dcfile.sbr"
	-@erase "$(INTDIR)\dcfilr.obj"
	-@erase "$(INTDIR)\dcfilr.sbr"
	-@erase "$(INTDIR)\dcfilw.obj"
	-@erase "$(INTDIR)\dcfilw.sbr"
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\tnode.obj"
	-@erase "$(INTDIR)\tnode.sbr"
	-@erase "$(OUTDIR)\tnode.bsc"
	-@erase "$(OUTDIR)\tnode.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /D "_WIN32" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D\
 "_WINDOWS" /D "_WIN32" /FR"$(INTDIR)/" /Fp"$(INTDIR)/tnode.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../util/Release/
CPP_SBRS=.\../util/Release/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tnode.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\tnode.sbr"

"$(OUTDIR)\tnode.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:console /machine:I386
# SUBTRACT LINK32 /incremental:yes /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/tnode.pdb" /machine:I386 /out:"$(OUTDIR)/tnode.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\tnode.obj"

"$(OUTDIR)\tnode.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tnode - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../util/Debug"
# PROP Intermediate_Dir "../util/Debug"
# PROP Target_Dir ""
OUTDIR=.\../util/Debug
INTDIR=.\../util/Debug

ALL : "$(OUTDIR)\tnode.exe" "$(OUTDIR)\tnode.bsc"

CLEAN : 
	-@erase "$(INTDIR)\dcappl.obj"
	-@erase "$(INTDIR)\dcappl.sbr"
	-@erase "$(INTDIR)\dcbase.obj"
	-@erase "$(INTDIR)\dcbase.sbr"
	-@erase "$(INTDIR)\dcfile.obj"
	-@erase "$(INTDIR)\dcfile.sbr"
	-@erase "$(INTDIR)\dcfilr.obj"
	-@erase "$(INTDIR)\dcfilr.sbr"
	-@erase "$(INTDIR)\dcfilw.obj"
	-@erase "$(INTDIR)\dcfilw.sbr"
	-@erase "$(INTDIR)\dcglob.obj"
	-@erase "$(INTDIR)\dcglob.sbr"
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\tnode.obj"
	-@erase "$(INTDIR)\tnode.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\tnode.bsc"
	-@erase "$(OUTDIR)\tnode.exe"
	-@erase "$(OUTDIR)\tnode.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_WINDOWS" /D "_WIN32" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_CONSOLE" /D "_WINDOWS" /D "_WIN32" /FR"$(INTDIR)/" /Fp"$(INTDIR)/tnode.pch"\
 /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../util/Debug/
CPP_SBRS=.\../util/Debug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tnode.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\tnode.sbr"

"$(OUTDIR)\tnode.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:console /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /version:1.0 /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/tnode.pdb" /debug /machine:I386 /out:"$(OUTDIR)/tnode.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\tnode.obj"

"$(OUTDIR)\tnode.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "tnode - Win32 Release"
# Name "tnode - Win32 Debug"

!IF  "$(CFG)" == "tnode - Win32 Release"

!ELSEIF  "$(CFG)" == "tnode - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tnode.cpp
DEP_CPP_TNODE=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\tnode.obj" : $(SOURCE) $(DEP_CPP_TNODE) "$(INTDIR)"

"$(INTDIR)\tnode.sbr" : $(SOURCE) $(DEP_CPP_TNODE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	".\..\base\dcport.h"\
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

SOURCE=\cvs\base\dcnode.cpp

!IF  "$(CFG)" == "tnode - Win32 Release"

DEP_CPP_DCNOD=\
	".\..\base\dcbase.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "tnode - Win32 Debug"

DEP_CPP_DCNOD=\
	".\..\base\dcbase.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DCNOD=\
	"..\base\SetNodeName"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcmem.obj" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcmem.sbr" : $(SOURCE) $(DEP_CPP_DCMEM) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dclist.obj" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dclist.sbr" : $(SOURCE) $(DEP_CPP_DCLIS) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcglob.obj" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcglob.sbr" : $(SOURCE) $(DEP_CPP_DCGLO) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilw.obj" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilw.sbr" : $(SOURCE) $(DEP_CPP_DCFIL) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfilr.obj" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfilr.sbr" : $(SOURCE) $(DEP_CPP_DCFILR) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcfile.obj" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcfile.sbr" : $(SOURCE) $(DEP_CPP_DCFILE) "$(INTDIR)"
   $(BuildCmds)

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
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcbase.obj" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcbase.sbr" : $(SOURCE) $(DEP_CPP_DCBAS) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcappl.cpp
DEP_CPP_DCAPP=\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcglob.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcappl.obj" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcappl.sbr" : $(SOURCE) $(DEP_CPP_DCAPP) "$(INTDIR)"
   $(BuildCmds)

# End Source File
# End Target
# End Project
################################################################################
