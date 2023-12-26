# Microsoft Developer Studio Generated NMAKE File, Based on loading.dsp
!IF "$(CFG)" == ""
CFG=loading - Win32 Debug
!MESSAGE No configuration specified. Defaulting to loading - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "loading - Win32 Release" && "$(CFG)" != "loading - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "loading.mak" CFG="loading - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "loading - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "loading - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "loading - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\bin\loading.exe"


CLEAN :
	-@erase "$(INTDIR)\loading.obj"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\bin\loading.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /Ob2 /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\loading.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\loading.pdb" /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../../bin/loading.exe" 
LINK32_OBJS= \
	"$(INTDIR)\loading.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\loading.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "loading - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\bin\loading.exe" "$(OUTDIR)\loading.bsc"


CLEAN :
	-@erase "$(INTDIR)\loading.obj"
	-@erase "$(INTDIR)\loading.sbr"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\samplecommon.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\loading.bsc"
	-@erase "$(OUTDIR)\loading.pdb"
	-@erase "..\..\bin\loading.exe"
	-@erase "..\..\bin\loading.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\loading.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\loading.sbr" \
	"$(INTDIR)\samplecommon.sbr"

"$(OUTDIR)\loading.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\loading.pdb" /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"../../bin/loading.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\loading.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\loading.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("loading.dep")
!INCLUDE "loading.dep"
!ELSE 
!MESSAGE Warning: cannot find "loading.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "loading - Win32 Release" || "$(CFG)" == "loading - Win32 Debug"
SOURCE=.\loading.cpp

!IF  "$(CFG)" == "loading - Win32 Release"


"$(INTDIR)\loading.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "loading - Win32 Debug"


"$(INTDIR)\loading.obj"	"$(INTDIR)\loading.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\common\samplecommon.cpp

!IF  "$(CFG)" == "loading - Win32 Release"


"$(INTDIR)\samplecommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "loading - Win32 Debug"


"$(INTDIR)\samplecommon.obj"	"$(INTDIR)\samplecommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

