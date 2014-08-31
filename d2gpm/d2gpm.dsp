# Microsoft Developer Studio Project File - Name="d2gpm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=d2gpm - Win32 x2gpmDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "d2gpm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "d2gpm.mak" CFG="d2gpm - Win32 x2gpmDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "d2gpm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "d2gpm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "d2gpm - Win32 x2gpmRelease" (based on "Win32 (x86) Application")
!MESSAGE "d2gpm - Win32 x2gpmDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "d2gpm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VCProjectDebug"
# PROP BASE Intermediate_Dir "VCProjectDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VCProjectDebug"
# PROP Intermediate_Dir "VCProjectDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /FR /Fd"VCProjectDebug\d2gpm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "__WXDEBUG__" /D "_DEBUG" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "DITA" /FR /Fd"VCProjectDebug\d2gpm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
# ADD RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "__WXDEBUG__" /d "_DEBUG" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "DITA"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build
TargetPath=.\VCProjectDebug\d2gpm.exe
TargetName=d2gpm
InputPath=.\VCProjectDebug\d2gpm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "d2gpm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VCProjectRelease"
# PROP BASE Intermediate_Dir "VCProjectRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VCProjectRelease"
# PROP Intermediate_Dir "VCProjectRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /FR /Fd"VCProjectRelease\d2gpm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "DITA" /FR /Fd"VCProjectRelease\d2gpm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "DITA"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build - Copying $(InputPath) to %omsyshome%
TargetPath=.\VCProjectRelease\d2gpm.exe
TargetName=d2gpm
InputPath=.\VCProjectRelease\d2gpm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "d2gpm - Win32 x2gpmRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "d2gpm___Win32_x2gpmRelease"
# PROP BASE Intermediate_Dir "d2gpm___Win32_x2gpmRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "x2gpmRelease"
# PROP Intermediate_Dir "x2gpmRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /FR /Fd"VCProjectRelease\d2gpm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "UDOC" /FR /Fd"VCProjectRelease\d2gpm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "UDOC"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build - Copying $(InputPath) to %omsyshome%
TargetPath=.\x2gpmRelease\d2gpm.exe
InputPath=.\x2gpmRelease\d2gpm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\x2gpm.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\x2gpm.exe

# End Custom Build

!ELSEIF  "$(CFG)" == "d2gpm - Win32 x2gpmDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "d2gpm___Win32_x2gpmDebug"
# PROP BASE Intermediate_Dir "d2gpm___Win32_x2gpmDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "x2gpmDebug"
# PROP Intermediate_Dir "x2gpmDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /FR /Fd"VCProjectDebug\d2gpm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "__WXDEBUG__" /D "_DEBUG" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "UDOC" /FR /Fd"VCProjectDebug\d2gpm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "__WXDEBUG__" /d "_DEBUG" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "UDOC"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build
TargetPath=.\x2gpmDebug\d2gpm.exe
InputPath=.\x2gpmDebug\d2gpm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\x2gpm.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin\x2gpm.exe

# End Custom Build

!ENDIF 

# Begin Target

# Name "d2gpm - Win32 Debug"
# Name "d2gpm - Win32 Release"
# Name "d2gpm - Win32 x2gpmRelease"
# Name "d2gpm - Win32 x2gpmDebug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="configstart.cpp"
# End Source File
# Begin Source File

SOURCE="d2gdcl.cpp"
# End Source File
# Begin Source File

SOURCE="d2gmain.cpp"
# End Source File
# Begin Source File

SOURCE="d2gmanager.cpp"
# End Source File
# Begin Source File

SOURCE="d2gui.rc"

!IF  "$(CFG)" == "d2gpm - Win32 Debug"

!ELSEIF  "$(CFG)" == "d2gpm - Win32 Release"

!ELSEIF  "$(CFG)" == "d2gpm - Win32 x2gpmRelease"

!ELSEIF  "$(CFG)" == "d2gpm - Win32 x2gpmDebug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="d2guiapp.cpp"
# End Source File
# Begin Source File

SOURCE="d2gutil.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcbase.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcfile.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcfili.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcfilp.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcfilr.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcfilw.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcglob.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dclist.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dclog.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcnode.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcnodt.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcport.cpp"
# End Source File
# Begin Source File

SOURCE="initoedit.cpp"
# End Source File
# Begin Source File

SOURCE="optrest.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="configstart.h"
# End Source File
# Begin Source File

SOURCE="d2gmain.h"
# End Source File
# Begin Source File

SOURCE="d2gmanager.h"
# End Source File
# Begin Source File

SOURCE="d2guiapp.h"
# End Source File
# Begin Source File

SOURCE="initoedit.h"
# End Source File
# Begin Source File

SOURCE="optrest.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cross.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\std.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\wx.manifest
# End Source File
# End Target
# End Project
