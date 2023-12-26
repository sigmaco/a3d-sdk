# Microsoft Developer Studio Generated NMAKE File, Based on ia3dutil.dsp
!IF "$(CFG)" == ""
CFG=ia3dutil - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ia3dutil - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ia3dutil - Win32 Release" && "$(CFG)" !=\
 "ia3dutil - Win32 Debug"
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

!IF  "$(CFG)" == "ia3dutil - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\lib\ia3dutil.lib"

!ELSE 

ALL : "..\..\..\lib\ia3dutil.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "..\..\..\lib\ia3dutil.lib"
	-@erase ".\Release\ia3dutil.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\ia3dutil.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ia3dutil.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\..\lib\ia3dutil.lib" 
LIB32_OBJS= \
	".\Release\ia3dutil.obj"

"..\..\..\lib\ia3dutil.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ia3dutil - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\Lib\ia3dutild.lib" "$(OUTDIR)\ia3dutil.bsc"

!ELSE 

ALL : "..\..\..\Lib\ia3dutild.lib" "$(OUTDIR)\ia3dutil.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\ia3dutil.bsc"
	-@erase "..\..\..\Lib\ia3dutild.lib"
	-@erase ".\Debug\ia3dutil.obj"
	-@erase ".\Debug\ia3dutil.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\..\..\inc" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ia3dutil.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ia3dutil.bsc" 
BSC32_SBRS= \
	".\Debug\ia3dutil.sbr"

"$(OUTDIR)\ia3dutil.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"..\..\..\Lib\ia3dutild.lib" 
LIB32_OBJS= \
	".\Debug\ia3dutil.obj"

"..\..\..\Lib\ia3dutild.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "ia3dutil - Win32 Release" || "$(CFG)" ==\
 "ia3dutil - Win32 Debug"
SOURCE=.\ia3dutil.c

!IF  "$(CFG)" == "ia3dutil - Win32 Release"

DEP_CPP_IA3DU=\
	"..\..\..\..\..\src\a3d\apidll\verifya3d.h"\
	"..\..\..\inc\ia3dapi.h"\
	"..\..\..\inc\ia3dutil.h"\
	
INTDIR_SRC=.\Release
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "..\..\..\inc" /D "WIN32" /D "NDEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR_SRC)\ia3dutil.pch" /YX /Fo"$(INTDIR_SRC)\\"\
 /Fd"$(INTDIR_SRC)\\" /FD /c 

".\Release\ia3dutil.obj" : $(SOURCE) $(DEP_CPP_IA3DU) "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ia3dutil - Win32 Debug"

DEP_CPP_IA3DU=\
	"..\..\..\..\..\src\a3d\apidll\verifya3d.h"\
	"..\..\..\inc\ia3dapi.h"\
	"..\..\..\inc\ia3dutil.h"\
	
INTDIR_SRC=.\Debug
CPP_SWITCHES=/nologo /MTd /W3 /GX /Z7 /Od /I "..\..\..\inc" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR_SRC)\\" /Fp"$(INTDIR_SRC)\ia3dutil.pch" /YX\
 /Fo"$(INTDIR_SRC)\\" /Fd"$(INTDIR_SRC)\\" /FD /c 

".\Debug\ia3dutil.obj"	".\Debug\ia3dutil.sbr" : $(SOURCE) $(DEP_CPP_IA3DU)\
 "$(INTDIR_SRC)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

