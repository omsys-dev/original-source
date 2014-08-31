# Microsoft Developer Studio Project File - Name="m2gframe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=m2gframe - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "m2gframe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "m2gframe.mak" CFG="m2gframe - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "m2gframe - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2gframe - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2gframe - Win32 DbgRel" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "m2gframe - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\fdk\Release"
# PROP Intermediate_Dir "..\fdk\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "M2GFRAME" /FR /YX"m2gframe.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib shlwapi.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
# Begin Custom Build - Copying to FrameMaker directories...
TargetPath=\cvs\fdk\Release\m2gframe.dll
TargetName=m2gframe
InputPath=\cvs\fdk\Release\m2gframe.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy $(TargetPath) C:\FM8\fminit\plugins \
	del /q C:\FM8\fminit\plugins\.cache \
	copy $(TargetPath) C:\FM9\fminit\plugins \
	del /q C:\FM9\fminit\plugins\.cache \
	

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"C:\FM9\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "m2gframe - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\fdk\Debug"
# PROP Intermediate_Dir "..\fdk\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2GFRAME" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib shlwapi.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
# Begin Custom Build - Copying to FrameMaker directories...
TargetPath=\cvs\fdk\Debug\m2gframe.dll
TargetName=m2gframe
InputPath=\cvs\fdk\Debug\m2gframe.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy $(TargetPath) C:\FM8\fminit\plugins \
	del /q C:\FM8\fminit\plugins\.cache \
	copy $(TargetPath) C:\FM9\fminit\plugins \
	del /q C:\FM9\fminit\plugins\.cache \
	

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"C:\FM9\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "m2gframe - Win32 DbgRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "m2gframe___Win32_DbgRel"
# PROP BASE Intermediate_Dir "m2gframe___Win32_DbgRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "m2gframe___Win32_DbgRel"
# PROP Intermediate_Dir "m2gframe___Win32_DbgRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "M2GFRAME" /FR /YX"m2gframe.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "M2GFRAME" /FR /YX"m2gframe.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib shlwapi.lib /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"
# Begin Custom Build - Copying to FrameMaker directories...
TargetPath=.\m2gframe___Win32_DbgRel\m2gframe.dll
TargetName=m2gframe
InputPath=.\m2gframe___Win32_DbgRel\m2gframe.dll
SOURCE="$(InputPath)"

BuildCmds= \
	copy $(TargetPath) C:\FM8\fminit\plugins \
	del /q C:\FM8\fminit\plugins\.cache \
	copy $(TargetPath) C:\FM9\fminit\plugins \
	del /q C:\FM9\fminit\plugins\.cache \
	

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"C:\FM9\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# Begin Target

# Name "m2gframe - Win32 Release"
# Name "m2gframe - Win32 Debug"
# Name "m2gframe - Win32 DbgRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=\cvs\base\dcbase.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcctlg.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcfile.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcfili.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcfilp.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcfilr.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcfilw.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcglob.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcini.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dclist.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dclog.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcmacr.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcmem.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcnode.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcnodt.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcport.cpp
# End Source File
# Begin Source File

SOURCE=.\m2gdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\m2gframe.cpp
# End Source File
# Begin Source File

SOURCE=.\m2gframe.rc
# End Source File
# Begin Source File

SOURCE=.\m2gini.cpp
# End Source File
# Begin Source File

SOURCE=.\m2gmacr.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\base\dcappl.h
# End Source File
# Begin Source File

SOURCE=..\base\dcbase.h
# End Source File
# Begin Source File

SOURCE=..\base\dcctlg.h
# End Source File
# Begin Source File

SOURCE=..\base\dcfile.h
# End Source File
# Begin Source File

SOURCE=..\base\dcfili.h
# End Source File
# Begin Source File

SOURCE=..\base\dcfilp.h
# End Source File
# Begin Source File

SOURCE=..\base\dcfilr.h
# End Source File
# Begin Source File

SOURCE=..\base\dcfilw.h
# End Source File
# Begin Source File

SOURCE=..\base\dcglob.h
# End Source File
# Begin Source File

SOURCE=..\base\dcini.h
# End Source File
# Begin Source File

SOURCE=..\base\dclist.h
# End Source File
# Begin Source File

SOURCE=..\base\dclog.h
# End Source File
# Begin Source File

SOURCE=..\base\dcmacr.h
# End Source File
# Begin Source File

SOURCE=..\base\dcmem.h
# End Source File
# Begin Source File

SOURCE=..\base\dcnode.h
# End Source File
# Begin Source File

SOURCE=..\base\dcport.h
# End Source File
# Begin Source File

SOURCE=.\m2gframe.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
