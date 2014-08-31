# Microsoft Developer Studio Project File - Name="DITA2map" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=DITA2map - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DITA2map.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DITA2map.mak" CFG="DITA2map - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DITA2map - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "DITA2map - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DITA2map - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../exe/Win6Release"
# PROP Intermediate_Dir "../exe/Win6Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Shlwapi.lib /nologo /subsystem:console /incremental:yes /machine:I386
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\exe\Win6Release\DITA2map.exe
TargetName=DITA2map
InputPath=\cvs\exe\Win6Release\DITA2map.exe
SOURCE="$(InputPath)"

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ELSEIF  "$(CFG)" == "DITA2map - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../exe/Win6Debug"
# PROP Intermediate_Dir "../exe/Win6Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_WIN32" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Shlwapi.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Copying to NT directory...
TargetPath=\cvs\exe\Win6Debug\DITA2map.exe
TargetName=DITA2map
InputPath=\cvs\exe\Win6Debug\DITA2map.exe
SOURCE="$(InputPath)"

"c:\winnt\system32\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cmd /d /c copy $(TargetPath) c:\winnt\system32

# End Custom Build

!ENDIF 

# Begin Target

# Name "DITA2map - Win32 Release"
# Name "DITA2map - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\base\dcappl.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcbase.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcctlg.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcelem.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcfile.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcfili.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcfilp.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcfilr.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcfilw.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcglob.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcini.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dclist.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dclog.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcnode.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcnodt.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcport.cpp
# End Source File
# Begin Source File

SOURCE=.\DITA2map.cpp
# End Source File
# Begin Source File

SOURCE=..\base\drxmlfl.cpp
# End Source File
# Begin Source File

SOURCE=.\dtmapel.cpp
# End Source File
# Begin Source File

SOURCE=.\dtmapin.cpp
# End Source File
# Begin Source File

SOURCE=.\dtmapmp.cpp
# End Source File
# Begin Source File

SOURCE=.\dtmaptx.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dwhtmfl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\d2m.h
# End Source File
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

SOURCE=..\base\dcelem.h
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

SOURCE=..\base\dcnode.h
# End Source File
# Begin Source File

SOURCE=..\base\dcport.h
# End Source File
# Begin Source File

SOURCE=..\base\drxmlfl.h
# End Source File
# Begin Source File

SOURCE=..\base\dwhtmfl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\DITA2map.rc
# End Source File
# End Group
# End Target
# End Project
