# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=dtd2ini - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to dtd2ini - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dtd2ini - Win32 Release" && "$(CFG)" !=\
 "dtd2ini - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dtd2ini.mak" CFG="dtd2ini - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dtd2ini - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dtd2ini - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "dtd2ini - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dtd2ini - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\GPL\Release"
# PROP Intermediate_Dir "..\GPL\Release"
# PROP Target_Dir ""
OUTDIR=.\..\GPL\Release
INTDIR=.\..\GPL\Release

ALL : "$(OUTDIR)\dtd2ini.exe" "$(OUTDIR)\dtd2ini.bsc"

CLEAN : 
	-@erase "$(INTDIR)\charset.obj"
	-@erase "$(INTDIR)\charset.sbr"
	-@erase "$(INTDIR)\ctype16.obj"
	-@erase "$(INTDIR)\ctype16.sbr"
	-@erase "$(INTDIR)\d2ird.obj"
	-@erase "$(INTDIR)\d2ird.sbr"
	-@erase "$(INTDIR)\dtd.obj"
	-@erase "$(INTDIR)\dtd.sbr"
	-@erase "$(INTDIR)\dtd2ini.obj"
	-@erase "$(INTDIR)\dtd2ini.res"
	-@erase "$(INTDIR)\dtd2ini.sbr"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hash.sbr"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\http.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\namespaces.obj"
	-@erase "$(INTDIR)\namespaces.sbr"
	-@erase "$(INTDIR)\stdio16.obj"
	-@erase "$(INTDIR)\stdio16.sbr"
	-@erase "$(INTDIR)\string16.obj"
	-@erase "$(INTDIR)\string16.sbr"
	-@erase "$(INTDIR)\system.obj"
	-@erase "$(INTDIR)\system.sbr"
	-@erase "$(INTDIR)\url.obj"
	-@erase "$(INTDIR)\url.sbr"
	-@erase "$(INTDIR)\xmlparser.obj"
	-@erase "$(INTDIR)\xmlparser.sbr"
	-@erase "$(OUTDIR)\dtd2ini.bsc"
	-@erase "$(OUTDIR)\dtd2ini.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D CHAR_SIZE=8 /D "_BUILD_STD" /FR /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D\
 CHAR_SIZE=8 /D "_BUILD_STD" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dtd2ini.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\GPL\Release/
CPP_SBRS=.\..\GPL\Release/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dtd2ini.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dtd2ini.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\charset.sbr" \
	"$(INTDIR)\ctype16.sbr" \
	"$(INTDIR)\d2ird.sbr" \
	"$(INTDIR)\dtd.sbr" \
	"$(INTDIR)\dtd2ini.sbr" \
	"$(INTDIR)\hash.sbr" \
	"$(INTDIR)\http.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\namespaces.sbr" \
	"$(INTDIR)\stdio16.sbr" \
	"$(INTDIR)\string16.sbr" \
	"$(INTDIR)\system.sbr" \
	"$(INTDIR)\url.sbr" \
	"$(INTDIR)\xmlparser.sbr"

"$(OUTDIR)\dtd2ini.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WSOCK32.LIB /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib WSOCK32.LIB /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dtd2ini.pdb" /machine:I386 /out:"$(OUTDIR)/dtd2ini.exe" 
LINK32_OBJS= \
	"$(INTDIR)\charset.obj" \
	"$(INTDIR)\ctype16.obj" \
	"$(INTDIR)\d2ird.obj" \
	"$(INTDIR)\dtd.obj" \
	"$(INTDIR)\dtd2ini.obj" \
	"$(INTDIR)\dtd2ini.res" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\namespaces.obj" \
	"$(INTDIR)\stdio16.obj" \
	"$(INTDIR)\string16.obj" \
	"$(INTDIR)\system.obj" \
	"$(INTDIR)\url.obj" \
	"$(INTDIR)\xmlparser.obj"

"$(OUTDIR)\dtd2ini.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dtd2ini - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\GPL\Debug"
# PROP Intermediate_Dir "..\GPL\Debug"
# PROP Target_Dir ""
OUTDIR=.\..\GPL\Debug
INTDIR=.\..\GPL\Debug

ALL : "$(OUTDIR)\dtd2ini.exe" "$(OUTDIR)\dtd2ini.bsc"

CLEAN : 
	-@erase "$(INTDIR)\charset.obj"
	-@erase "$(INTDIR)\charset.sbr"
	-@erase "$(INTDIR)\ctype16.obj"
	-@erase "$(INTDIR)\ctype16.sbr"
	-@erase "$(INTDIR)\d2ird.obj"
	-@erase "$(INTDIR)\d2ird.sbr"
	-@erase "$(INTDIR)\dtd.obj"
	-@erase "$(INTDIR)\dtd.sbr"
	-@erase "$(INTDIR)\dtd2ini.obj"
	-@erase "$(INTDIR)\dtd2ini.res"
	-@erase "$(INTDIR)\dtd2ini.sbr"
	-@erase "$(INTDIR)\hash.obj"
	-@erase "$(INTDIR)\hash.sbr"
	-@erase "$(INTDIR)\http.obj"
	-@erase "$(INTDIR)\http.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\namespaces.obj"
	-@erase "$(INTDIR)\namespaces.sbr"
	-@erase "$(INTDIR)\stdio16.obj"
	-@erase "$(INTDIR)\stdio16.sbr"
	-@erase "$(INTDIR)\string16.obj"
	-@erase "$(INTDIR)\string16.sbr"
	-@erase "$(INTDIR)\system.obj"
	-@erase "$(INTDIR)\system.sbr"
	-@erase "$(INTDIR)\url.obj"
	-@erase "$(INTDIR)\url.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\xmlparser.obj"
	-@erase "$(INTDIR)\xmlparser.sbr"
	-@erase "$(OUTDIR)\dtd2ini.bsc"
	-@erase "$(OUTDIR)\dtd2ini.exe"
	-@erase "$(OUTDIR)\dtd2ini.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D CHAR_SIZE=8 /D "_BUILD_STD" /FR /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE"\
 /D CHAR_SIZE=8 /D "_BUILD_STD" /FR"$(INTDIR)/" /Fp"$(INTDIR)/dtd2ini.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\..\GPL\Debug/
CPP_SBRS=.\..\GPL\Debug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dtd2ini.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dtd2ini.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\charset.sbr" \
	"$(INTDIR)\ctype16.sbr" \
	"$(INTDIR)\d2ird.sbr" \
	"$(INTDIR)\dtd.sbr" \
	"$(INTDIR)\dtd2ini.sbr" \
	"$(INTDIR)\hash.sbr" \
	"$(INTDIR)\http.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\namespaces.sbr" \
	"$(INTDIR)\stdio16.sbr" \
	"$(INTDIR)\string16.sbr" \
	"$(INTDIR)\system.sbr" \
	"$(INTDIR)\url.sbr" \
	"$(INTDIR)\xmlparser.sbr"

"$(OUTDIR)\dtd2ini.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib WSOCK32.LIB /nologo /subsystem:console /incremental:no /debug /machine:I386 /WARN:1
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib WSOCK32.LIB /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dtd2ini.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dtd2ini.exe"\
 /WARN:1 
LINK32_OBJS= \
	"$(INTDIR)\charset.obj" \
	"$(INTDIR)\ctype16.obj" \
	"$(INTDIR)\d2ird.obj" \
	"$(INTDIR)\dtd.obj" \
	"$(INTDIR)\dtd2ini.obj" \
	"$(INTDIR)\dtd2ini.res" \
	"$(INTDIR)\hash.obj" \
	"$(INTDIR)\http.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\namespaces.obj" \
	"$(INTDIR)\stdio16.obj" \
	"$(INTDIR)\string16.obj" \
	"$(INTDIR)\system.obj" \
	"$(INTDIR)\url.obj" \
	"$(INTDIR)\xmlparser.obj"

"$(OUTDIR)\dtd2ini.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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

# Name "dtd2ini - Win32 Release"
# Name "dtd2ini - Win32 Debug"

!IF  "$(CFG)" == "dtd2ini - Win32 Release"

!ELSEIF  "$(CFG)" == "dtd2ini - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dtd.c
DEP_CPP_DTD_C=\
	".\charset.h"\
	".\dtd.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	".\url.h"\
	
NODEP_CPP_DTD_C=\
	".\lt-memory.h"\
	".\nsllib.h"\
	".\zlib.h"\
	

"$(INTDIR)\dtd.obj" : $(SOURCE) $(DEP_CPP_DTD_C) "$(INTDIR)"

"$(INTDIR)\dtd.sbr" : $(SOURCE) $(DEP_CPP_DTD_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\system.c
DEP_CPP_SYSTE=\
	".\charset.h"\
	".\stdio16.h"\
	".\system.h"\
	
NODEP_CPP_SYSTE=\
	".\zlib.h"\
	

"$(INTDIR)\system.obj" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"

"$(INTDIR)\system.sbr" : $(SOURCE) $(DEP_CPP_SYSTE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\string16.c
DEP_CPP_STRIN=\
	".\charset.h"\
	".\ctype16.h"\
	".\string16.h"\
	".\system.h"\
	
NODEP_CPP_STRIN=\
	".\lt-memory.h"\
	

"$(INTDIR)\string16.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"

"$(INTDIR)\string16.sbr" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ctype16.c
DEP_CPP_CTYPE=\
	".\charset.h"\
	".\ctype16.h"\
	".\system.h"\
	
NODEP_CPP_CTYPE=\
	".\lt-defs.h"\
	

"$(INTDIR)\ctype16.obj" : $(SOURCE) $(DEP_CPP_CTYPE) "$(INTDIR)"

"$(INTDIR)\ctype16.sbr" : $(SOURCE) $(DEP_CPP_CTYPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\http.c
DEP_CPP_HTTP_=\
	".\charset.h"\
	".\http.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	".\url.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
NODEP_CPP_HTTP_=\
	".\lt-comment.h"\
	".\lt-defs.h"\
	".\lt-errmsg.h"\
	".\lt-memory.h"\
	".\lt-safe.h"\
	".\nsl-err.h"\
	".\zlib.h"\
	

"$(INTDIR)\http.obj" : $(SOURCE) $(DEP_CPP_HTTP_) "$(INTDIR)"

"$(INTDIR)\http.sbr" : $(SOURCE) $(DEP_CPP_HTTP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdio16.c
DEP_CPP_STDIO=\
	".\charset.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	
NODEP_CPP_STDIO=\
	".\lt-memory.h"\
	".\nsl-err.h"\
	".\zlib.h"\
	

"$(INTDIR)\stdio16.obj" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"

"$(INTDIR)\stdio16.sbr" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\url.c
DEP_CPP_URL_C=\
	".\charset.h"\
	".\http.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	".\url.h"\
	{$(INCLUDE)}"\sys\types.h"\
	
NODEP_CPP_URL_C=\
	".\lt-comment.h"\
	".\lt-defs.h"\
	".\lt-errmsg.h"\
	".\lt-memory.h"\
	".\lt-safe.h"\
	".\nsl-err.h"\
	".\zlib.h"\
	

"$(INTDIR)\url.obj" : $(SOURCE) $(DEP_CPP_URL_C) "$(INTDIR)"

"$(INTDIR)\url.sbr" : $(SOURCE) $(DEP_CPP_URL_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\charset.c
DEP_CPP_CHARS=\
	".\charset.h"\
	".\iso-8859\iso-8859-10"\
	".\iso-8859\iso-8859-11"\
	".\iso-8859\iso-8859-13"\
	".\iso-8859\iso-8859-14"\
	".\iso-8859\iso-8859-15"\
	".\iso-8859\iso-8859-2"\
	".\iso-8859\iso-8859-3"\
	".\iso-8859\iso-8859-4"\
	".\iso-8859\iso-8859-5"\
	".\iso-8859\iso-8859-6"\
	".\iso-8859\iso-8859-7"\
	".\iso-8859\iso-8859-8"\
	".\iso-8859\iso-8859-9"\
	".\string16.h"\
	".\system.h"\
	
NODEP_CPP_CHARS=\
	".\lt-memory.h"\
	

"$(INTDIR)\charset.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"

"$(INTDIR)\charset.sbr" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\namespaces.c
DEP_CPP_NAMES=\
	".\charset.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\string16.h"\
	".\system.h"\
	
NODEP_CPP_NAMES=\
	".\lt-memory.h"\
	".\nsllib.h"\
	

"$(INTDIR)\namespaces.obj" : $(SOURCE) $(DEP_CPP_NAMES) "$(INTDIR)"

"$(INTDIR)\namespaces.sbr" : $(SOURCE) $(DEP_CPP_NAMES) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dtd2ini.c
DEP_CPP_DTD2I=\
	".\charset.h"\
	".\dtd.h"\
	".\dtd2ini.h"\
	".\hash.h"\
	".\input.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\system.h"\
	".\xmlparser.h"\
	
NODEP_CPP_DTD2I=\
	".\lt-hash.h"\
	".\nf16check.h"\
	".\zlib.h"\
	

"$(INTDIR)\dtd2ini.obj" : $(SOURCE) $(DEP_CPP_DTD2I) "$(INTDIR)"

"$(INTDIR)\dtd2ini.sbr" : $(SOURCE) $(DEP_CPP_DTD2I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\xmlparser.c
DEP_CPP_XMLPA=\
	".\charset.h"\
	".\ctype16.h"\
	".\dtd.h"\
	".\hash.h"\
	".\input.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	".\url.h"\
	".\xmlparser.h"\
	
NODEP_CPP_XMLPA=\
	".\lt-hash.h"\
	".\lt-memory.h"\
	".\nf16check.h"\
	".\nsllib.h"\
	".\zlib.h"\
	

"$(INTDIR)\xmlparser.obj" : $(SOURCE) $(DEP_CPP_XMLPA) "$(INTDIR)"

"$(INTDIR)\xmlparser.sbr" : $(SOURCE) $(DEP_CPP_XMLPA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hash.c
DEP_CPP_HASH_=\
	".\hash.h"\
	".\system.h"\
	

"$(INTDIR)\hash.obj" : $(SOURCE) $(DEP_CPP_HASH_) "$(INTDIR)"

"$(INTDIR)\hash.sbr" : $(SOURCE) $(DEP_CPP_HASH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\input.c
DEP_CPP_INPUT=\
	".\charset.h"\
	".\ctype16.h"\
	".\dtd.h"\
	".\input.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\string16.h"\
	".\system.h"\
	".\url.h"\
	
NODEP_CPP_INPUT=\
	".\lt-memory.h"\
	".\nsllib.h"\
	".\zlib.h"\
	

"$(INTDIR)\input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"

"$(INTDIR)\input.sbr" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dtd2ini.rc

"$(INTDIR)\dtd2ini.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\d2ird.c
DEP_CPP_D2IRD=\
	".\charset.h"\
	".\dtd.h"\
	".\dtd2ini.h"\
	".\hash.h"\
	".\input.h"\
	".\namespaces.h"\
	".\rxputil.h"\
	".\stdio16.h"\
	".\system.h"\
	".\xmlparser.h"\
	
NODEP_CPP_D2IRD=\
	".\lt-hash.h"\
	".\nf16check.h"\
	".\zlib.h"\
	

"$(INTDIR)\d2ird.obj" : $(SOURCE) $(DEP_CPP_D2IRD) "$(INTDIR)"

"$(INTDIR)\d2ird.sbr" : $(SOURCE) $(DEP_CPP_D2IRD) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
