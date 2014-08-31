# Microsoft Developer Studio Project File - Name="dwhtm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=dwhtm - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dwhtm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dwhtm.mak" CFG="dwhtm - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dwhtm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwhtm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "dwhtm - Win32 DbgRel" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dwhtm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\dll\WinRel"
# PROP Intermediate_Dir "..\dll\WinRel"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 icuuc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Shlwapi.lib /nologo /base:"0x67e00000" /subsystem:windows /dll /machine:I386
# SUBTRACT LINK32 /incremental:yes /nodefaultlib
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\WinRel\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinRel\dwhtm.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) %omsyshome%\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "dwhtm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\dll\WinDebug"
# PROP Intermediate_Dir "..\dll\WinDebug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /Gi /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 icuuc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Shlwapi.lib /nologo /base:"0x67e00000" /subsystem:windows /dll /map /debug /machine:I386
# SUBTRACT LINK32 /profile /nodefaultlib
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\WinDebug\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinDebug\dwhtm.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) %omsyshome%\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "dwhtm - Win32 DbgRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\dwhtm___"
# PROP BASE Intermediate_Dir ".\dwhtm___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\dll\WinDbRel"
# PROP Intermediate_Dir "..\dll\WinDbRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /Zi /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x67e00000" /subsystem:windows /dll /machine:I386
# ADD LINK32 icuuc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Shlwapi.lib /nologo /base:"0x67e00000" /subsystem:windows /dll /debug /machine:I386
# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\dll\WinDbRel\dwhtm.dll
TargetName=dwhtm
InputPath=\cvs\dll\WinDbRel\dwhtm.dll
SOURCE="$(InputPath)"

"%omsyshome%\common\bin\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) %omsyshome%\common\bin

# End Custom Build

!ENDIF 

# Begin Target

# Name "dwhtm - Win32 Release"
# Name "dwhtm - Win32 Debug"
# Name "dwhtm - Win32 DbgRel"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=\cvs\base\dcappl.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcauto.cpp
# End Source File
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

SOURCE=..\base\dcfmtr.cpp
# End Source File
# Begin Source File

SOURCE=\cvs\base\dcglob.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcicu.cpp
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

SOURCE=..\base\dcmxl.cpp
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

SOURCE=..\base\dctxfx.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtm.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtm.rc
# End Source File
# Begin Source File

SOURCE=.\dwhtman.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmcn.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmd0.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmd1.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmdb.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmdd.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmdm.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmds.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmdt.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dwhtmfl.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmfm.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmhr.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmin.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmpp.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmrf.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmsp.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmtb.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmtx.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmud.cpp
# End Source File
# Begin Source File

SOURCE=.\dwhtmxf.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\base\dcappl.h
# End Source File
# Begin Source File

SOURCE=..\base\dcauto.h
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

SOURCE=..\base\dcfmtr.h
# End Source File
# Begin Source File

SOURCE=..\base\dcglob.h
# End Source File
# Begin Source File

SOURCE=..\base\dcicu.h
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

SOURCE=..\base\dcmxl.h
# End Source File
# Begin Source File

SOURCE=..\base\dcnode.h
# End Source File
# Begin Source File

SOURCE=..\base\dcport.h
# End Source File
# Begin Source File

SOURCE=..\base\dctxfx.h
# End Source File
# Begin Source File

SOURCE=.\dwhtm.h
# End Source File
# Begin Source File

SOURCE=..\base\dwhtmfl.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\dropclose.gif
# End Source File
# Begin Source File

SOURCE=.\dropopen.gif
# End Source File
# End Group
# End Target
# End Project
