# Microsoft Developer Studio Project File - Name="d2gcm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=d2gcm - Win32 M2GcmDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "d2gcm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "d2gcm.mak" CFG="d2gcm - Win32 M2GcmDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "d2gcm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "d2gcm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "d2gcm - Win32 M2GcmRelease" (based on "Win32 (x86) Application")
!MESSAGE "d2gcm - Win32 M2GcmDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "d2gcm - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /FR /Fd"VCProjectDebug\d2gcm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /FR /Fd"VCProjectDebug\d2gcm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"

!ELSEIF  "$(CFG)" == "d2gcm - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\cvs\base" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /FR /Fd"VCProjectRelease\d2gcm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\cvs\base" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /FR /Fd"VCProjectRelease\d2gcm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /map /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=.\VCProjectRelease\d2gcm.exe
TargetName=d2gcm
InputPath=.\VCProjectRelease\d2gcm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "d2gcm - Win32 M2GcmRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "d2gcm___Win32_M2GcmRelease"
# PROP BASE Intermediate_Dir "d2gcm___Win32_M2GcmRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "M2GcmRelease"
# PROP Intermediate_Dir "M2GcmRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\cvs\base" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /FR /Fd"VCProjectRelease\d2gcm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /I "C:\cvs\base" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "_M2GCM" /FR /Fd"VCProjectRelease\d2gcm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/msw" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "_M2GCM"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"M2GcmRelease/m2gcm.bsc"
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /map /machine:I386 /out:"M2GcmRelease/m2gcm.exe" /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# Begin Custom Build - Copying to omsyshome directory...
TargetPath=.\M2GcmRelease\m2gcm.exe
TargetName=m2gcm
InputPath=.\M2GcmRelease\m2gcm.exe
SOURCE="$(InputPath)"

"C:\omsys\common\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\omsys\common\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "d2gcm - Win32 M2GcmDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "d2gcm___Win32_M2GcmDebug"
# PROP BASE Intermediate_Dir "d2gcm___Win32_M2GcmDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "M2GcmDebug"
# PROP Intermediate_Dir "M2GcmDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /FR /Fd"VCProjectDebug\d2gcm.pdb" /FD /EHsc /c
# ADD CPP /nologo /MDd /W4 /GR /Zi /Od /I "C:\wxWin\wxWidgets-2.8.11/include" /I "C:\wxWin\wxWidgets-2.8.11/contrib/include" /I "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /I "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /D "_M2GCM" /FR /Fd"VCProjectDebug\d2gcm.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NOPCH" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "__WXDEBUG__" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG"
# ADD RSC /l 0x409 /i "C:\wxWin\wxWidgets-2.8.11/include" /i "C:\wxWin\wxWidgets-2.8.11/contrib/include" /i "C:\wxWin\wxWidgets-2.8.11/lib/vc_lib/mswd" /i "C:\Program Files (x86)\Microsoft Visual Studio\vc98\include" /d "NOPCH" /d "WIN32" /d "__WXMSW__" /d "_WINDOWS" /d "__WXDEBUG__" /d "_DEBUG" /d "_M2GCM"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"M2GcmDebug/m2gcm.bsc"
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"
# ADD LINK32 wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"M2GcmDebug/m2gcm.exe" /libpath:"C:\wxWin\wxWidgets-2.8.11/lib/vc_lib" /libpath:"C:\Program Files (x86)\Microsoft Visual Studio\vc98\lib"

!ENDIF 

# Begin Target

# Name "d2gcm - Win32 Debug"
# Name "d2gcm - Win32 Release"
# Name "d2gcm - Win32 M2GcmRelease"
# Name "d2gcm - Win32 M2GcmDebug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="cmdcl.cpp"
# End Source File
# Begin Source File

SOURCE=.\cmdita.cpp
# End Source File
# Begin Source File

SOURCE="configmgr.cpp"
# End Source File
# Begin Source File

SOURCE="d2gcm.rc"
# End Source File
# Begin Source File

SOURCE="d2gcmapp.cpp"
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

SOURCE="..\base\dcfmtr.cpp"
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

SOURCE="..\base\dcmem.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcnode.cpp"
# End Source File
# Begin Source File

SOURCE="..\base\dcnodt.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="cmdcl.h"
# End Source File
# Begin Source File

SOURCE="configmgr.h"
# End Source File
# Begin Source File

SOURCE="d2gcmapp.h"
# End Source File
# Begin Source File

SOURCE=..\base\dcbase.h
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

SOURCE=..\base\dclist.h
# End Source File
# Begin Source File

SOURCE=..\base\dclog.h
# End Source File
# Begin Source File

SOURCE="..\..\wxWin\wxWidgets-2.8.11\include\wx\dcmemory.h"
# End Source File
# Begin Source File

SOURCE="..\..\wxWin\wxWidgets-2.8.11\include\wx\msw\dcmemory.h"
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
# End Group
# Begin Group "Icons"

# PROP Default_Filter "ico"
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
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

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\std.ico
# End Source File
# End Group
# Begin Group "Cursors"

# PROP Default_Filter "cur"
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cross.cur
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

SOURCE=.\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\wx\msw\wx.manifest
# End Source File
# End Target
# End Project
