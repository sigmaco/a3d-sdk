# Microsoft Developer Studio Generated NMAKE File, Based on ia3dutil.dsp
!IF "$(CFG)" == ""
CFG=ia3dutil - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ia3dutil - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ia3dutil - Win32 Release" && "$(CFG)" != "ia3dutil - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ia3dutil.mak" CFG="ia3dutil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ia3dutil - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ia3dutil - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "ia3dutil - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\lib\ia3dutil.lib"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\..\lib\ia3dutil.lib"
	-@erase ".\Release\ia3dutil.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /Ob2 /I "..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ia3dutil.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\lib\ia3dutil.lib" 
LIB32_OBJS= \
	".\Release\ia3dutil.obj"

"..\..\lib\ia3dutil.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ia3dutil - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\lib\ia3dutild.lib" "$(OUTDIR)\ia3dutil.bsc"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ia3dutil.bsc"
	-@erase "..\..\lib\ia3dutild.lib"
	-@erase ".\Debug\ia3dutil.obj"
	-@erase ".\Debug\ia3dutil.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /GX /ZI /Od /I "..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ia3dutil.bsc" 
BSC32_SBRS= \
	".\Debug\ia3dutil.sbr"

"$(OUTDIR)\ia3dutil.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\lib\ia3dutild.lib" 
LIB32_OBJS= \
	".\Debug\ia3dutil.obj"

"..\..\lib\ia3dutild.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("ia3dutil.dep")
!INCLUDE "ia3dutil.dep"
!ELSE 
!MESSAGE Warning: cannot find "ia3dutil.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ia3dutil - Win32 Release" || "$(CFG)" == "ia3dutil - Win32 Debug"
SOURCE=.\ia3dutil.c

!IF  "$(CFG)" == "ia3dutil - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /Ob2 /I "..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fo"Release/" /Fd"Release/" /FD /c 

".\Release\ia3dutil.obj" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ia3dutil - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GX /ZI /Od /I "..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"Debug/" /Fo"Debug/" /Fd"Debug/" /FD /c 

".\Debug\ia3dutil.obj"	".\Debug\ia3dutil.sbr" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

