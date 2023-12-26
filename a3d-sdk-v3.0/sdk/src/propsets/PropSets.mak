# Microsoft Developer Studio Generated NMAKE File, Based on PropSets.dsp
!IF "$(CFG)" == ""
CFG=PropSets - Win32 Debug
!MESSAGE No configuration specified. Defaulting to PropSets - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "PropSets - Win32 Release" && "$(CFG)" != "PropSets - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PropSets.mak" CFG="PropSets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PropSets - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PropSets - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "PropSets - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\bin\PropSets.exe"


CLEAN :
	-@erase "$(INTDIR)\PropSets.obj"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\bin\PropSets.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PropSets.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\PropSets.pdb" /machine:I386 /out:"../../bin/PropSets.exe" 
LINK32_OBJS= \
	"$(INTDIR)\PropSets.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\PropSets.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PropSets - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\bin\PropSets.exe"


CLEAN :
	-@erase "$(INTDIR)\PropSets.obj"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PropSets.pdb"
	-@erase "..\..\bin\PropSets.exe"
	-@erase "..\..\bin\PropSets.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\PropSets.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PropSets.pdb" /debug /machine:I386 /out:"../../bin/PropSets.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\PropSets.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\PropSets.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("PropSets.dep")
!INCLUDE "PropSets.dep"
!ELSE 
!MESSAGE Warning: cannot find "PropSets.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PropSets - Win32 Release" || "$(CFG)" == "PropSets - Win32 Debug"
SOURCE=.\PropSets.cpp

"$(INTDIR)\PropSets.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=..\common\samplecommon.cpp

"$(INTDIR)\samplecommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

