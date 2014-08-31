# Microsoft Developer Studio Project File - Name="m2rbook" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=m2rbook - Win32 FM8 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "m2rbook.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "m2rbook.mak" CFG="m2rbook - Win32 FM8 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "m2rbook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Rel50" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Deb50" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Debug FM5" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 DbgRel" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 FM8 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 FM8 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "m2rbook - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "NEWINI" /FR /YX"m2rbook.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Shlwapi.lib fdk.lib api.lib fmdbms32.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /libpath:"D:\FDK6\lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none /debug
# Begin Custom Build - Copying to FrameMaker 8 and 9 directories...
TargetPath=\cvs\fdk\Release\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Release\m2rbook.dll
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

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "NEWINI" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 Shlwapi.lib fdk.lib api.lib fmdbms32.lib htmlhelp.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /libpath:"D:\FDK6\lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker 8 and 9 directories...
TargetPath=\cvs\fdk\Debug\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Debug\m2rbook.dll
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

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\m2rbook_"
# PROP BASE Intermediate_Dir ".\m2rbook_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\fdk\Rel50"
# PROP Intermediate_Dir "..\fdk\Rel50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FM50VER" /Fr /YX"m2rbook.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fdk50.lib api50.lib fmdbms50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Rel50\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Rel50\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\m2rbook0"
# PROP BASE Intermediate_Dir ".\m2rbook0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\fdk\Deb50"
# PROP Intermediate_Dir "..\fdk\Deb50"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FM50VER" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fdk50.lib api50.lib fmdbms50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Deb50\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Deb50\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\m2rbook_"
# PROP BASE Intermediate_Dir ".\m2rbook_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\fdk\Debug"
# PROP Intermediate_Dir "..\fdk\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fdk.lib api.lib fmdbms32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Debug\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Debug\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 DbgRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "m2rbook___Win32_DbgRel"
# PROP BASE Intermediate_Dir "m2rbook___Win32_DbgRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "m2rbook___Win32_DbgRel"
# PROP Intermediate_Dir "m2rbook___Win32_DbgRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fdk.lib api.lib fmdbms32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /incremental:yes /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /libpath:"D:\FDK6\lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=.\m2rbook___Win32_DbgRel\m2rbook.dll
TargetName=m2rbook
InputPath=.\m2rbook___Win32_DbgRel\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 FM8 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "m2rbook___Win32_FM8_Debug"
# PROP BASE Intermediate_Dir "m2rbook___Win32_FM8_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\fdk\Debug8"
# PROP Intermediate_Dir "..\fdk\Debug8"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /Fr /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FDK8" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 bufferoverflowU.lib fdk.lib api.lib fmdbms32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcmt.lib" /libpath:"D:\FDK8\lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Debug8\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Debug8\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 FM8 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "m2rbook___Win32_FM8_Release"
# PROP BASE Intermediate_Dir "m2rbook___Win32_FM8_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\fdk\Release8"
# PROP Intermediate_Dir "..\fdk\Release8"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FDK8" /FR /YX"m2rbook.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 fdk.lib api.lib fmdbms32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /libpath:"D:\FDK8\lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Release8\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Release8\m2rbook.dll
SOURCE="$(InputPath)"

"C:\FM8\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) C:\FM8\fminit\plugins 
	del /q C:\FM8\fminit\plugins\.cache 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "m2rbook - Win32 Release"
# Name "m2rbook - Win32 Debug"
# Name "m2rbook - Win32 Rel50"
# Name "m2rbook - Win32 Deb50"
# Name "m2rbook - Win32 Debug FM5"
# Name "m2rbook - Win32 DbgRel"
# Name "m2rbook - Win32 FM8 Debug"
# Name "m2rbook - Win32 FM8 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\m2rbook.c
# End Source File
# Begin Source File

SOURCE=.\m2rbook.rc
# End Source File
# Begin Source File

SOURCE=.\m2rdcnt.c
# End Source File
# Begin Source File

SOURCE=.\m2rdes.c
# End Source File
# Begin Source File

SOURCE=.\m2rdfnt.c
# End Source File
# Begin Source File

SOURCE=.\m2rdgi.c
# End Source File
# Begin Source File

SOURCE=.\m2rdgr.c
# End Source File
# Begin Source File

SOURCE=.\m2rdgra.c
# End Source File
# Begin Source File

SOURCE=.\m2rdgrb.c
# End Source File
# Begin Source File

SOURCE=.\m2rdgt.c
# End Source File
# Begin Source File

SOURCE=.\m2rdhmk.c
# End Source File
# Begin Source File

SOURCE=.\m2rdhpj.c
# End Source File
# Begin Source File

SOURCE=.\m2rdlg.c
# End Source File
# Begin Source File

SOURCE=.\m2rdmf.c
# End Source File
# Begin Source File

SOURCE=.\m2rdshb.c
# End Source File
# Begin Source File

SOURCE=.\m2rdshh.c
# End Source File
# Begin Source File

SOURCE=.\m2rdshm.c
# End Source File
# Begin Source File

SOURCE=.\m2rdshs.c
# End Source File
# Begin Source File

SOURCE=.\m2rdsht.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtbl.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtl.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtpl.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtxa.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtxb.c
# End Source File
# Begin Source File

SOURCE=.\m2rdtxh.c
# End Source File
# Begin Source File

SOURCE=.\m2rdver.c
# End Source File
# Begin Source File

SOURCE=.\m2rgr.c
# End Source File
# Begin Source File

SOURCE=.\m2rgrx.c
# End Source File
# Begin Source File

SOURCE=.\m2rini.c
# End Source File
# Begin Source File

SOURCE=.\m2rinid.c
# End Source File
# Begin Source File

SOURCE=.\m2rinit.c
# End Source File
# Begin Source File

SOURCE=.\m2rmenu.c
# End Source File
# Begin Source File

SOURCE=.\m2rproc.c
# End Source File
# Begin Source File

SOURCE=.\m2rproj.c
# End Source File
# Begin Source File

SOURCE=.\m2rset.c
# End Source File
# Begin Source File

SOURCE=.\m2rseta.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetb.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetc.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetd.c
# End Source File
# Begin Source File

SOURCE=.\m2rsete.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetf.c
# End Source File
# Begin Source File

SOURCE=.\m2rseth.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetj.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetm.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetn.c
# End Source File
# Begin Source File

SOURCE=.\m2rsetw.c
# End Source File
# Begin Source File

SOURCE=.\m2rstr.c
# End Source File
# Begin Source File

SOURCE=.\m2rsty.c
# End Source File
# Begin Source File

SOURCE=.\m2rtxt.c
# End Source File
# Begin Source File

SOURCE=.\m2rutil.c
# End Source File
# Begin Source File

SOURCE=.\m2rvar.c
# End Source File
# Begin Source File

SOURCE=.\m2rwrt.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\m2rbook.h
# End Source File
# Begin Source File

SOURCE=.\m2rhids.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\m2rdcnt.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdfnt.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdgi.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdgr.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdgra.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdgrb.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdgt.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdhmk.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdhpj.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdmf.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdshb.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdshh.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdshm.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdshs.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdsht.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtbl.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtl.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtpl.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtxa.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtxb.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdtxh.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rdver.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rinit.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rproj.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rseta.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetb.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetc.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetd.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsete.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetf.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rseth.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetj.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetm.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetn.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rsetw.dlg
# End Source File
# Begin Source File

SOURCE=.\m2rvar.dlg
# End Source File
# End Group
# Begin Source File

SOURCE=.\m2rdcnt.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdfnt.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdgi.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdgr.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdgra.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdgrb.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdgt.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdhmk.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdhpj.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdmf.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdshb.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdshh.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdshm.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdshs.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdsht.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtbl.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtl.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtpl.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtxa.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtxb.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdtxh.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rdver.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rinit.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rproj.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rseta.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetb.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetc.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetd.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsete.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetf.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rseth.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetj.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetm.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetn.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rsetw.xdi
# End Source File
# Begin Source File

SOURCE=.\m2rvar.xdi
# End Source File
# End Target
# End Project
