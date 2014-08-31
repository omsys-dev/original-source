# Microsoft Developer Studio Project File - Name="d2gui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=d2gui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "d2gui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "d2gui.mak" CFG="d2gui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "d2gui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "d2gui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF "$(CFG)" == "d2gui - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VCProjectDebug"
# PROP BASE Intermediate_Dir "VCProjectDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "VCProjectDebug"
# PROP Intermediate_Dir "VCProjectDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /Fd"VCProjectDebug\d2gui.pdb" /MDd /Od /GR /EHsc /W4 /Zi /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG  /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/mswd" /I "C:\VS6\vc98\include D:\cvs\base" /c
# ADD CPP /nologo /FD /Fd"VCProjectDebug\d2gui.pdb" /MDd /Od /GR /EHsc /W4 /Zi /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG  /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/mswd" /I "C:\VS6\vc98\include D:\cvs\base" /c
# ADD BASE MTL /nologo /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG /mktyplib203 /win32
# ADD MTL /nologo /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/mswd" /I "C:\VS6\vc98\include D:\cvs\base"
# ADD RSC /l 0x405 /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /D__WXDEBUG__ /D_DEBUG /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/mswd" /I "C:\VS6\vc98\include D:\cvs\base"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /libpath:"D:\wxWin\wxMSW-2.8.10/lib/vc_lib" /libpath:"C:\VS6\vc98\lib" /nologo /SUBSYSTEM:WINDOWS /machine:i386 /DEBUG /out:"VCProjectDebug\d2gui.exe"
# ADD LINK32  wxmsw28d_richtext.lib wxmsw28d_aui.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxmsw28d_adv.lib wxmsw28d_html.lib wxmsw28d_xrc.lib wxbase28d_net.lib wxbase28d_xml.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /libpath:"D:\wxWin\wxMSW-2.8.10/lib/vc_lib" /libpath:"C:\VS6\vc98\lib" /nologo /SUBSYSTEM:WINDOWS /machine:i386 /DEBUG /out:"VCProjectDebug\d2gui.exe"

!ELSEIF "$(CFG)" == "d2gui - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VCProjectRelease"
# PROP BASE Intermediate_Dir "VCProjectRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "VCProjectRelease"
# PROP Intermediate_Dir "VCProjectRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /Fd"VCProjectRelease\d2gui.pdb" /MD /O2 /GR /EHsc /W4  /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS  /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/msw" /I "C:\VS6\vc98\include d:\cvs\base" /c
# ADD CPP /nologo /FD /Fd"VCProjectRelease\d2gui.pdb" /MD /O2 /GR /EHsc /W4  /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS  /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/msw" /I "C:\VS6\vc98\include d:\cvs\base" /c
# ADD BASE MTL /nologo /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /mktyplib203 /win32
# ADD MTL /nologo /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/msw" /I "C:\VS6\vc98\include d:\cvs\base"
# ADD RSC /l 0x405 /DNOPCH /DWIN32 /D__WXMSW__ /D_WINDOWS /I "D:\wxWin\wxMSW-2.8.10/include" /I "D:\wxWin\wxMSW-2.8.10/contrib/include" /I "D:\wxWin\wxMSW-2.8.10/lib/vc_lib/msw" /I "C:\VS6\vc98\include d:\cvs\base"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /libpath:"D:\wxWin\wxMSW-2.8.10/lib/vc_lib" /libpath:"C:\VS6\vc98\lib" /nologo /SUBSYSTEM:WINDOWS /machine:i386 /out:"VCProjectRelease\d2gui.exe"
# ADD LINK32  wxmsw28_richtext.lib wxmsw28_aui.lib wxmsw28_html.lib wxmsw28_xrc.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxmsw28_adv.lib wxmsw28_html.lib wxmsw28_xrc.lib wxbase28_net.lib wxbase28_xml.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /libpath:"D:\wxWin\wxMSW-2.8.10/lib/vc_lib" /libpath:"C:\VS6\vc98\lib" /nologo /SUBSYSTEM:WINDOWS /machine:i386 /out:"VCProjectRelease\d2gui.exe"

!ENDIF

# Begin Target

# Name "d2gui - Win32 Debug"
# Name "d2gui - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\cvs\base\dcfilp.cpp"
# End Source File
# Begin Source File

SOURCE="..\cvs\base\dcnode.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcbase.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcfile.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcfili.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcfilr.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcfilw.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcglob.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dclist.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dclog.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcnodt.cpp"
# End Source File
# Begin Source File

SOURCE="D:\cvs\base\dcport.cpp"
# End Source File
# Begin Source File

SOURCE="app_resources.cpp"
# End Source File
# Begin Source File

SOURCE="d2gdcl.cpp"
# End Source File
# Begin Source File

SOURCE="d2gmain.cpp"
# End Source File
# Begin Source File

SOURCE="d2guiapp.cpp"
# End Source File
# Begin Source File

SOURCE="d2gui.rc"
# End Source File

# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="d2gmain.h"
# End Source File
# Begin Source File

SOURCE="d2guiapp.h"
# End Source File

# End Group
# End Target
# End Project

