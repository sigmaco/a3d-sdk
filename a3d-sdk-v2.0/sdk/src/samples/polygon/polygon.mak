# Microsoft Developer Studio Generated NMAKE File, Based on polygon.dsp
!IF "$(CFG)" == ""
CFG=polygon - Win32 Debug
!MESSAGE No configuration specified. Defaulting to polygon - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "polygon - Win32 Release" && "$(CFG)" !=\
 "polygon - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "polygon.mak" CFG="polygon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "polygon - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "polygon - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "polygon - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\bin\polygon.exe"

!ELSE 

ALL : "..\..\..\bin\polygon.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\polygon.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "..\..\..\bin\polygon.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../../../inc" /D "WIN32" /D "NDEBUG" /D\
 "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\polygon.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\polygon.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ia3dutil.lib opengl32.lib glu32.lib glaux.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\polygon.pdb" /machine:I386\
 /out:"..\..\..\bin\polygon.exe" /libpath:"..\..\..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\polygon.obj"

"..\..\..\bin\polygon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "polygon - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\..\..\bin\polygon.exe"

!ELSE 

ALL : "..\..\..\bin\polygon.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\polygon.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\polygon.pdb"
	-@erase "..\..\..\bin\polygon.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "../../../inc" /D "WIN32" /D\
 "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\polygon.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\polygon.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ia3dutild.lib opengl32.lib glu32.lib glaux.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\polygon.pdb" /debug\
 /machine:I386 /out:"..\..\..\bin\polygon.exe" /pdbtype:sept\
 /libpath:"..\..\..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\polygon.obj"

"..\..\..\bin\polygon.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "polygon - Win32 Release" || "$(CFG)" ==\
 "polygon - Win32 Debug"
SOURCE=.\polygon.cpp

!IF  "$(CFG)" == "polygon - Win32 Release"

DEP_CPP_POLYG=\
	"..\..\..\inc\ia3dapi.h"\
	"..\..\..\inc\ia3dutil.h"\
	

"$(INTDIR)\polygon.obj" : $(SOURCE) $(DEP_CPP_POLYG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "polygon - Win32 Debug"

DEP_CPP_POLYG=\
	"..\..\..\inc\ia3dapi.h"\
	"..\..\..\inc\ia3dutil.h"\
	

"$(INTDIR)\polygon.obj" : $(SOURCE) $(DEP_CPP_POLYG) "$(INTDIR)"


!ENDIF 


!ENDIF 

