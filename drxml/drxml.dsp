# Microsoft Developer Studio Project File - Name="drxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=drxml - Win32 drmxl Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "drxml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "drxml.mak" CFG="drxml - Win32 drmxl Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "drxml - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 Sample" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 SampleDbg" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 drmxl Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 drmxl Sample" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "drxml - Win32 drmxl Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "drxml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\dll\WinRel"
# PROP Intermediate_Dir "..\dll\WinRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DRXML_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "MXLPARSER_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\WinRel\drxml.dll
TargetName=drxml
InputPath=\cvs\dll\WinRel\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\dll\WinDebug"
# PROP Intermediate_Dir "..\dll\WinDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DRXML_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "MXLPARSER_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /map /debug /machine:I386 /libpath:"D:\cvs\expat\dll\WinDebug\\"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\WinDebug\drxml.dll
TargetName=drxml
InputPath=\cvs\dll\WinDebug\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 Sample"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drxml___Win32_Sample"
# PROP BASE Intermediate_Dir "drxml___Win32_Sample"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\dll\Sample"
# PROP Intermediate_Dir "..\dll\Sample"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "SAMPLE" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\Sample\drxml.dll
InputPath=\cvs\dll\Sample\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\drxmldemo.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\drxmldemo.dll

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 SampleDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drxml___Win32_SampleDbg"
# PROP BASE Intermediate_Dir "drxml___Win32_SampleDbg"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\dll\SampleDbg"
# PROP Intermediate_Dir "..\dll\SampleDbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "SAMPLE" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /map /debug /machine:I386 /libpath:"D:\cvs\expat\dll\WinDebug\\"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /map /debug /machine:I386 /libpath:"D:\cvs\expat\dll\WinDebug\\"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=\cvs\dll\SampleDbg\drxml.dll
InputPath=\cvs\dll\SampleDbg\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\drxmldemo.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\drxmldemo.dll

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 drmxl Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drxml___Win32_drmxl_Release"
# PROP BASE Intermediate_Dir "drxml___Win32_drmxl_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "drmxl_Release"
# PROP Intermediate_Dir "drmxl_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "MXLPARSER_EXPORTS" /D "DITA" /D "UDOC" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "MXLPARSER_EXPORTS" /D "UDOC" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=.\drmxl_Release\drxml.dll
InputPath=.\drmxl_Release\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\drmxl.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\drmxl.dll

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 drmxl Sample"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "drxml___Win32_drmxl_Sample"
# PROP BASE Intermediate_Dir "drxml___Win32_drmxl_Sample"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "drmxl_Sample"
# PROP Intermediate_Dir "drmxl_Sample"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "SAMPLE" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /D "UDOC" /FR /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /D "NDEBUG" /D "MXLPARSER_EXPORTS" /D "SAMPLE" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "UDOC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /base:"0x67e00000" /dll /machine:I386
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=.\drmxl_Sample\drxml.dll
InputPath=.\drmxl_Sample\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\drmxldemo.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\drmxldemo.dll

# End Custom Build

!ELSEIF  "$(CFG)" == "drxml - Win32 drmxl Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "drxml___Win32_drmxl_Debug"
# PROP BASE Intermediate_Dir "drxml___Win32_drmxl_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "drmxl_Debug"
# PROP Intermediate_Dir "drmxl_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "MXLPARSER_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "DITA" /D "UDOC" /FR /YX /FD /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "MXLPARSER_EXPORTS" /D "WIN32" /D "_WINDOWS" /D "_WIN32" /D "_DLL" /D "UDOC" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /map /debug /machine:I386 /libpath:"D:\cvs\expat\dll\WinDebug\\"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 libexpat.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /map /debug /machine:I386 /libpath:"D:\cvs\expat\dll\WinDebug\\"
# SUBTRACT LINK32 /profile
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=.\drmxl_Debug\drxml.dll
InputPath=.\drmxl_Debug\drxml.dll
SOURCE="$(InputPath)"

"C:\omsys\common\bin\drmxl.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\drmxl.dll

# End Custom Build

!ENDIF 

# Begin Target

# Name "drxml - Win32 Release"
# Name "drxml - Win32 Debug"
# Name "drxml - Win32 Sample"
# Name "drxml - Win32 SampleDbg"
# Name "drxml - Win32 drmxl Release"
# Name "drxml - Win32 drmxl Sample"
# Name "drxml - Win32 drmxl Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\base\dcappl.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcauto.cpp
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

SOURCE=..\base\dcfmtr.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcglob.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcindx.cpp
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

SOURCE=..\base\dcmacr.cpp
# End Source File
# Begin Source File

SOURCE=..\base\dcmem.cpp
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

SOURCE=..\base\drxdvfl.cpp
# End Source File
# Begin Source File

SOURCE=.\drxml.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlan.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlcn.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmldt.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmldv.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlel.cpp
# End Source File
# Begin Source File

SOURCE=..\base\drxmlfl.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlgn.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlgr.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlin.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlmp.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlpi.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlrl.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlsm.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmltb.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmltx.cpp
# End Source File
# Begin Source File

SOURCE=.\drxmlud.cpp
# End Source File
# Begin Source File

SOURCE=..\mxlparser\mxlparser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=..\base\dcfmtr.h
# End Source File
# Begin Source File

SOURCE=..\base\dcglob.h
# End Source File
# Begin Source File

SOURCE=..\base\dcindx.h
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

SOURCE=..\base\dcnode.h
# End Source File
# Begin Source File

SOURCE=..\base\dcport.h
# End Source File
# Begin Source File

SOURCE=..\base\dctxfx.h
# End Source File
# Begin Source File

SOURCE=..\base\drxdvfl.h
# End Source File
# Begin Source File

SOURCE=.\drxml.h
# End Source File
# Begin Source File

SOURCE=..\base\drxmlfl.h
# End Source File
# Begin Source File

SOURCE=.\drxmlud.h
# End Source File
# Begin Source File

SOURCE=..\mxlparser\mxlparser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\drxml.rc
# End Source File
# End Group
# End Target
# End Project
