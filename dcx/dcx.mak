# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=dcx - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to dcx - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dcx - Win32 Release" && "$(CFG)" != "dcx - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcx.mak" CFG="dcx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dcx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dcx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "dcx - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "dcx - Win32 Release"

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
TargetName=dcx
# End Custom Macros

ALL : "$(OUTDIR)\dcx.dll" "$(OUTDIR)\dcx.bsc" "d:\Winnt3.51\System32\dcx"

CLEAN : 
	-@erase "..\dll\WinRel\dcx.bsc"
	-@erase "..\dll\WinRel\dcport.sbr"
	-@erase "..\dll\WinRel\dcmem.sbr"
	-@erase "..\dll\WinRel\dclist.sbr"
	-@erase "..\dll\WinRel\dcnode.sbr"
	-@erase "..\dll\WinRel\dcappl.sbr"
	-@erase "..\dll\WinRel\dcbase.sbr"
	-@erase "..\dll\WinRel\dcfile.sbr"
	-@erase "..\dll\WinRel\dcfilw.sbr"
	-@erase "..\dll\WinRel\dcglob.sbr"
	-@erase "..\dll\WinRel\dcfilr.sbr"
	-@erase "..\dll\WinRel\dcx.sbr"
	-@erase "..\dll\WinRel\dcnodt.sbr"
	-@erase "..\dll\WinRel\dcx.lib"
	-@erase "..\dll\WinRel\dcx.obj"
	-@erase "..\dll\WinRel\dcnodt.obj"
	-@erase "..\dll\WinRel\dcport.obj"
	-@erase "..\dll\WinRel\dcmem.obj"
	-@erase "..\dll\WinRel\dclist.obj"
	-@erase "..\dll\WinRel\dcnode.obj"
	-@erase "..\dll\WinRel\dcappl.obj"
	-@erase "..\dll\WinRel\dcbase.obj"
	-@erase "..\dll\WinRel\dcfile.obj"
	-@erase "..\dll\WinRel\dcfilw.obj"
	-@erase "..\dll\WinRel\dcglob.obj"
	-@erase "..\dll\WinRel\dcfilr.obj"
	-@erase "..\dll\WinRel\dcx.res"
	-@erase "..\dll\WinRel\dcx.exp"
	-@erase "d:\Winnt3.51\System32\dcx"
	-@erase "..\dll\WinRel\dcx.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DCLDLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_DCLDLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dcx.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinRel/
CPP_SBRS=.\../dll/WinRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcx.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcx.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/dcport.sbr" \
	"$(INTDIR)/dcmem.sbr" \
	"$(INTDIR)/dclist.sbr" \
	"$(INTDIR)/dcnode.sbr" \
	"$(INTDIR)/dcappl.sbr" \
	"$(INTDIR)/dcbase.sbr" \
	"$(INTDIR)/dcfile.sbr" \
	"$(INTDIR)/dcfilw.sbr" \
	"$(INTDIR)/dcglob.sbr" \
	"$(INTDIR)/dcfilr.sbr" \
	"$(INTDIR)/dcx.sbr" \
	"$(INTDIR)/dcnodt.sbr"

"$(OUTDIR)\dcx.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/dcx.pdb" /machine:I386 /out:"$(OUTDIR)/dcx.dll"\
 /implib:"$(OUTDIR)/dcx.lib" 
LINK32_OBJS= \
	"$(INTDIR)/dcx.obj" \
	"$(INTDIR)/dcnodt.obj" \
	"$(INTDIR)/dcport.obj" \
	"$(INTDIR)/dcmem.obj" \
	"$(INTDIR)/dclist.obj" \
	"$(INTDIR)/dcnode.obj" \
	"$(INTDIR)/dcappl.obj" \
	"$(INTDIR)/dcbase.obj" \
	"$(INTDIR)/dcfile.obj" \
	"$(INTDIR)/dcfilw.obj" \
	"$(INTDIR)/dcglob.obj" \
	"$(INTDIR)/dcfilr.obj" \
	"$(INTDIR)/dcx.res"

"$(OUTDIR)\dcx.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinRel\dcx.dll
TargetName=dcx
InputPath=\cvs\dll\WinRel\dcx.dll
SOURCE=$(InputPath)

"d:\Winnt3.51\System32\$(TargetName)" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) d:\Winnt3.51\System32

# End Custom Build

!ELSEIF  "$(CFG)" == "dcx - Win32 Debug"

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

ALL : "$(OUTDIR)\dcx.dll" "$(OUTDIR)\dcx.bsc"

CLEAN : 
	-@erase "..\dll\WinDebug\vc40.pdb"
	-@erase "..\dll\WinDebug\vc40.idb"
	-@erase "..\dll\WinDebug\dcx.bsc"
	-@erase "..\dll\WinDebug\dcappl.sbr"
	-@erase "..\dll\WinDebug\dcnodt.sbr"
	-@erase "..\dll\WinDebug\dcport.sbr"
	-@erase "..\dll\WinDebug\dcnode.sbr"
	-@erase "..\dll\WinDebug\dcmem.sbr"
	-@erase "..\dll\WinDebug\dcbase.sbr"
	-@erase "..\dll\WinDebug\dcfile.sbr"
	-@erase "..\dll\WinDebug\dcfilw.sbr"
	-@erase "..\dll\WinDebug\dcx.sbr"
	-@erase "..\dll\WinDebug\dclist.sbr"
	-@erase "..\dll\WinDebug\dcglob.sbr"
	-@erase "..\dll\WinDebug\dcfilr.sbr"
	-@erase "..\dll\WinDebug\dcx.dll"
	-@erase "..\dll\WinDebug\dcglob.obj"
	-@erase "..\dll\WinDebug\dcfilr.obj"
	-@erase "..\dll\WinDebug\dcappl.obj"
	-@erase "..\dll\WinDebug\dcnodt.obj"
	-@erase "..\dll\WinDebug\dcport.obj"
	-@erase "..\dll\WinDebug\dcnode.obj"
	-@erase "..\dll\WinDebug\dcmem.obj"
	-@erase "..\dll\WinDebug\dcbase.obj"
	-@erase "..\dll\WinDebug\dcfile.obj"
	-@erase "..\dll\WinDebug\dcfilw.obj"
	-@erase "..\dll\WinDebug\dcx.obj"
	-@erase "..\dll\WinDebug\dclist.obj"
	-@erase "..\dll\WinDebug\dcx.res"
	-@erase "..\dll\WinDebug\dcx.ilk"
	-@erase "..\dll\WinDebug\dcx.lib"
	-@erase "..\dll\WinDebug\dcx.exp"
	-@erase "..\dll\WinDebug\dcx.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_DCLDLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_DCLDLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dcx.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDebug/
CPP_SBRS=.\../dll/WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcx.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcx.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/dcappl.sbr" \
	"$(INTDIR)/dcnodt.sbr" \
	"$(INTDIR)/dcport.sbr" \
	"$(INTDIR)/dcnode.sbr" \
	"$(INTDIR)/dcmem.sbr" \
	"$(INTDIR)/dcbase.sbr" \
	"$(INTDIR)/dcfile.sbr" \
	"$(INTDIR)/dcfilw.sbr" \
	"$(INTDIR)/dcx.sbr" \
	"$(INTDIR)/dclist.sbr" \
	"$(INTDIR)/dcglob.sbr" \
	"$(INTDIR)/dcfilr.sbr"

"$(OUTDIR)\dcx.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/dcx.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dcx.dll"\
 /implib:"$(OUTDIR)/dcx.lib" 
LINK32_OBJS= \
	"$(INTDIR)/dcglob.obj" \
	"$(INTDIR)/dcfilr.obj" \
	"$(INTDIR)/dcappl.obj" \
	"$(INTDIR)/dcnodt.obj" \
	"$(INTDIR)/dcport.obj" \
	"$(INTDIR)/dcnode.obj" \
	"$(INTDIR)/dcmem.obj" \
	"$(INTDIR)/dcbase.obj" \
	"$(INTDIR)/dcfile.obj" \
	"$(INTDIR)/dcfilw.obj" \
	"$(INTDIR)/dcx.obj" \
	"$(INTDIR)/dclist.obj" \
	"$(INTDIR)/dcx.res"

"$(OUTDIR)\dcx.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "dcx - Win32 Release"
# Name "dcx - Win32 Debug"

!IF  "$(CFG)" == "dcx - Win32 Release"

!ELSEIF  "$(CFG)" == "dcx - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcx.rc

"$(INTDIR)\dcx.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcx.cpp
DEP_CPP_DCX_C=\
	{$(INCLUDE)}"\dcappl.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

"$(INTDIR)\dcx.obj" : $(SOURCE) $(DEP_CPP_DCX_C) "$(INTDIR)"

"$(INTDIR)\dcx.sbr" : $(SOURCE) $(DEP_CPP_DCX_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
DEP_CPP_DCPOR=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcport.obj" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcport.sbr" : $(SOURCE) $(DEP_CPP_DCPOR) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnodt.cpp
DEP_CPP_DCNOD=\
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnodt.obj" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnodt.sbr" : $(SOURCE) $(DEP_CPP_DCNOD) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcnode.cpp
DEP_CPP_DCNODE=\
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\dcnode.obj" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dcnode.sbr" : $(SOURCE) $(DEP_CPP_DCNODE) "$(INTDIR)"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=\cvs\base\dcmem.cpp
DEP_CPP_DCMEM=\
	".\..\base\dcglob.h"\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcfile.h"\
	".\..\base\dclist.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dclist.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	".\..\base\dcglob.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
	{$(INCLUDE)}"\dcappl.h"\
	".\..\base\dcglob.h"\
	".\..\base\dcbase.h"\
	".\..\base\dclist.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcfile.h"\
	".\..\base\dcfilr.h"\
	".\..\base\dcfilw.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\sys\STAT.H"\
	

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
