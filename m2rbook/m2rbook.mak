# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=m2rbook - Win32 Debug FM5
!MESSAGE No configuration specified.  Defaulting to m2rbook - Win32 Debug FM5.
!ENDIF 

!IF "$(CFG)" != "m2rbook - Win32 Release" && "$(CFG)" !=\
 "m2rbook - Win32 Debug" && "$(CFG)" != "m2rbook - Win32 Rel50" && "$(CFG)" !=\
 "m2rbook - Win32 Deb50" && "$(CFG)" != "m2rbook - Win32 Debug FM5"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "m2rbook.mak" CFG="m2rbook - Win32 Debug FM5"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "m2rbook - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Rel50" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Deb50" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "m2rbook - Win32 Debug FM5" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "m2rbook - Win32 Release"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "m2rbook - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../fdk/Release"
# PROP Intermediate_Dir "../fdk/Release"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Release
INTDIR=.\../fdk/Release
# Begin Custom Macros
TargetName=m2rbook
# End Custom Macros

ALL : "$(OUTDIR)\m2rbook.dll" "$(OUTDIR)\m2rbook.bsc"\
 "..\..\FM7.1\fminit\plugins\m2rbook.dll" "..\..\FM7.1\fminit\plugins\.cache"

CLEAN : 
	-@erase "$(INTDIR)\m2rbook.obj"
	-@erase "$(INTDIR)\m2rbook.res"
	-@erase "$(INTDIR)\m2rbook.sbr"
	-@erase "$(INTDIR)\m2rdcnt.obj"
	-@erase "$(INTDIR)\m2rdcnt.sbr"
	-@erase "$(INTDIR)\m2rdes.obj"
	-@erase "$(INTDIR)\m2rdes.sbr"
	-@erase "$(INTDIR)\m2rdfnt.obj"
	-@erase "$(INTDIR)\m2rdfnt.sbr"
	-@erase "$(INTDIR)\m2rdgi.obj"
	-@erase "$(INTDIR)\m2rdgi.sbr"
	-@erase "$(INTDIR)\m2rdgr.obj"
	-@erase "$(INTDIR)\m2rdgr.sbr"
	-@erase "$(INTDIR)\m2rdgra.obj"
	-@erase "$(INTDIR)\m2rdgra.sbr"
	-@erase "$(INTDIR)\m2rdgrb.obj"
	-@erase "$(INTDIR)\m2rdgrb.sbr"
	-@erase "$(INTDIR)\m2rdgt.obj"
	-@erase "$(INTDIR)\m2rdgt.sbr"
	-@erase "$(INTDIR)\m2rdhmk.obj"
	-@erase "$(INTDIR)\m2rdhmk.sbr"
	-@erase "$(INTDIR)\m2rdhpj.obj"
	-@erase "$(INTDIR)\m2rdhpj.sbr"
	-@erase "$(INTDIR)\m2rdlg.obj"
	-@erase "$(INTDIR)\m2rdlg.sbr"
	-@erase "$(INTDIR)\m2rdmf.obj"
	-@erase "$(INTDIR)\m2rdmf.sbr"
	-@erase "$(INTDIR)\m2rdshb.obj"
	-@erase "$(INTDIR)\m2rdshb.sbr"
	-@erase "$(INTDIR)\m2rdshh.obj"
	-@erase "$(INTDIR)\m2rdshh.sbr"
	-@erase "$(INTDIR)\m2rdshm.obj"
	-@erase "$(INTDIR)\m2rdshm.sbr"
	-@erase "$(INTDIR)\m2rdshs.obj"
	-@erase "$(INTDIR)\m2rdshs.sbr"
	-@erase "$(INTDIR)\m2rdsht.obj"
	-@erase "$(INTDIR)\m2rdsht.sbr"
	-@erase "$(INTDIR)\m2rdtbl.obj"
	-@erase "$(INTDIR)\m2rdtbl.sbr"
	-@erase "$(INTDIR)\m2rdtl.obj"
	-@erase "$(INTDIR)\m2rdtl.sbr"
	-@erase "$(INTDIR)\m2rdtpl.obj"
	-@erase "$(INTDIR)\m2rdtpl.sbr"
	-@erase "$(INTDIR)\m2rdtxa.obj"
	-@erase "$(INTDIR)\m2rdtxa.sbr"
	-@erase "$(INTDIR)\m2rdtxb.obj"
	-@erase "$(INTDIR)\m2rdtxb.sbr"
	-@erase "$(INTDIR)\m2rdtxh.obj"
	-@erase "$(INTDIR)\m2rdtxh.sbr"
	-@erase "$(INTDIR)\m2rdver.obj"
	-@erase "$(INTDIR)\m2rdver.sbr"
	-@erase "$(INTDIR)\m2rgr.obj"
	-@erase "$(INTDIR)\m2rgr.sbr"
	-@erase "$(INTDIR)\m2rgrx.obj"
	-@erase "$(INTDIR)\m2rgrx.sbr"
	-@erase "$(INTDIR)\m2rini.obj"
	-@erase "$(INTDIR)\m2rini.sbr"
	-@erase "$(INTDIR)\m2rinit.obj"
	-@erase "$(INTDIR)\m2rinit.sbr"
	-@erase "$(INTDIR)\m2rmenu.obj"
	-@erase "$(INTDIR)\m2rmenu.sbr"
	-@erase "$(INTDIR)\m2rproc.obj"
	-@erase "$(INTDIR)\m2rproc.sbr"
	-@erase "$(INTDIR)\m2rproj.obj"
	-@erase "$(INTDIR)\m2rproj.sbr"
	-@erase "$(INTDIR)\m2rset.obj"
	-@erase "$(INTDIR)\m2rset.sbr"
	-@erase "$(INTDIR)\m2rseta.obj"
	-@erase "$(INTDIR)\m2rseta.sbr"
	-@erase "$(INTDIR)\m2rsetb.obj"
	-@erase "$(INTDIR)\m2rsetb.sbr"
	-@erase "$(INTDIR)\m2rsetc.obj"
	-@erase "$(INTDIR)\m2rsetc.sbr"
	-@erase "$(INTDIR)\m2rsetd.obj"
	-@erase "$(INTDIR)\m2rsetd.sbr"
	-@erase "$(INTDIR)\m2rsete.obj"
	-@erase "$(INTDIR)\m2rsete.sbr"
	-@erase "$(INTDIR)\m2rsetf.obj"
	-@erase "$(INTDIR)\m2rsetf.sbr"
	-@erase "$(INTDIR)\m2rseth.obj"
	-@erase "$(INTDIR)\m2rseth.sbr"
	-@erase "$(INTDIR)\m2rsetj.obj"
	-@erase "$(INTDIR)\m2rsetj.sbr"
	-@erase "$(INTDIR)\m2rsetm.obj"
	-@erase "$(INTDIR)\m2rsetm.sbr"
	-@erase "$(INTDIR)\m2rsetn.obj"
	-@erase "$(INTDIR)\m2rsetn.sbr"
	-@erase "$(INTDIR)\m2rsetw.obj"
	-@erase "$(INTDIR)\m2rsetw.sbr"
	-@erase "$(INTDIR)\m2rstr.obj"
	-@erase "$(INTDIR)\m2rstr.sbr"
	-@erase "$(INTDIR)\m2rsty.obj"
	-@erase "$(INTDIR)\m2rsty.sbr"
	-@erase "$(INTDIR)\m2rtxt.obj"
	-@erase "$(INTDIR)\m2rtxt.sbr"
	-@erase "$(INTDIR)\m2rutil.obj"
	-@erase "$(INTDIR)\m2rutil.sbr"
	-@erase "$(INTDIR)\m2rvar.obj"
	-@erase "$(INTDIR)\m2rvar.sbr"
	-@erase "$(INTDIR)\m2rwrt.obj"
	-@erase "$(INTDIR)\m2rwrt.sbr"
	-@erase "$(OUTDIR)\m2rbook.bsc"
	-@erase "$(OUTDIR)\m2rbook.dll"
	-@erase "$(OUTDIR)\m2rbook.exp"
	-@erase "$(OUTDIR)\m2rbook.ilk"
	-@erase "$(OUTDIR)\m2rbook.lib"
	-@erase "..\..\FM7.1\fminit\plugins\.cache"
	-@erase "..\..\FM7.1\fminit\plugins\m2rbook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "M2RBOOK" /FR"$(INTDIR)/" /Fp"$(INTDIR)/m2rbook.pch" /YX"m2rbook.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Release/
CPP_SBRS=.\../fdk/Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2rbook.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2rbook.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\m2rbook.sbr" \
	"$(INTDIR)\m2rdcnt.sbr" \
	"$(INTDIR)\m2rdes.sbr" \
	"$(INTDIR)\m2rdfnt.sbr" \
	"$(INTDIR)\m2rdgi.sbr" \
	"$(INTDIR)\m2rdgr.sbr" \
	"$(INTDIR)\m2rdgra.sbr" \
	"$(INTDIR)\m2rdgrb.sbr" \
	"$(INTDIR)\m2rdgt.sbr" \
	"$(INTDIR)\m2rdhmk.sbr" \
	"$(INTDIR)\m2rdhpj.sbr" \
	"$(INTDIR)\m2rdlg.sbr" \
	"$(INTDIR)\m2rdmf.sbr" \
	"$(INTDIR)\m2rdshb.sbr" \
	"$(INTDIR)\m2rdshh.sbr" \
	"$(INTDIR)\m2rdshm.sbr" \
	"$(INTDIR)\m2rdshs.sbr" \
	"$(INTDIR)\m2rdsht.sbr" \
	"$(INTDIR)\m2rdtbl.sbr" \
	"$(INTDIR)\m2rdtl.sbr" \
	"$(INTDIR)\m2rdtpl.sbr" \
	"$(INTDIR)\m2rdtxa.sbr" \
	"$(INTDIR)\m2rdtxb.sbr" \
	"$(INTDIR)\m2rdtxh.sbr" \
	"$(INTDIR)\m2rdver.sbr" \
	"$(INTDIR)\m2rgr.sbr" \
	"$(INTDIR)\m2rgrx.sbr" \
	"$(INTDIR)\m2rini.sbr" \
	"$(INTDIR)\m2rinit.sbr" \
	"$(INTDIR)\m2rmenu.sbr" \
	"$(INTDIR)\m2rproc.sbr" \
	"$(INTDIR)\m2rproj.sbr" \
	"$(INTDIR)\m2rset.sbr" \
	"$(INTDIR)\m2rseta.sbr" \
	"$(INTDIR)\m2rsetb.sbr" \
	"$(INTDIR)\m2rsetc.sbr" \
	"$(INTDIR)\m2rsetd.sbr" \
	"$(INTDIR)\m2rsete.sbr" \
	"$(INTDIR)\m2rsetf.sbr" \
	"$(INTDIR)\m2rseth.sbr" \
	"$(INTDIR)\m2rsetj.sbr" \
	"$(INTDIR)\m2rsetm.sbr" \
	"$(INTDIR)\m2rsetn.sbr" \
	"$(INTDIR)\m2rsetw.sbr" \
	"$(INTDIR)\m2rstr.sbr" \
	"$(INTDIR)\m2rsty.sbr" \
	"$(INTDIR)\m2rtxt.sbr" \
	"$(INTDIR)\m2rutil.sbr" \
	"$(INTDIR)\m2rvar.sbr" \
	"$(INTDIR)\m2rwrt.sbr"

"$(OUTDIR)\m2rbook.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2rbook.pdb" /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2rbook.dll" /implib:"$(OUTDIR)/m2rbook.lib" /section:.rsrc,w 
LINK32_OBJS= \
	"$(INTDIR)\m2rbook.obj" \
	"$(INTDIR)\m2rbook.res" \
	"$(INTDIR)\m2rdcnt.obj" \
	"$(INTDIR)\m2rdes.obj" \
	"$(INTDIR)\m2rdfnt.obj" \
	"$(INTDIR)\m2rdgi.obj" \
	"$(INTDIR)\m2rdgr.obj" \
	"$(INTDIR)\m2rdgra.obj" \
	"$(INTDIR)\m2rdgrb.obj" \
	"$(INTDIR)\m2rdgt.obj" \
	"$(INTDIR)\m2rdhmk.obj" \
	"$(INTDIR)\m2rdhpj.obj" \
	"$(INTDIR)\m2rdlg.obj" \
	"$(INTDIR)\m2rdmf.obj" \
	"$(INTDIR)\m2rdshb.obj" \
	"$(INTDIR)\m2rdshh.obj" \
	"$(INTDIR)\m2rdshm.obj" \
	"$(INTDIR)\m2rdshs.obj" \
	"$(INTDIR)\m2rdsht.obj" \
	"$(INTDIR)\m2rdtbl.obj" \
	"$(INTDIR)\m2rdtl.obj" \
	"$(INTDIR)\m2rdtpl.obj" \
	"$(INTDIR)\m2rdtxa.obj" \
	"$(INTDIR)\m2rdtxb.obj" \
	"$(INTDIR)\m2rdtxh.obj" \
	"$(INTDIR)\m2rdver.obj" \
	"$(INTDIR)\m2rgr.obj" \
	"$(INTDIR)\m2rgrx.obj" \
	"$(INTDIR)\m2rini.obj" \
	"$(INTDIR)\m2rinit.obj" \
	"$(INTDIR)\m2rmenu.obj" \
	"$(INTDIR)\m2rproc.obj" \
	"$(INTDIR)\m2rproj.obj" \
	"$(INTDIR)\m2rset.obj" \
	"$(INTDIR)\m2rseta.obj" \
	"$(INTDIR)\m2rsetb.obj" \
	"$(INTDIR)\m2rsetc.obj" \
	"$(INTDIR)\m2rsetd.obj" \
	"$(INTDIR)\m2rsete.obj" \
	"$(INTDIR)\m2rsetf.obj" \
	"$(INTDIR)\m2rseth.obj" \
	"$(INTDIR)\m2rsetj.obj" \
	"$(INTDIR)\m2rsetm.obj" \
	"$(INTDIR)\m2rsetn.obj" \
	"$(INTDIR)\m2rsetw.obj" \
	"$(INTDIR)\m2rstr.obj" \
	"$(INTDIR)\m2rsty.obj" \
	"$(INTDIR)\m2rtxt.obj" \
	"$(INTDIR)\m2rutil.obj" \
	"$(INTDIR)\m2rvar.obj" \
	"$(INTDIR)\m2rwrt.obj"

"$(OUTDIR)\m2rbook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Release\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Release\m2rbook.dll
SOURCE=$(InputPath)

BuildCmds= \
	cmd /c copy $(TargetPath) d:\FM7.1\fminit\plugins \
	D:\cygwin\bin\touch d:\FM7.1\fminit\plugins\.cache \
	cmd /c del /q d:\FM7.1\fminit\plugins\.cache \
	

"d:\FM7.1\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"d:\FM7.1\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../fdk/Debug"
# PROP Intermediate_Dir "../fdk/Debug"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Debug
INTDIR=.\../fdk/Debug
# Begin Custom Macros
TargetName=m2rbook
# End Custom Macros

ALL : "$(OUTDIR)\m2rbook.dll" "$(OUTDIR)\m2rbook.bsc"\
 "..\..\FM7.1\fminit\plugins\m2rbook.dll" "..\..\FM7.1\fminit\plugins\.cache"

CLEAN : 
	-@erase "$(INTDIR)\m2rbook.obj"
	-@erase "$(INTDIR)\m2rbook.res"
	-@erase "$(INTDIR)\m2rbook.sbr"
	-@erase "$(INTDIR)\m2rdcnt.obj"
	-@erase "$(INTDIR)\m2rdcnt.sbr"
	-@erase "$(INTDIR)\m2rdes.obj"
	-@erase "$(INTDIR)\m2rdes.sbr"
	-@erase "$(INTDIR)\m2rdfnt.obj"
	-@erase "$(INTDIR)\m2rdfnt.sbr"
	-@erase "$(INTDIR)\m2rdgi.obj"
	-@erase "$(INTDIR)\m2rdgi.sbr"
	-@erase "$(INTDIR)\m2rdgr.obj"
	-@erase "$(INTDIR)\m2rdgr.sbr"
	-@erase "$(INTDIR)\m2rdgra.obj"
	-@erase "$(INTDIR)\m2rdgra.sbr"
	-@erase "$(INTDIR)\m2rdgrb.obj"
	-@erase "$(INTDIR)\m2rdgrb.sbr"
	-@erase "$(INTDIR)\m2rdgt.obj"
	-@erase "$(INTDIR)\m2rdgt.sbr"
	-@erase "$(INTDIR)\m2rdhmk.obj"
	-@erase "$(INTDIR)\m2rdhmk.sbr"
	-@erase "$(INTDIR)\m2rdhpj.obj"
	-@erase "$(INTDIR)\m2rdhpj.sbr"
	-@erase "$(INTDIR)\m2rdlg.obj"
	-@erase "$(INTDIR)\m2rdlg.sbr"
	-@erase "$(INTDIR)\m2rdmf.obj"
	-@erase "$(INTDIR)\m2rdmf.sbr"
	-@erase "$(INTDIR)\m2rdshb.obj"
	-@erase "$(INTDIR)\m2rdshb.sbr"
	-@erase "$(INTDIR)\m2rdshh.obj"
	-@erase "$(INTDIR)\m2rdshh.sbr"
	-@erase "$(INTDIR)\m2rdshm.obj"
	-@erase "$(INTDIR)\m2rdshm.sbr"
	-@erase "$(INTDIR)\m2rdshs.obj"
	-@erase "$(INTDIR)\m2rdshs.sbr"
	-@erase "$(INTDIR)\m2rdsht.obj"
	-@erase "$(INTDIR)\m2rdsht.sbr"
	-@erase "$(INTDIR)\m2rdtbl.obj"
	-@erase "$(INTDIR)\m2rdtbl.sbr"
	-@erase "$(INTDIR)\m2rdtl.obj"
	-@erase "$(INTDIR)\m2rdtl.sbr"
	-@erase "$(INTDIR)\m2rdtpl.obj"
	-@erase "$(INTDIR)\m2rdtpl.sbr"
	-@erase "$(INTDIR)\m2rdtxa.obj"
	-@erase "$(INTDIR)\m2rdtxa.sbr"
	-@erase "$(INTDIR)\m2rdtxb.obj"
	-@erase "$(INTDIR)\m2rdtxb.sbr"
	-@erase "$(INTDIR)\m2rdtxh.obj"
	-@erase "$(INTDIR)\m2rdtxh.sbr"
	-@erase "$(INTDIR)\m2rdver.obj"
	-@erase "$(INTDIR)\m2rdver.sbr"
	-@erase "$(INTDIR)\m2rgr.obj"
	-@erase "$(INTDIR)\m2rgr.sbr"
	-@erase "$(INTDIR)\m2rgrx.obj"
	-@erase "$(INTDIR)\m2rgrx.sbr"
	-@erase "$(INTDIR)\m2rini.obj"
	-@erase "$(INTDIR)\m2rini.sbr"
	-@erase "$(INTDIR)\m2rinit.obj"
	-@erase "$(INTDIR)\m2rinit.sbr"
	-@erase "$(INTDIR)\m2rmenu.obj"
	-@erase "$(INTDIR)\m2rmenu.sbr"
	-@erase "$(INTDIR)\m2rproc.obj"
	-@erase "$(INTDIR)\m2rproc.sbr"
	-@erase "$(INTDIR)\m2rproj.obj"
	-@erase "$(INTDIR)\m2rproj.sbr"
	-@erase "$(INTDIR)\m2rset.obj"
	-@erase "$(INTDIR)\m2rset.sbr"
	-@erase "$(INTDIR)\m2rseta.obj"
	-@erase "$(INTDIR)\m2rseta.sbr"
	-@erase "$(INTDIR)\m2rsetb.obj"
	-@erase "$(INTDIR)\m2rsetb.sbr"
	-@erase "$(INTDIR)\m2rsetc.obj"
	-@erase "$(INTDIR)\m2rsetc.sbr"
	-@erase "$(INTDIR)\m2rsetd.obj"
	-@erase "$(INTDIR)\m2rsetd.sbr"
	-@erase "$(INTDIR)\m2rsete.obj"
	-@erase "$(INTDIR)\m2rsete.sbr"
	-@erase "$(INTDIR)\m2rsetf.obj"
	-@erase "$(INTDIR)\m2rsetf.sbr"
	-@erase "$(INTDIR)\m2rseth.obj"
	-@erase "$(INTDIR)\m2rseth.sbr"
	-@erase "$(INTDIR)\m2rsetj.obj"
	-@erase "$(INTDIR)\m2rsetj.sbr"
	-@erase "$(INTDIR)\m2rsetm.obj"
	-@erase "$(INTDIR)\m2rsetm.sbr"
	-@erase "$(INTDIR)\m2rsetn.obj"
	-@erase "$(INTDIR)\m2rsetn.sbr"
	-@erase "$(INTDIR)\m2rsetw.obj"
	-@erase "$(INTDIR)\m2rsetw.sbr"
	-@erase "$(INTDIR)\m2rstr.obj"
	-@erase "$(INTDIR)\m2rstr.sbr"
	-@erase "$(INTDIR)\m2rsty.obj"
	-@erase "$(INTDIR)\m2rsty.sbr"
	-@erase "$(INTDIR)\m2rtxt.obj"
	-@erase "$(INTDIR)\m2rtxt.sbr"
	-@erase "$(INTDIR)\m2rutil.obj"
	-@erase "$(INTDIR)\m2rutil.sbr"
	-@erase "$(INTDIR)\m2rvar.obj"
	-@erase "$(INTDIR)\m2rvar.sbr"
	-@erase "$(INTDIR)\m2rwrt.obj"
	-@erase "$(INTDIR)\m2rwrt.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\m2rbook.bsc"
	-@erase "$(OUTDIR)\m2rbook.dll"
	-@erase "$(OUTDIR)\m2rbook.exp"
	-@erase "$(OUTDIR)\m2rbook.ilk"
	-@erase "$(OUTDIR)\m2rbook.lib"
	-@erase "$(OUTDIR)\m2rbook.pdb"
	-@erase "..\..\FM7.1\fminit\plugins\.cache"
	-@erase "..\..\FM7.1\fminit\plugins\m2rbook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /Fr /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "M2RBOOK" /Fr"$(INTDIR)/" /Fp"$(INTDIR)/m2rbook.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Debug/
CPP_SBRS=.\../fdk/Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2rbook.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2rbook.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\m2rbook.sbr" \
	"$(INTDIR)\m2rdcnt.sbr" \
	"$(INTDIR)\m2rdes.sbr" \
	"$(INTDIR)\m2rdfnt.sbr" \
	"$(INTDIR)\m2rdgi.sbr" \
	"$(INTDIR)\m2rdgr.sbr" \
	"$(INTDIR)\m2rdgra.sbr" \
	"$(INTDIR)\m2rdgrb.sbr" \
	"$(INTDIR)\m2rdgt.sbr" \
	"$(INTDIR)\m2rdhmk.sbr" \
	"$(INTDIR)\m2rdhpj.sbr" \
	"$(INTDIR)\m2rdlg.sbr" \
	"$(INTDIR)\m2rdmf.sbr" \
	"$(INTDIR)\m2rdshb.sbr" \
	"$(INTDIR)\m2rdshh.sbr" \
	"$(INTDIR)\m2rdshm.sbr" \
	"$(INTDIR)\m2rdshs.sbr" \
	"$(INTDIR)\m2rdsht.sbr" \
	"$(INTDIR)\m2rdtbl.sbr" \
	"$(INTDIR)\m2rdtl.sbr" \
	"$(INTDIR)\m2rdtpl.sbr" \
	"$(INTDIR)\m2rdtxa.sbr" \
	"$(INTDIR)\m2rdtxb.sbr" \
	"$(INTDIR)\m2rdtxh.sbr" \
	"$(INTDIR)\m2rdver.sbr" \
	"$(INTDIR)\m2rgr.sbr" \
	"$(INTDIR)\m2rgrx.sbr" \
	"$(INTDIR)\m2rini.sbr" \
	"$(INTDIR)\m2rinit.sbr" \
	"$(INTDIR)\m2rmenu.sbr" \
	"$(INTDIR)\m2rproc.sbr" \
	"$(INTDIR)\m2rproj.sbr" \
	"$(INTDIR)\m2rset.sbr" \
	"$(INTDIR)\m2rseta.sbr" \
	"$(INTDIR)\m2rsetb.sbr" \
	"$(INTDIR)\m2rsetc.sbr" \
	"$(INTDIR)\m2rsetd.sbr" \
	"$(INTDIR)\m2rsete.sbr" \
	"$(INTDIR)\m2rsetf.sbr" \
	"$(INTDIR)\m2rseth.sbr" \
	"$(INTDIR)\m2rsetj.sbr" \
	"$(INTDIR)\m2rsetm.sbr" \
	"$(INTDIR)\m2rsetn.sbr" \
	"$(INTDIR)\m2rsetw.sbr" \
	"$(INTDIR)\m2rstr.sbr" \
	"$(INTDIR)\m2rsty.sbr" \
	"$(INTDIR)\m2rtxt.sbr" \
	"$(INTDIR)\m2rutil.sbr" \
	"$(INTDIR)\m2rvar.sbr" \
	"$(INTDIR)\m2rwrt.sbr"

"$(OUTDIR)\m2rbook.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2rbook.pdb" /debug /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2rbook.dll" /implib:"$(OUTDIR)/m2rbook.lib" /section:.rsrc,w 
LINK32_OBJS= \
	"$(INTDIR)\m2rbook.obj" \
	"$(INTDIR)\m2rbook.res" \
	"$(INTDIR)\m2rdcnt.obj" \
	"$(INTDIR)\m2rdes.obj" \
	"$(INTDIR)\m2rdfnt.obj" \
	"$(INTDIR)\m2rdgi.obj" \
	"$(INTDIR)\m2rdgr.obj" \
	"$(INTDIR)\m2rdgra.obj" \
	"$(INTDIR)\m2rdgrb.obj" \
	"$(INTDIR)\m2rdgt.obj" \
	"$(INTDIR)\m2rdhmk.obj" \
	"$(INTDIR)\m2rdhpj.obj" \
	"$(INTDIR)\m2rdlg.obj" \
	"$(INTDIR)\m2rdmf.obj" \
	"$(INTDIR)\m2rdshb.obj" \
	"$(INTDIR)\m2rdshh.obj" \
	"$(INTDIR)\m2rdshm.obj" \
	"$(INTDIR)\m2rdshs.obj" \
	"$(INTDIR)\m2rdsht.obj" \
	"$(INTDIR)\m2rdtbl.obj" \
	"$(INTDIR)\m2rdtl.obj" \
	"$(INTDIR)\m2rdtpl.obj" \
	"$(INTDIR)\m2rdtxa.obj" \
	"$(INTDIR)\m2rdtxb.obj" \
	"$(INTDIR)\m2rdtxh.obj" \
	"$(INTDIR)\m2rdver.obj" \
	"$(INTDIR)\m2rgr.obj" \
	"$(INTDIR)\m2rgrx.obj" \
	"$(INTDIR)\m2rini.obj" \
	"$(INTDIR)\m2rinit.obj" \
	"$(INTDIR)\m2rmenu.obj" \
	"$(INTDIR)\m2rproc.obj" \
	"$(INTDIR)\m2rproj.obj" \
	"$(INTDIR)\m2rset.obj" \
	"$(INTDIR)\m2rseta.obj" \
	"$(INTDIR)\m2rsetb.obj" \
	"$(INTDIR)\m2rsetc.obj" \
	"$(INTDIR)\m2rsetd.obj" \
	"$(INTDIR)\m2rsete.obj" \
	"$(INTDIR)\m2rsetf.obj" \
	"$(INTDIR)\m2rseth.obj" \
	"$(INTDIR)\m2rsetj.obj" \
	"$(INTDIR)\m2rsetm.obj" \
	"$(INTDIR)\m2rsetn.obj" \
	"$(INTDIR)\m2rsetw.obj" \
	"$(INTDIR)\m2rstr.obj" \
	"$(INTDIR)\m2rsty.obj" \
	"$(INTDIR)\m2rtxt.obj" \
	"$(INTDIR)\m2rutil.obj" \
	"$(INTDIR)\m2rvar.obj" \
	"$(INTDIR)\m2rwrt.obj"

"$(OUTDIR)\m2rbook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Debug\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Debug\m2rbook.dll
SOURCE=$(InputPath)

BuildCmds= \
	cmd /c copy $(TargetPath) d:\FM7.1\fminit\plugins \
	D:\cygwin\bin\touch d:\FM7.1\fminit\plugins\.cache \
	cmd /c del /q d:\FM7.1\fminit\plugins\.cache \
	

"d:\FM7.1\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"d:\FM7.1\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "m2rbook_"
# PROP BASE Intermediate_Dir "m2rbook_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../fdk/Rel50"
# PROP Intermediate_Dir "../fdk/Rel50"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Rel50
INTDIR=.\../fdk/Rel50
# Begin Custom Macros
TargetName=m2rbook
# End Custom Macros

ALL : "$(OUTDIR)\m2rbook.dll" "f:\maker5\fminit\m2rbook.dll"

CLEAN : 
	-@erase "$(INTDIR)\m2rbook.obj"
	-@erase "$(INTDIR)\m2rbook.res"
	-@erase "$(INTDIR)\m2rdcnt.obj"
	-@erase "$(INTDIR)\m2rdes.obj"
	-@erase "$(INTDIR)\m2rdfnt.obj"
	-@erase "$(INTDIR)\m2rdgi.obj"
	-@erase "$(INTDIR)\m2rdgr.obj"
	-@erase "$(INTDIR)\m2rdgra.obj"
	-@erase "$(INTDIR)\m2rdgrb.obj"
	-@erase "$(INTDIR)\m2rdgt.obj"
	-@erase "$(INTDIR)\m2rdhmk.obj"
	-@erase "$(INTDIR)\m2rdhpj.obj"
	-@erase "$(INTDIR)\m2rdlg.obj"
	-@erase "$(INTDIR)\m2rdmf.obj"
	-@erase "$(INTDIR)\m2rdshb.obj"
	-@erase "$(INTDIR)\m2rdshh.obj"
	-@erase "$(INTDIR)\m2rdshm.obj"
	-@erase "$(INTDIR)\m2rdshs.obj"
	-@erase "$(INTDIR)\m2rdsht.obj"
	-@erase "$(INTDIR)\m2rdtbl.obj"
	-@erase "$(INTDIR)\m2rdtl.obj"
	-@erase "$(INTDIR)\m2rdtpl.obj"
	-@erase "$(INTDIR)\m2rdtxa.obj"
	-@erase "$(INTDIR)\m2rdtxb.obj"
	-@erase "$(INTDIR)\m2rdtxh.obj"
	-@erase "$(INTDIR)\m2rdver.obj"
	-@erase "$(INTDIR)\m2rgr.obj"
	-@erase "$(INTDIR)\m2rgrx.obj"
	-@erase "$(INTDIR)\m2rini.obj"
	-@erase "$(INTDIR)\m2rinit.obj"
	-@erase "$(INTDIR)\m2rmenu.obj"
	-@erase "$(INTDIR)\m2rproc.obj"
	-@erase "$(INTDIR)\m2rproj.obj"
	-@erase "$(INTDIR)\m2rset.obj"
	-@erase "$(INTDIR)\m2rseta.obj"
	-@erase "$(INTDIR)\m2rsetb.obj"
	-@erase "$(INTDIR)\m2rsetc.obj"
	-@erase "$(INTDIR)\m2rsetd.obj"
	-@erase "$(INTDIR)\m2rsete.obj"
	-@erase "$(INTDIR)\m2rsetf.obj"
	-@erase "$(INTDIR)\m2rseth.obj"
	-@erase "$(INTDIR)\m2rsetj.obj"
	-@erase "$(INTDIR)\m2rsetm.obj"
	-@erase "$(INTDIR)\m2rsetn.obj"
	-@erase "$(INTDIR)\m2rsetw.obj"
	-@erase "$(INTDIR)\m2rstr.obj"
	-@erase "$(INTDIR)\m2rsty.obj"
	-@erase "$(INTDIR)\m2rtxt.obj"
	-@erase "$(INTDIR)\m2rutil.obj"
	-@erase "$(INTDIR)\m2rvar.obj"
	-@erase "$(INTDIR)\m2rwrt.obj"
	-@erase "$(OUTDIR)\m2rbook.dll"
	-@erase "$(OUTDIR)\m2rbook.exp"
	-@erase "$(OUTDIR)\m2rbook.lib"
	-@erase "f:\maker5\fminit\m2rbook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /FR /YX"m2rbook.h" /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FM50VER" /YX"m2rbook.h" /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "M2RBOOK" /D "FM50VER" /Fp"$(INTDIR)/m2rbook.pch" /YX"m2rbook.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Rel50/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2rbook.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2rbook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk50.lib api50.lib fmdbms50.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk50.lib api50.lib fmdbms50.lib /nologo /subsystem:windows /dll\
 /incremental:no /pdb:"$(OUTDIR)/m2rbook.pdb" /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2rbook.dll" /implib:"$(OUTDIR)/m2rbook.lib" /section:.rsrc,w 
LINK32_OBJS= \
	"$(INTDIR)\m2rbook.obj" \
	"$(INTDIR)\m2rbook.res" \
	"$(INTDIR)\m2rdcnt.obj" \
	"$(INTDIR)\m2rdes.obj" \
	"$(INTDIR)\m2rdfnt.obj" \
	"$(INTDIR)\m2rdgi.obj" \
	"$(INTDIR)\m2rdgr.obj" \
	"$(INTDIR)\m2rdgra.obj" \
	"$(INTDIR)\m2rdgrb.obj" \
	"$(INTDIR)\m2rdgt.obj" \
	"$(INTDIR)\m2rdhmk.obj" \
	"$(INTDIR)\m2rdhpj.obj" \
	"$(INTDIR)\m2rdlg.obj" \
	"$(INTDIR)\m2rdmf.obj" \
	"$(INTDIR)\m2rdshb.obj" \
	"$(INTDIR)\m2rdshh.obj" \
	"$(INTDIR)\m2rdshm.obj" \
	"$(INTDIR)\m2rdshs.obj" \
	"$(INTDIR)\m2rdsht.obj" \
	"$(INTDIR)\m2rdtbl.obj" \
	"$(INTDIR)\m2rdtl.obj" \
	"$(INTDIR)\m2rdtpl.obj" \
	"$(INTDIR)\m2rdtxa.obj" \
	"$(INTDIR)\m2rdtxb.obj" \
	"$(INTDIR)\m2rdtxh.obj" \
	"$(INTDIR)\m2rdver.obj" \
	"$(INTDIR)\m2rgr.obj" \
	"$(INTDIR)\m2rgrx.obj" \
	"$(INTDIR)\m2rini.obj" \
	"$(INTDIR)\m2rinit.obj" \
	"$(INTDIR)\m2rmenu.obj" \
	"$(INTDIR)\m2rproc.obj" \
	"$(INTDIR)\m2rproj.obj" \
	"$(INTDIR)\m2rset.obj" \
	"$(INTDIR)\m2rseta.obj" \
	"$(INTDIR)\m2rsetb.obj" \
	"$(INTDIR)\m2rsetc.obj" \
	"$(INTDIR)\m2rsetd.obj" \
	"$(INTDIR)\m2rsete.obj" \
	"$(INTDIR)\m2rsetf.obj" \
	"$(INTDIR)\m2rseth.obj" \
	"$(INTDIR)\m2rsetj.obj" \
	"$(INTDIR)\m2rsetm.obj" \
	"$(INTDIR)\m2rsetn.obj" \
	"$(INTDIR)\m2rsetw.obj" \
	"$(INTDIR)\m2rstr.obj" \
	"$(INTDIR)\m2rsty.obj" \
	"$(INTDIR)\m2rtxt.obj" \
	"$(INTDIR)\m2rutil.obj" \
	"$(INTDIR)\m2rvar.obj" \
	"$(INTDIR)\m2rwrt.obj"

"$(OUTDIR)\m2rbook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Rel50\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Rel50\m2rbook.dll
SOURCE=$(InputPath)

"f:\maker5\fminit\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) f:\maker5\fminit

# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "m2rbook0"
# PROP BASE Intermediate_Dir "m2rbook0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../fdk/Deb50"
# PROP Intermediate_Dir "../fdk/Deb50"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Deb50
INTDIR=.\../fdk/Deb50
# Begin Custom Macros
TargetName=m2rbook
# End Custom Macros

ALL : "$(OUTDIR)\m2rbook.dll" "f:\maker5\fminit\m2rbook.dll"

CLEAN : 
	-@erase "$(INTDIR)\m2rbook.obj"
	-@erase "$(INTDIR)\m2rbook.res"
	-@erase "$(INTDIR)\m2rdcnt.obj"
	-@erase "$(INTDIR)\m2rdes.obj"
	-@erase "$(INTDIR)\m2rdfnt.obj"
	-@erase "$(INTDIR)\m2rdgi.obj"
	-@erase "$(INTDIR)\m2rdgr.obj"
	-@erase "$(INTDIR)\m2rdgra.obj"
	-@erase "$(INTDIR)\m2rdgrb.obj"
	-@erase "$(INTDIR)\m2rdgt.obj"
	-@erase "$(INTDIR)\m2rdhmk.obj"
	-@erase "$(INTDIR)\m2rdhpj.obj"
	-@erase "$(INTDIR)\m2rdlg.obj"
	-@erase "$(INTDIR)\m2rdmf.obj"
	-@erase "$(INTDIR)\m2rdshb.obj"
	-@erase "$(INTDIR)\m2rdshh.obj"
	-@erase "$(INTDIR)\m2rdshm.obj"
	-@erase "$(INTDIR)\m2rdshs.obj"
	-@erase "$(INTDIR)\m2rdsht.obj"
	-@erase "$(INTDIR)\m2rdtbl.obj"
	-@erase "$(INTDIR)\m2rdtl.obj"
	-@erase "$(INTDIR)\m2rdtpl.obj"
	-@erase "$(INTDIR)\m2rdtxa.obj"
	-@erase "$(INTDIR)\m2rdtxb.obj"
	-@erase "$(INTDIR)\m2rdtxh.obj"
	-@erase "$(INTDIR)\m2rdver.obj"
	-@erase "$(INTDIR)\m2rgr.obj"
	-@erase "$(INTDIR)\m2rgrx.obj"
	-@erase "$(INTDIR)\m2rini.obj"
	-@erase "$(INTDIR)\m2rinit.obj"
	-@erase "$(INTDIR)\m2rmenu.obj"
	-@erase "$(INTDIR)\m2rproc.obj"
	-@erase "$(INTDIR)\m2rproj.obj"
	-@erase "$(INTDIR)\m2rset.obj"
	-@erase "$(INTDIR)\m2rseta.obj"
	-@erase "$(INTDIR)\m2rsetb.obj"
	-@erase "$(INTDIR)\m2rsetc.obj"
	-@erase "$(INTDIR)\m2rsetd.obj"
	-@erase "$(INTDIR)\m2rsete.obj"
	-@erase "$(INTDIR)\m2rsetf.obj"
	-@erase "$(INTDIR)\m2rseth.obj"
	-@erase "$(INTDIR)\m2rsetj.obj"
	-@erase "$(INTDIR)\m2rsetm.obj"
	-@erase "$(INTDIR)\m2rsetn.obj"
	-@erase "$(INTDIR)\m2rsetw.obj"
	-@erase "$(INTDIR)\m2rstr.obj"
	-@erase "$(INTDIR)\m2rsty.obj"
	-@erase "$(INTDIR)\m2rtxt.obj"
	-@erase "$(INTDIR)\m2rutil.obj"
	-@erase "$(INTDIR)\m2rvar.obj"
	-@erase "$(INTDIR)\m2rwrt.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\m2rbook.dll"
	-@erase "$(OUTDIR)\m2rbook.exp"
	-@erase "$(OUTDIR)\m2rbook.ilk"
	-@erase "$(OUTDIR)\m2rbook.lib"
	-@erase "$(OUTDIR)\m2rbook.pdb"
	-@erase "f:\maker5\fminit\m2rbook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /D "FM50VER" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "M2RBOOK" /D "FM50VER" /Fp"$(INTDIR)/m2rbook.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Deb50/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2rbook.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2rbook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk50.lib api50.lib fmdbms50.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk50.lib api50.lib fmdbms50.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2rbook.pdb" /debug /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2rbook.dll" /implib:"$(OUTDIR)/m2rbook.lib" /section:.rsrc,w 
LINK32_OBJS= \
	"$(INTDIR)\m2rbook.obj" \
	"$(INTDIR)\m2rbook.res" \
	"$(INTDIR)\m2rdcnt.obj" \
	"$(INTDIR)\m2rdes.obj" \
	"$(INTDIR)\m2rdfnt.obj" \
	"$(INTDIR)\m2rdgi.obj" \
	"$(INTDIR)\m2rdgr.obj" \
	"$(INTDIR)\m2rdgra.obj" \
	"$(INTDIR)\m2rdgrb.obj" \
	"$(INTDIR)\m2rdgt.obj" \
	"$(INTDIR)\m2rdhmk.obj" \
	"$(INTDIR)\m2rdhpj.obj" \
	"$(INTDIR)\m2rdlg.obj" \
	"$(INTDIR)\m2rdmf.obj" \
	"$(INTDIR)\m2rdshb.obj" \
	"$(INTDIR)\m2rdshh.obj" \
	"$(INTDIR)\m2rdshm.obj" \
	"$(INTDIR)\m2rdshs.obj" \
	"$(INTDIR)\m2rdsht.obj" \
	"$(INTDIR)\m2rdtbl.obj" \
	"$(INTDIR)\m2rdtl.obj" \
	"$(INTDIR)\m2rdtpl.obj" \
	"$(INTDIR)\m2rdtxa.obj" \
	"$(INTDIR)\m2rdtxb.obj" \
	"$(INTDIR)\m2rdtxh.obj" \
	"$(INTDIR)\m2rdver.obj" \
	"$(INTDIR)\m2rgr.obj" \
	"$(INTDIR)\m2rgrx.obj" \
	"$(INTDIR)\m2rini.obj" \
	"$(INTDIR)\m2rinit.obj" \
	"$(INTDIR)\m2rmenu.obj" \
	"$(INTDIR)\m2rproc.obj" \
	"$(INTDIR)\m2rproj.obj" \
	"$(INTDIR)\m2rset.obj" \
	"$(INTDIR)\m2rseta.obj" \
	"$(INTDIR)\m2rsetb.obj" \
	"$(INTDIR)\m2rsetc.obj" \
	"$(INTDIR)\m2rsetd.obj" \
	"$(INTDIR)\m2rsete.obj" \
	"$(INTDIR)\m2rsetf.obj" \
	"$(INTDIR)\m2rseth.obj" \
	"$(INTDIR)\m2rsetj.obj" \
	"$(INTDIR)\m2rsetm.obj" \
	"$(INTDIR)\m2rsetn.obj" \
	"$(INTDIR)\m2rsetw.obj" \
	"$(INTDIR)\m2rstr.obj" \
	"$(INTDIR)\m2rsty.obj" \
	"$(INTDIR)\m2rtxt.obj" \
	"$(INTDIR)\m2rutil.obj" \
	"$(INTDIR)\m2rvar.obj" \
	"$(INTDIR)\m2rwrt.obj"

"$(OUTDIR)\m2rbook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Deb50\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Deb50\m2rbook.dll
SOURCE=$(InputPath)

"f:\maker5\fminit\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(TargetPath) f:\maker5\fminit

# End Custom Build

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "m2rbook_"
# PROP BASE Intermediate_Dir "m2rbook_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../fdk/Debug"
# PROP Intermediate_Dir "../fdk/Debug"
# PROP Target_Dir ""
OUTDIR=.\../fdk/Debug
INTDIR=.\../fdk/Debug
# Begin Custom Macros
TargetName=m2rbook
# End Custom Macros

ALL : "$(OUTDIR)\m2rbook.dll" "x:\fm556\fminit\plugins\m2rbook.dll"\
 "x:\fm556\fminit\plugins\.cache" "s:\Frame6\fminit\plugins\m2rbook.dll"\
 "s:\Frame6\fminit\plugins\.cache"

CLEAN : 
	-@erase "$(INTDIR)\m2rbook.obj"
	-@erase "$(INTDIR)\m2rbook.res"
	-@erase "$(INTDIR)\m2rdcnt.obj"
	-@erase "$(INTDIR)\m2rdes.obj"
	-@erase "$(INTDIR)\m2rdfnt.obj"
	-@erase "$(INTDIR)\m2rdgi.obj"
	-@erase "$(INTDIR)\m2rdgr.obj"
	-@erase "$(INTDIR)\m2rdgra.obj"
	-@erase "$(INTDIR)\m2rdgrb.obj"
	-@erase "$(INTDIR)\m2rdgt.obj"
	-@erase "$(INTDIR)\m2rdhmk.obj"
	-@erase "$(INTDIR)\m2rdhpj.obj"
	-@erase "$(INTDIR)\m2rdlg.obj"
	-@erase "$(INTDIR)\m2rdmf.obj"
	-@erase "$(INTDIR)\m2rdshb.obj"
	-@erase "$(INTDIR)\m2rdshh.obj"
	-@erase "$(INTDIR)\m2rdshm.obj"
	-@erase "$(INTDIR)\m2rdshs.obj"
	-@erase "$(INTDIR)\m2rdsht.obj"
	-@erase "$(INTDIR)\m2rdtbl.obj"
	-@erase "$(INTDIR)\m2rdtl.obj"
	-@erase "$(INTDIR)\m2rdtpl.obj"
	-@erase "$(INTDIR)\m2rdtxa.obj"
	-@erase "$(INTDIR)\m2rdtxb.obj"
	-@erase "$(INTDIR)\m2rdtxh.obj"
	-@erase "$(INTDIR)\m2rdver.obj"
	-@erase "$(INTDIR)\m2rgr.obj"
	-@erase "$(INTDIR)\m2rgrx.obj"
	-@erase "$(INTDIR)\m2rini.obj"
	-@erase "$(INTDIR)\m2rinit.obj"
	-@erase "$(INTDIR)\m2rmenu.obj"
	-@erase "$(INTDIR)\m2rproc.obj"
	-@erase "$(INTDIR)\m2rproj.obj"
	-@erase "$(INTDIR)\m2rset.obj"
	-@erase "$(INTDIR)\m2rseta.obj"
	-@erase "$(INTDIR)\m2rsetb.obj"
	-@erase "$(INTDIR)\m2rsetc.obj"
	-@erase "$(INTDIR)\m2rsetd.obj"
	-@erase "$(INTDIR)\m2rsete.obj"
	-@erase "$(INTDIR)\m2rsetf.obj"
	-@erase "$(INTDIR)\m2rseth.obj"
	-@erase "$(INTDIR)\m2rsetj.obj"
	-@erase "$(INTDIR)\m2rsetm.obj"
	-@erase "$(INTDIR)\m2rsetn.obj"
	-@erase "$(INTDIR)\m2rsetw.obj"
	-@erase "$(INTDIR)\m2rstr.obj"
	-@erase "$(INTDIR)\m2rsty.obj"
	-@erase "$(INTDIR)\m2rtxt.obj"
	-@erase "$(INTDIR)\m2rutil.obj"
	-@erase "$(INTDIR)\m2rvar.obj"
	-@erase "$(INTDIR)\m2rwrt.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\m2rbook.dll"
	-@erase "$(OUTDIR)\m2rbook.exp"
	-@erase "$(OUTDIR)\m2rbook.ilk"
	-@erase "$(OUTDIR)\m2rbook.lib"
	-@erase "$(OUTDIR)\m2rbook.pdb"
	-@erase "s:\Frame6\fminit\plugins\.cache"
	-@erase "s:\Frame6\fminit\plugins\m2rbook.dll"
	-@erase "x:\fm556\fminit\plugins\.cache"
	-@erase "x:\fm556\fminit\plugins\m2rbook.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "M2RBOOK" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "M2RBOOK" /Fp"$(INTDIR)/m2rbook.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../fdk/Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/m2rbook.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/m2rbook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /section:.rsrc,w
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib fdk.lib api.lib fmdbms32.lib /nologo /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)/m2rbook.pdb" /debug /machine:I386\
 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib"\
 /out:"$(OUTDIR)/m2rbook.dll" /implib:"$(OUTDIR)/m2rbook.lib" /section:.rsrc,w 
LINK32_OBJS= \
	"$(INTDIR)\m2rbook.obj" \
	"$(INTDIR)\m2rbook.res" \
	"$(INTDIR)\m2rdcnt.obj" \
	"$(INTDIR)\m2rdes.obj" \
	"$(INTDIR)\m2rdfnt.obj" \
	"$(INTDIR)\m2rdgi.obj" \
	"$(INTDIR)\m2rdgr.obj" \
	"$(INTDIR)\m2rdgra.obj" \
	"$(INTDIR)\m2rdgrb.obj" \
	"$(INTDIR)\m2rdgt.obj" \
	"$(INTDIR)\m2rdhmk.obj" \
	"$(INTDIR)\m2rdhpj.obj" \
	"$(INTDIR)\m2rdlg.obj" \
	"$(INTDIR)\m2rdmf.obj" \
	"$(INTDIR)\m2rdshb.obj" \
	"$(INTDIR)\m2rdshh.obj" \
	"$(INTDIR)\m2rdshm.obj" \
	"$(INTDIR)\m2rdshs.obj" \
	"$(INTDIR)\m2rdsht.obj" \
	"$(INTDIR)\m2rdtbl.obj" \
	"$(INTDIR)\m2rdtl.obj" \
	"$(INTDIR)\m2rdtpl.obj" \
	"$(INTDIR)\m2rdtxa.obj" \
	"$(INTDIR)\m2rdtxb.obj" \
	"$(INTDIR)\m2rdtxh.obj" \
	"$(INTDIR)\m2rdver.obj" \
	"$(INTDIR)\m2rgr.obj" \
	"$(INTDIR)\m2rgrx.obj" \
	"$(INTDIR)\m2rini.obj" \
	"$(INTDIR)\m2rinit.obj" \
	"$(INTDIR)\m2rmenu.obj" \
	"$(INTDIR)\m2rproc.obj" \
	"$(INTDIR)\m2rproj.obj" \
	"$(INTDIR)\m2rset.obj" \
	"$(INTDIR)\m2rseta.obj" \
	"$(INTDIR)\m2rsetb.obj" \
	"$(INTDIR)\m2rsetc.obj" \
	"$(INTDIR)\m2rsetd.obj" \
	"$(INTDIR)\m2rsete.obj" \
	"$(INTDIR)\m2rsetf.obj" \
	"$(INTDIR)\m2rseth.obj" \
	"$(INTDIR)\m2rsetj.obj" \
	"$(INTDIR)\m2rsetm.obj" \
	"$(INTDIR)\m2rsetn.obj" \
	"$(INTDIR)\m2rsetw.obj" \
	"$(INTDIR)\m2rstr.obj" \
	"$(INTDIR)\m2rsty.obj" \
	"$(INTDIR)\m2rtxt.obj" \
	"$(INTDIR)\m2rutil.obj" \
	"$(INTDIR)\m2rvar.obj" \
	"$(INTDIR)\m2rwrt.obj"

"$(OUTDIR)\m2rbook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying to FrameMaker directory...
TargetPath=\cvs\fdk\Debug\m2rbook.dll
TargetName=m2rbook
InputPath=\cvs\fdk\Debug\m2rbook.dll
SOURCE=$(InputPath)

BuildCmds= \
	copy $(TargetPath) x:\fm556\fminit\plugins \
	del x:\fm556\fminit\plugins\.cache \
	copy $(TargetPath) s:\Frame6\fminit\plugins \
	del s:\Frame6\fminit\plugins\.cache \
	

"x:\fm556\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"x:\fm556\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"s:\Frame6\fminit\plugins\$(TargetName).dll" : $(SOURCE) "$(INTDIR)"\
 "$(OUTDIR)"
   $(BuildCmds)

"s:\Frame6\fminit\plugins\.cache" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
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

# Name "m2rbook - Win32 Release"
# Name "m2rbook - Win32 Debug"
# Name "m2rbook - Win32 Rel50"
# Name "m2rbook - Win32 Deb50"
# Name "m2rbook - Win32 Debug FM5"

!IF  "$(CFG)" == "m2rbook - Win32 Release"

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\m2rbook.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RBO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rbook.obj" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"

"$(INTDIR)\m2rbook.sbr" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RBO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rbook.obj" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"

"$(INTDIR)\m2rbook.sbr" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RBO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rbook.obj" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RBO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rbook.obj" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RBO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rbook.obj" : $(SOURCE) $(DEP_CPP_M2RBO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rbook.rc
DEP_RSC_M2RBOO=\
	".\m2rdcnt.dlg"\
	".\m2rdcnt.xdi"\
	".\m2rdfnt.dlg"\
	".\m2rdfnt.xdi"\
	".\m2rdgi.dlg"\
	".\m2rdgi.xdi"\
	".\m2rdgr.dlg"\
	".\m2rdgr.xdi"\
	".\m2rdgra.dlg"\
	".\m2rdgra.xdi"\
	".\m2rdgrb.dlg"\
	".\m2rdgrb.xdi"\
	".\m2rdgt.dlg"\
	".\m2rdgt.xdi"\
	".\m2rdhmk.dlg"\
	".\m2rdhmk.xdi"\
	".\m2rdhpj.dlg"\
	".\m2rdhpj.xdi"\
	".\m2rdmf.dlg"\
	".\m2rdmf.xdi"\
	".\m2rdshb.dlg"\
	".\m2rdshb.xdi"\
	".\m2rdshh.dlg"\
	".\m2rdshh.xdi"\
	".\m2rdshm.dlg"\
	".\m2rdshm.xdi"\
	".\m2rdshs.dlg"\
	".\m2rdshs.xdi"\
	".\m2rdsht.dlg"\
	".\m2rdsht.xdi"\
	".\m2rdtbl.dlg"\
	".\m2rdtbl.xdi"\
	".\m2rdtl.dlg"\
	".\m2rdtl.xdi"\
	".\m2rdtpl.dlg"\
	".\m2rdtpl.xdi"\
	".\m2rdtxa.dlg"\
	".\m2rdtxa.xdi"\
	".\m2rdtxb.dlg"\
	".\m2rdtxb.xdi"\
	".\m2rdtxh.dlg"\
	".\m2rdtxh.xdi"\
	".\m2rdver.dlg"\
	".\m2rdver.xdi"\
	".\m2rinit.dlg"\
	".\m2rinit.xdi"\
	".\m2rproj.dlg"\
	".\m2rproj.xdi"\
	".\m2rseta.dlg"\
	".\m2rseta.xdi"\
	".\m2rsetb.dlg"\
	".\m2rsetb.xdi"\
	".\m2rsetc.dlg"\
	".\m2rsetc.xdi"\
	".\m2rsetd.dlg"\
	".\m2rsetd.xdi"\
	".\m2rsete.dlg"\
	".\m2rsete.xdi"\
	".\m2rsetf.dlg"\
	".\m2rsetf.xdi"\
	".\m2rseth.dlg"\
	".\m2rseth.xdi"\
	".\m2rsetj.dlg"\
	".\m2rsetj.xdi"\
	".\m2rsetm.dlg"\
	".\m2rsetm.xdi"\
	".\m2rsetn.dlg"\
	".\m2rsetn.xdi"\
	".\m2rsetw.dlg"\
	".\m2rsetw.xdi"\
	".\m2rvar.dlg"\
	".\m2rvar.xdi"\
	

!IF  "$(CFG)" == "m2rbook - Win32 Release"


"$(INTDIR)\m2rbook.res" : $(SOURCE) $(DEP_RSC_M2RBOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"


"$(INTDIR)\m2rbook.res" : $(SOURCE) $(DEP_RSC_M2RBOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"


"$(INTDIR)\m2rbook.res" : $(SOURCE) $(DEP_RSC_M2RBOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"


"$(INTDIR)\m2rbook.res" : $(SOURCE) $(DEP_RSC_M2RBOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"


"$(INTDIR)\m2rbook.res" : $(SOURCE) $(DEP_RSC_M2RBOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rutil.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RUT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rutil.obj" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"

"$(INTDIR)\m2rutil.sbr" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RUT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rutil.obj" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"

"$(INTDIR)\m2rutil.sbr" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RUT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rutil.obj" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RUT=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rutil.obj" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RUT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rutil.obj" : $(SOURCE) $(DEP_CPP_M2RUT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rstr.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RST=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\fstrlist.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rstr.obj" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"

"$(INTDIR)\m2rstr.sbr" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RST=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\fstrlist.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rstr.obj" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"

"$(INTDIR)\m2rstr.sbr" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RST=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\fstrlist.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rstr.obj" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RST=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\fstrlist.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rstr.obj" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RST=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\fstrlist.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rstr.obj" : $(SOURCE) $(DEP_CPP_M2RST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rproj.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RPR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproj.obj" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"

"$(INTDIR)\m2rproj.sbr" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RPR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproj.obj" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"

"$(INTDIR)\m2rproj.sbr" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RPR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproj.obj" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RPR=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproj.obj" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RPR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproj.obj" : $(SOURCE) $(DEP_CPP_M2RPR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rini.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RIN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rini.obj" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"

"$(INTDIR)\m2rini.sbr" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RIN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rini.obj" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"

"$(INTDIR)\m2rini.sbr" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RIN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rini.obj" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RIN=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rini.obj" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RIN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rini.obj" : $(SOURCE) $(DEP_CPP_M2RIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rwrt.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RWR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rwrt.obj" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"

"$(INTDIR)\m2rwrt.sbr" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RWR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rwrt.obj" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"

"$(INTDIR)\m2rwrt.sbr" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RWR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rwrt.obj" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RWR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rwrt.obj" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RWR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rwrt.obj" : $(SOURCE) $(DEP_CPP_M2RWR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rgr.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgr.obj" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"

"$(INTDIR)\m2rgr.sbr" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgr.obj" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"

"$(INTDIR)\m2rgr.sbr" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgr.obj" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RGR=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgr.obj" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgr.obj" : $(SOURCE) $(DEP_CPP_M2RGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rproc.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RPRO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproc.obj" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"

"$(INTDIR)\m2rproc.sbr" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RPRO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproc.obj" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"

"$(INTDIR)\m2rproc.sbr" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RPRO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproc.obj" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RPRO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	
NODEP_CPP_M2RPRO=\
	".\PT_htm"\
	

"$(INTDIR)\m2rproc.obj" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RPRO=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rproc.obj" : $(SOURCE) $(DEP_CPP_M2RPRO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rseth.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseth.obj" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"

"$(INTDIR)\m2rseth.sbr" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseth.obj" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"

"$(INTDIR)\m2rseth.sbr" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseth.obj" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSE=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseth.obj" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseth.obj" : $(SOURCE) $(DEP_CPP_M2RSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetw.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSET=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetw.obj" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"

"$(INTDIR)\m2rsetw.sbr" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSET=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetw.obj" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"

"$(INTDIR)\m2rsetw.sbr" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSET=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetw.obj" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSET=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetw.obj" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSET=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetw.obj" : $(SOURCE) $(DEP_CPP_M2RSET) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rinit.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RINI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rinit.obj" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"

"$(INTDIR)\m2rinit.sbr" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RINI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rinit.obj" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"

"$(INTDIR)\m2rinit.sbr" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RINI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rinit.obj" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RINI=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rinit.obj" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RINI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rinit.obj" : $(SOURCE) $(DEP_CPP_M2RINI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetn.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetn.obj" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"

"$(INTDIR)\m2rsetn.sbr" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetn.obj" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"

"$(INTDIR)\m2rsetn.sbr" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetn.obj" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETN=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetn.obj" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETN=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetn.obj" : $(SOURCE) $(DEP_CPP_M2RSETN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdes.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdes.obj" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"

"$(INTDIR)\m2rdes.sbr" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdes.obj" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"

"$(INTDIR)\m2rdes.sbr" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdes.obj" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDE=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdes.obj" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdes.obj" : $(SOURCE) $(DEP_CPP_M2RDE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsty.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSTY=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsty.obj" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"

"$(INTDIR)\m2rsty.sbr" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSTY=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsty.obj" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"

"$(INTDIR)\m2rsty.sbr" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSTY=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsty.obj" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSTY=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsty.obj" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSTY=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsty.obj" : $(SOURCE) $(DEP_CPP_M2RSTY) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rtxt.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rtxt.obj" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"

"$(INTDIR)\m2rtxt.sbr" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rtxt.obj" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"

"$(INTDIR)\m2rtxt.sbr" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rtxt.obj" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RTX=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rtxt.obj" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rtxt.obj" : $(SOURCE) $(DEP_CPP_M2RTX) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtxb.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxb.obj" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"

"$(INTDIR)\m2rdtxb.sbr" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxb.obj" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"

"$(INTDIR)\m2rdtxb.sbr" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxb.obj" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDT=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxb.obj" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxb.obj" : $(SOURCE) $(DEP_CPP_M2RDT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtl.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDTL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtl.obj" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"

"$(INTDIR)\m2rdtl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDTL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtl.obj" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"

"$(INTDIR)\m2rdtl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDTL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtl.obj" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDTL=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtl.obj" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDTL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtl.obj" : $(SOURCE) $(DEP_CPP_M2RDTL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdsht.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDS=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdsht.obj" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"

"$(INTDIR)\m2rdsht.sbr" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDS=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdsht.obj" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"

"$(INTDIR)\m2rdsht.sbr" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDS=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdsht.obj" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDS=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdsht.obj" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDS=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdsht.obj" : $(SOURCE) $(DEP_CPP_M2RDS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdshs.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDSH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshs.obj" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"

"$(INTDIR)\m2rdshs.sbr" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDSH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshs.obj" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"

"$(INTDIR)\m2rdshs.sbr" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDSH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshs.obj" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDSH=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshs.obj" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDSH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshs.obj" : $(SOURCE) $(DEP_CPP_M2RDSH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdshm.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDSHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshm.obj" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"

"$(INTDIR)\m2rdshm.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDSHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshm.obj" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"

"$(INTDIR)\m2rdshm.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDSHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshm.obj" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDSHM=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshm.obj" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDSHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshm.obj" : $(SOURCE) $(DEP_CPP_M2RDSHM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdshh.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDSHH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshh.obj" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"

"$(INTDIR)\m2rdshh.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDSHH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshh.obj" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"

"$(INTDIR)\m2rdshh.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDSHH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshh.obj" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDSHH=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshh.obj" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDSHH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshh.obj" : $(SOURCE) $(DEP_CPP_M2RDSHH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdshb.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDSHB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshb.obj" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"

"$(INTDIR)\m2rdshb.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDSHB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshb.obj" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"

"$(INTDIR)\m2rdshb.sbr" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDSHB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshb.obj" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDSHB=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshb.obj" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDSHB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdshb.obj" : $(SOURCE) $(DEP_CPP_M2RDSHB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdgr.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDG=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgr.obj" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"

"$(INTDIR)\m2rdgr.sbr" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDG=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgr.obj" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"

"$(INTDIR)\m2rdgr.sbr" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDG=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgr.obj" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDG=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgr.obj" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDG=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgr.obj" : $(SOURCE) $(DEP_CPP_M2RDG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdgi.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDGI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgi.obj" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"

"$(INTDIR)\m2rdgi.sbr" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDGI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgi.obj" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"

"$(INTDIR)\m2rdgi.sbr" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDGI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgi.obj" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDGI=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgi.obj" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDGI=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgi.obj" : $(SOURCE) $(DEP_CPP_M2RDGI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdlg.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdlg.obj" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"

"$(INTDIR)\m2rdlg.sbr" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdlg.obj" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"

"$(INTDIR)\m2rdlg.sbr" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdlg.obj" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDL=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdlg.obj" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDL=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdlg.obj" : $(SOURCE) $(DEP_CPP_M2RDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rgrx.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RGRX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgrx.obj" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"

"$(INTDIR)\m2rgrx.sbr" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RGRX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgrx.obj" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"

"$(INTDIR)\m2rgrx.sbr" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RGRX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgrx.obj" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RGRX=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgrx.obj" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RGRX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rgrx.obj" : $(SOURCE) $(DEP_CPP_M2RGRX) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdgt.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDGT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgt.obj" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"

"$(INTDIR)\m2rdgt.sbr" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDGT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgt.obj" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"

"$(INTDIR)\m2rdgt.sbr" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDGT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgt.obj" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDGT=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgt.obj" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDGT=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgt.obj" : $(SOURCE) $(DEP_CPP_M2RDGT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtbl.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDTB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtbl.obj" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"

"$(INTDIR)\m2rdtbl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDTB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtbl.obj" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"

"$(INTDIR)\m2rdtbl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDTB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtbl.obj" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDTB=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtbl.obj" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDTB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtbl.obj" : $(SOURCE) $(DEP_CPP_M2RDTB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdfnt.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdfnt.obj" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"

"$(INTDIR)\m2rdfnt.sbr" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdfnt.obj" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"

"$(INTDIR)\m2rdfnt.sbr" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdfnt.obj" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDF=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdfnt.obj" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdfnt.obj" : $(SOURCE) $(DEP_CPP_M2RDF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtxh.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxh.obj" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"

"$(INTDIR)\m2rdtxh.sbr" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxh.obj" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"

"$(INTDIR)\m2rdtxh.sbr" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxh.obj" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDTX=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxh.obj" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDTX=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxh.obj" : $(SOURCE) $(DEP_CPP_M2RDTX) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtxa.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDTXA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxa.obj" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"

"$(INTDIR)\m2rdtxa.sbr" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDTXA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxa.obj" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"

"$(INTDIR)\m2rdtxa.sbr" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDTXA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxa.obj" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDTXA=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxa.obj" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDTXA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtxa.obj" : $(SOURCE) $(DEP_CPP_M2RDTXA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdver.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDV=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdver.obj" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"

"$(INTDIR)\m2rdver.sbr" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDV=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdver.obj" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"

"$(INTDIR)\m2rdver.sbr" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDV=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdver.obj" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDV=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdver.obj" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDV=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdver.obj" : $(SOURCE) $(DEP_CPP_M2RDV) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdcnt.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdcnt.obj" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"

"$(INTDIR)\m2rdcnt.sbr" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdcnt.obj" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"

"$(INTDIR)\m2rdcnt.sbr" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdcnt.obj" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDC=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdcnt.obj" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdcnt.obj" : $(SOURCE) $(DEP_CPP_M2RDC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdhpj.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhpj.obj" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"

"$(INTDIR)\m2rdhpj.sbr" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhpj.obj" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"

"$(INTDIR)\m2rdhpj.sbr" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhpj.obj" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDH=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhpj.obj" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDH=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhpj.obj" : $(SOURCE) $(DEP_CPP_M2RDH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdtpl.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDTP=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtpl.obj" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"

"$(INTDIR)\m2rdtpl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDTP=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtpl.obj" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"

"$(INTDIR)\m2rdtpl.sbr" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDTP=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtpl.obj" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDTP=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtpl.obj" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDTP=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdtpl.obj" : $(SOURCE) $(DEP_CPP_M2RDTP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdhmk.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhmk.obj" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"

"$(INTDIR)\m2rdhmk.sbr" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhmk.obj" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"

"$(INTDIR)\m2rdhmk.sbr" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhmk.obj" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDHM=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhmk.obj" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDHM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdhmk.obj" : $(SOURCE) $(DEP_CPP_M2RDHM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdmf.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdmf.obj" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"

"$(INTDIR)\m2rdmf.sbr" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdmf.obj" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"

"$(INTDIR)\m2rdmf.sbr" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdmf.obj" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDM=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdmf.obj" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdmf.obj" : $(SOURCE) $(DEP_CPP_M2RDM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdgra.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgra.obj" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"

"$(INTDIR)\m2rdgra.sbr" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgra.obj" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"

"$(INTDIR)\m2rdgra.sbr" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgra.obj" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDGR=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgra.obj" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDGR=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgra.obj" : $(SOURCE) $(DEP_CPP_M2RDGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rdgrb.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RDGRB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgrb.obj" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"

"$(INTDIR)\m2rdgrb.sbr" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RDGRB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgrb.obj" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"

"$(INTDIR)\m2rdgrb.sbr" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RDGRB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgrb.obj" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RDGRB=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgrb.obj" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RDGRB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rdgrb.obj" : $(SOURCE) $(DEP_CPP_M2RDGRB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rvar.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RVA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rvar.obj" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"

"$(INTDIR)\m2rvar.sbr" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RVA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rvar.obj" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"

"$(INTDIR)\m2rvar.sbr" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RVA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rvar.obj" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RVA=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rvar.obj" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RVA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rvar.obj" : $(SOURCE) $(DEP_CPP_M2RVA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetf.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetf.obj" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"

"$(INTDIR)\m2rsetf.sbr" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetf.obj" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"

"$(INTDIR)\m2rsetf.sbr" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetf.obj" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETF=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetf.obj" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETF=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetf.obj" : $(SOURCE) $(DEP_CPP_M2RSETF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rseta.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseta.obj" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"

"$(INTDIR)\m2rseta.sbr" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseta.obj" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"

"$(INTDIR)\m2rseta.sbr" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseta.obj" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETA=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseta.obj" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETA=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rseta.obj" : $(SOURCE) $(DEP_CPP_M2RSETA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rset.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSET_=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rset.obj" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"

"$(INTDIR)\m2rset.sbr" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSET_=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rset.obj" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"

"$(INTDIR)\m2rset.sbr" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSET_=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rset.obj" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSET_=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rset.obj" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSET_=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rset.obj" : $(SOURCE) $(DEP_CPP_M2RSET_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetm.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetm.obj" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"

"$(INTDIR)\m2rsetm.sbr" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetm.obj" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"

"$(INTDIR)\m2rsetm.sbr" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetm.obj" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETM=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetm.obj" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETM=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetm.obj" : $(SOURCE) $(DEP_CPP_M2RSETM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetj.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETJ=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetj.obj" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"

"$(INTDIR)\m2rsetj.sbr" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETJ=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetj.obj" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"

"$(INTDIR)\m2rsetj.sbr" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETJ=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetj.obj" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETJ=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetj.obj" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETJ=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetj.obj" : $(SOURCE) $(DEP_CPP_M2RSETJ) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetc.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetc.obj" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"

"$(INTDIR)\m2rsetc.sbr" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetc.obj" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"

"$(INTDIR)\m2rsetc.sbr" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetc.obj" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETC=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetc.obj" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETC=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetc.obj" : $(SOURCE) $(DEP_CPP_M2RSETC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rmenu.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RME=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rmenu.obj" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"

"$(INTDIR)\m2rmenu.sbr" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RME=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rmenu.obj" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"

"$(INTDIR)\m2rmenu.sbr" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RME=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rmenu.obj" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RME=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rmenu.obj" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RME=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rmenu.obj" : $(SOURCE) $(DEP_CPP_M2RME) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsete.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsete.obj" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"

"$(INTDIR)\m2rsete.sbr" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsete.obj" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"

"$(INTDIR)\m2rsete.sbr" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsete.obj" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETE=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsete.obj" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETE=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsete.obj" : $(SOURCE) $(DEP_CPP_M2RSETE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetd.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETD=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetd.obj" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"

"$(INTDIR)\m2rsetd.sbr" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETD=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetd.obj" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"

"$(INTDIR)\m2rsetd.sbr" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETD=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetd.obj" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETD=\
	"..\..\FDK6\include\f_types.h"\
	".\m2rbook.h"\
	{$(INCLUDE)}"\fapi.h"\
	

"$(INTDIR)\m2rsetd.obj" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETD=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetd.obj" : $(SOURCE) $(DEP_CPP_M2RSETD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2rsetb.c

!IF  "$(CFG)" == "m2rbook - Win32 Release"

DEP_CPP_M2RSETB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetb.obj" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"

"$(INTDIR)\m2rsetb.sbr" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug"

DEP_CPP_M2RSETB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetb.obj" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"

"$(INTDIR)\m2rsetb.sbr" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Rel50"

DEP_CPP_M2RSETB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetb.obj" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Deb50"

DEP_CPP_M2RSETB=\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetb.obj" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "m2rbook - Win32 Debug FM5"

DEP_CPP_M2RSETB=\
	"..\..\FDK6\include\f_local.h"\
	"..\..\FDK6\include\f_types.h"\
	"..\..\FDK6\include\fapidefs.h"\
	"..\..\FDK6\include\fassert.h"\
	"..\..\FDK6\include\fdk_env.h"\
	"..\..\FDK6\include\fpath.h"\
	".\m2rbook.h"\
	".\m2rhids.h"\
	{$(INCLUDE)}"\fapi.h"\
	{$(INCLUDE)}"\fcharmap.h"\
	{$(INCLUDE)}"\fcodes.h"\
	{$(INCLUDE)}"\fdetypes.h"\
	{$(INCLUDE)}"\fmemory.h"\
	{$(INCLUDE)}"\fmetrics.h"\
	{$(INCLUDE)}"\fstrings.h"\
	{$(INCLUDE)}"\futils.h"\
	

"$(INTDIR)\m2rsetb.obj" : $(SOURCE) $(DEP_CPP_M2RSETB) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
