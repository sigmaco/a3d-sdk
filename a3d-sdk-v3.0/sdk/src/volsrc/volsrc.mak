# Microsoft Developer Studio Generated NMAKE File, Based on volsrc.dsp
!IF "$(CFG)" == ""
CFG=volsrc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to volsrc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "volsrc - Win32 Release" && "$(CFG)" != "volsrc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "volsrc.mak" CFG="volsrc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "volsrc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "volsrc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "volsrc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\bin\volsrc.exe"


CLEAN :
	-@erase "$(INTDIR)\d3denum.obj"
	-@erase "$(INTDIR)\d3dframe.obj"
	-@erase "$(INTDIR)\d3dmath.obj"
	-@erase "$(INTDIR)\d3dtextr.obj"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\userdlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\volsrc.obj"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(INTDIR)\winmain.res"
	-@erase "..\..\bin\volsrc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "./DX" /I "../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\winmain.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\volsrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ia3dutil.lib dxguid.lib d3dim.lib ddraw.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\volsrc.pdb" /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../../bin/volsrc.exe" /libpath:"../../lib" 
LINK32_OBJS= \
	"$(INTDIR)\d3denum.obj" \
	"$(INTDIR)\d3dframe.obj" \
	"$(INTDIR)\d3dmath.obj" \
	"$(INTDIR)\d3dtextr.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\userdlg.obj" \
	"$(INTDIR)\volsrc.obj" \
	"$(INTDIR)\winmain.obj" \
	"$(INTDIR)\winmain.res"

"..\..\bin\volsrc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "volsrc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\bin\volsrc.exe"


CLEAN :
	-@erase "$(INTDIR)\d3denum.obj"
	-@erase "$(INTDIR)\d3dframe.obj"
	-@erase "$(INTDIR)\d3dmath.obj"
	-@erase "$(INTDIR)\d3dtextr.obj"
	-@erase "$(INTDIR)\d3dutil.obj"
	-@erase "$(INTDIR)\userdlg.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\volsrc.obj"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(INTDIR)\winmain.res"
	-@erase "$(OUTDIR)\volsrc.pdb"
	-@erase "..\..\bin\volsrc.exe"
	-@erase "..\..\bin\volsrc.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "./DX" /I "../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\winmain.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\volsrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ia3dutil.lib dxguid.lib d3dim.lib ddraw.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\volsrc.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../../bin/volsrc.exe" /pdbtype:sept /libpath:"../../lib" 
LINK32_OBJS= \
	"$(INTDIR)\d3denum.obj" \
	"$(INTDIR)\d3dframe.obj" \
	"$(INTDIR)\d3dmath.obj" \
	"$(INTDIR)\d3dtextr.obj" \
	"$(INTDIR)\d3dutil.obj" \
	"$(INTDIR)\userdlg.obj" \
	"$(INTDIR)\volsrc.obj" \
	"$(INTDIR)\winmain.obj" \
	"$(INTDIR)\winmain.res"

"..\..\bin\volsrc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("volsrc.dep")
!INCLUDE "volsrc.dep"
!ELSE 
!MESSAGE Warning: cannot find "volsrc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "volsrc - Win32 Release" || "$(CFG)" == "volsrc - Win32 Debug"
SOURCE=.\Dx\d3denum.cpp

"$(INTDIR)\d3denum.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Dx\d3dframe.cpp

"$(INTDIR)\d3dframe.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Dx\d3dmath.cpp

"$(INTDIR)\d3dmath.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Dx\d3dtextr.cpp

"$(INTDIR)\d3dtextr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Dx\d3dutil.cpp

"$(INTDIR)\d3dutil.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Dx\userdlg.cpp

"$(INTDIR)\userdlg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\volsrc.cpp

"$(INTDIR)\volsrc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\winmain.cpp

"$(INTDIR)\winmain.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\winmain.rc

"$(INTDIR)\winmain.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

