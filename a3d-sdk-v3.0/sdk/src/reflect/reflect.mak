# Microsoft Developer Studio Generated NMAKE File, Based on reflect.dsp
!IF "$(CFG)" == ""
CFG=reflect - Win32 Debug
!MESSAGE No configuration specified. Defaulting to reflect - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "reflect - Win32 Release" && "$(CFG)" != "reflect - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "reflect.mak" CFG="reflect - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "reflect - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "reflect - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "reflect - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\bin\reflect.exe"


CLEAN :
	-@erase "$(INTDIR)\reflect.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\bin\reflect.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\reflect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=opengl32.lib glaux.lib glu32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\reflect.pdb" /machine:I386 /out:"../../bin/reflect.exe" /libpath:"../../lib" 
LINK32_OBJS= \
	"$(INTDIR)\reflect.obj"

"..\..\bin\reflect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "reflect - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\bin\reflect.exe"


CLEAN :
	-@erase "$(INTDIR)\reflect.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\reflect.pdb"
	-@erase "..\..\bin\reflect.exe"
	-@erase "..\..\bin\reflect.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\reflect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=opengl32.lib glu32.lib glaux.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\reflect.pdb" /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"../../bin/reflect.exe" /pdbtype:sept /libpath:"../../lib" 
LINK32_OBJS= \
	"$(INTDIR)\reflect.obj"

"..\..\bin\reflect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("reflect.dep")
!INCLUDE "reflect.dep"
!ELSE 
!MESSAGE Warning: cannot find "reflect.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "reflect - Win32 Release" || "$(CFG)" == "reflect - Win32 Debug"
SOURCE=.\reflect.cpp

"$(INTDIR)\reflect.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

