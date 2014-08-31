# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=DWINF - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to DWINF - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DWINF - Win32 Release" && "$(CFG)" != "DWINF - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dwinf.mak" CFG="DWINF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DWINF - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DWINF - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "DWINF - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "DWINF - Win32 Release"

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

ALL : "$(OUTDIR)\dwinf.dll" "$(OUTDIR)\dwinf.bsc" "c:\WinNT\System32\dwinf.dll"

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
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\dwinf.obj"
	-@erase "$(INTDIR)\dwinf.res"
	-@erase "$(INTDIR)\dwinf.sbr"
	-@erase "$(INTDIR)\dwinffl.obj"
	-@erase "$(INTDIR)\dwinffl.sbr"
	-@erase "$(OUTDIR)\dwinf.bsc"
	-@erase "$(OUTDIR)\dwinf.dll"
	-@erase "$(OUTDIR)\dwinf.exp"
	-@erase "$(OUTDIR)\dwinf.lib"
	-@erase "c:\WinNT\System32\dwinf.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwinf.pch" /YX /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\../dll/WinRel/
CPP_SBRS=.\../dll/WinRel/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwinf.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwinf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\dwinf.sbr" \
	"$(INTDIR)\dwinffl.sbr"

"$(OUTDIR)\dwinf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/dwinf.pdb" /machine:I386 /out:"$(OUTDIR)/dwinf.dll"\
 /implib:"$(OUTDIR)/dwinf.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\dwinf.obj" \
	"$(INTDIR)\dwinf.res" \
	"$(INTDIR)\dwinffl.obj"

"$(OUTDIR)\dwinf.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to system32...
TargetPath=\cvs\dll\WinRel\dwinf.dll
InputPath=\cvs\dll\WinRel\dwinf.dll
SOURCE=$(InputPath)

"c:\WinNT\System32\dwinf.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\WinNT\System32\dwinf.dll

# End Custom Build

!ELSEIF  "$(CFG)" == "DWINF - Win32 Debug"

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

ALL : "$(OUTDIR)\dwinf.dll" "$(OUTDIR)\dwinf.bsc" "c:\WinNT\System32\dwinf.dll"

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
	-@erase "$(INTDIR)\dclist.obj"
	-@erase "$(INTDIR)\dclist.sbr"
	-@erase "$(INTDIR)\dcmem.obj"
	-@erase "$(INTDIR)\dcmem.sbr"
	-@erase "$(INTDIR)\dcnode.obj"
	-@erase "$(INTDIR)\dcnode.sbr"
	-@erase "$(INTDIR)\dcnodt.obj"
	-@erase "$(INTDIR)\dcnodt.sbr"
	-@erase "$(INTDIR)\dcport.obj"
	-@erase "$(INTDIR)\dcport.sbr"
	-@erase "$(INTDIR)\dwinf.obj"
	-@erase "$(INTDIR)\dwinf.res"
	-@erase "$(INTDIR)\dwinf.sbr"
	-@erase "$(INTDIR)\dwinffl.obj"
	-@erase "$(INTDIR)\dwinffl.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dwinf.bsc"
	-@erase "$(OUTDIR)\dwinf.dll"
	-@erase "$(OUTDIR)\dwinf.exp"
	-@erase "$(OUTDIR)\dwinf.lib"
	-@erase "$(OUTDIR)\dwinf.pdb"
	-@erase "c:\WinNT\System32\dwinf.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
CPP_PROJ=/nologo /Zp1 /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WIN32" /D "_DLL" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dwinf.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../dll/WinDebug/
CPP_SBRS=.\../dll/WinDebug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dwinf.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dwinf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\dcappl.sbr" \
	"$(INTDIR)\dcbase.sbr" \
	"$(INTDIR)\dcctlg.sbr" \
	"$(INTDIR)\dcfile.sbr" \
	"$(INTDIR)\dcfili.sbr" \
	"$(INTDIR)\dcfilr.sbr" \
	"$(INTDIR)\dcfilw.sbr" \
	"$(INTDIR)\dcglob.sbr" \
	"$(INTDIR)\dclist.sbr" \
	"$(INTDIR)\dcmem.sbr" \
	"$(INTDIR)\dcnode.sbr" \
	"$(INTDIR)\dcnodt.sbr" \
	"$(INTDIR)\dcport.sbr" \
	"$(INTDIR)\dwinf.sbr" \
	"$(INTDIR)\dwinffl.sbr"

"$(OUTDIR)\dwinf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/dwinf.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dwinf.dll"\
 /implib:"$(OUTDIR)/dwinf.lib" 
LINK32_OBJS= \
	"$(INTDIR)\dcappl.obj" \
	"$(INTDIR)\dcbase.obj" \
	"$(INTDIR)\dcctlg.obj" \
	"$(INTDIR)\dcfile.obj" \
	"$(INTDIR)\dcfili.obj" \
	"$(INTDIR)\dcfilr.obj" \
	"$(INTDIR)\dcfilw.obj" \
	"$(INTDIR)\dcglob.obj" \
	"$(INTDIR)\dclist.obj" \
	"$(INTDIR)\dcmem.obj" \
	"$(INTDIR)\dcnode.obj" \
	"$(INTDIR)\dcnodt.obj" \
	"$(INTDIR)\dcport.obj" \
	"$(INTDIR)\dwinf.obj" \
	"$(INTDIR)\dwinf.res" \
	"$(INTDIR)\dwinffl.obj"

"$(OUTDIR)\dwinf.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to system32...
TargetPath=\cvs\dll\WinDebug\dwinf.dll
InputPath=\cvs\dll\WinDebug\dwinf.dll
SOURCE=$(InputPath)

"c:\WinNT\System32\dwinf.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) c:\WinNT\System32\dwinf.dll

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

# Name "DWINF - Win32 Release"
# Name "DWINF - Win32 Debug"

!IF  "$(CFG)" == "DWINF - Win32 Release"

!ELSEIF  "$(CFG)" == "DWINF - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dwinf.rc

"$(INTDIR)\dwinf.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwinffl.cpp
DEP_CPP_DWINF=\
	".\..\base\dcport.h"\
	".\dwinffl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwinffl.obj" : $(SOURCE) $(DEP_CPP_DWINF) "$(INTDIR)"

"$(INTDIR)\dwinffl.sbr" : $(SOURCE) $(DEP_CPP_DWINF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dwinf.cpp

!IF  "$(CFG)" == "DWINF - Win32 Release"

DEP_CPP_DWINF_=\
	".\..\base\dcfilr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwinf.h"\
	".\dwinffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	
NODEP_CPP_DWINF_=\
	".\AddToList"\
	".\StyleList"\
	

"$(INTDIR)\dwinf.obj" : $(SOURCE) $(DEP_CPP_DWINF_) "$(INTDIR)"

"$(INTDIR)\dwinf.sbr" : $(SOURCE) $(DEP_CPP_DWINF_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "DWINF - Win32 Debug"

DEP_CPP_DWINF_=\
	".\..\base\dcfilr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	".\dwinf.h"\
	".\dwinffl.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dwinf.obj" : $(SOURCE) $(DEP_CPP_DWINF_) "$(INTDIR)"

"$(INTDIR)\dwinf.sbr" : $(SOURCE) $(DEP_CPP_DWINF_) "$(INTDIR)"


!ENDIF 

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

SOURCE=\cvs\base\dcnodt.cpp
DEP_CPP_DCNOD=\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

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
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

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
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
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

SOURCE=\cvs\base\dclist.cpp
DEP_CPP_DCLIS=\
	".\..\base\dcport.h"\
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

SOURCE=\cvs\base\dcglob.cpp
DEP_CPP_DCGLO=\
	".\..\base\dcmem.h"\
	".\..\base\dcport.h"\
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

SOURCE=\cvs\base\dcfilw.cpp
DEP_CPP_DCFIL=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
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
	".\..\base\dcfilr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
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

SOURCE=\cvs\base\dcfili.cpp
DEP_CPP_DCFILI=\
	".\..\base\dcfilr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfili.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
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

SOURCE=\cvs\base\dcfile.cpp
DEP_CPP_DCFILE=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcglob.h"\
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

SOURCE=\cvs\base\dcctlg.cpp
DEP_CPP_DCCTL=\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcctlg.h"\
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

SOURCE=\cvs\base\dcbase.cpp
DEP_CPP_DCBAS=\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcglob.h"\
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
	".\..\base\dcfilr.h"\
	".\..\base\dcnode.h"\
	".\..\base\dcport.h"\
	{$(INCLUDE)}"\dcappl.h"\
	{$(INCLUDE)}"\dcbase.h"\
	{$(INCLUDE)}"\dcfile.h"\
	{$(INCLUDE)}"\dcfilw.h"\
	{$(INCLUDE)}"\dcglob.h"\
	{$(INCLUDE)}"\dclist.h"\
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
