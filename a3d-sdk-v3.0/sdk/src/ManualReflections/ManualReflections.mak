# Microsoft Developer Studio Generated NMAKE File, Based on ManualReflections.dsp
!IF "$(CFG)" == ""
CFG=ManualReflections - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ManualReflections - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ManualReflections - Win32 Release" && "$(CFG)" != "ManualReflections - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ManualReflections.mak" CFG="ManualReflections - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ManualReflections - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ManualReflections - Win32 Debug" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "ManualReflections - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\bin\ManualReflections.exe"


CLEAN :
	-@erase "$(INTDIR)\ManualReflections.obj"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\bin\ManualReflections.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ManualReflections.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ManualReflections.pdb" /machine:I386 /out:"../../bin/ManualReflections.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ManualReflections.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\ManualReflections.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ManualReflections - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\bin\ManualReflections.exe" "$(OUTDIR)\ManualReflections.bsc"


CLEAN :
	-@erase "$(INTDIR)\ManualReflections.obj"
	-@erase "$(INTDIR)\ManualReflections.sbr"
	-@erase "$(INTDIR)\samplecommon.obj"
	-@erase "$(INTDIR)\samplecommon.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ManualReflections.bsc"
	-@erase "$(OUTDIR)\ManualReflections.pdb"
	-@erase "..\..\bin\ManualReflections.exe"
	-@erase "..\..\bin\ManualReflections.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ManualReflections.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ManualReflections.sbr" \
	"$(INTDIR)\samplecommon.sbr"

"$(OUTDIR)\ManualReflections.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ManualReflections.pdb" /debug /machine:I386 /out:"../../bin/ManualReflections.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ManualReflections.obj" \
	"$(INTDIR)\samplecommon.obj"

"..\..\bin\ManualReflections.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("ManualReflections.dep")
!INCLUDE "ManualReflections.dep"
!ELSE 
!MESSAGE Warning: cannot find "ManualReflections.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ManualReflections - Win32 Release" || "$(CFG)" == "ManualReflections - Win32 Debug"
SOURCE=.\ManualReflections.cpp

!IF  "$(CFG)" == "ManualReflections - Win32 Release"


"$(INTDIR)\ManualReflections.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ManualReflections - Win32 Debug"


"$(INTDIR)\ManualReflections.obj"	"$(INTDIR)\ManualReflections.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=..\common\samplecommon.cpp

!IF  "$(CFG)" == "ManualReflections - Win32 Release"


"$(INTDIR)\samplecommon.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ManualReflections - Win32 Debug"


"$(INTDIR)\samplecommon.obj"	"$(INTDIR)\samplecommon.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

