# Microsoft Developer Studio Generated NMAKE File, Based on T1.dsp
!IF "$(CFG)" == ""
CFG=T1 - Win32 Release
!MESSAGE No configuration specified. Defaulting to T1 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "T1 - Win32 Release" && "$(CFG)" != "T1 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "T1.mak" CFG="T1 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "T1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "T1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "T1 - Win32 Release"

OUTDIR=.\..\Bin
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\..\Bin
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\T1Release.exe"

!ELSE 

ALL : "$(OUTDIR)\T1Release.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\A3d.obj"
	-@erase "$(INTDIR)\Guids.obj"
	-@erase "$(INTDIR)\t1.obj"
	-@erase "$(INTDIR)\tutorials.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\T1Release.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)\T1.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tutorials.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o".\Release\T1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib dsound.lib ia3d.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)\T1Release.pdb" /machine:I386\
 /out:"$(OUTDIR)\T1Release.exe" /libpath:"..\..\..\lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\A3d.obj" \
	"$(INTDIR)\Guids.obj" \
	"$(INTDIR)\t1.obj" \
	"$(INTDIR)\tutorials.res"

"$(OUTDIR)\T1Release.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "T1 - Win32 Debug"

OUTDIR=.\..\Bin
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\..\Bin
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\T1Debug.exe" ".\Debug\T1.bsc"

!ELSE 

ALL : "$(OUTDIR)\T1Debug.exe" ".\Debug\T1.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\A3d.obj"
	-@erase "$(INTDIR)\A3d.sbr"
	-@erase "$(INTDIR)\Guids.obj"
	-@erase "$(INTDIR)\Guids.sbr"
	-@erase "$(INTDIR)\t1.obj"
	-@erase "$(INTDIR)\t1.sbr"
	-@erase "$(INTDIR)\tutorials.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\T1Debug.exe"
	-@erase "$(OUTDIR)\T1Debug.ilk"
	-@erase "$(OUTDIR)\T1Debug.pdb"
	-@erase ".\Debug\T1.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\inc" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\T1.pch" /YX\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\tutorials.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o".\Debug\T1.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\A3d.sbr" \
	"$(INTDIR)\Guids.sbr" \
	"$(INTDIR)\t1.sbr"

".\Debug\T1.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib winmm.lib dsound.lib ia3dd.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)\T1Debug.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\T1Debug.exe" /libpath:"..\..\..\lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\A3d.obj" \
	"$(INTDIR)\Guids.obj" \
	"$(INTDIR)\t1.obj" \
	"$(INTDIR)\tutorials.res"

"$(OUTDIR)\T1Debug.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(CFG)" == "T1 - Win32 Release" || "$(CFG)" == "T1 - Win32 Debug"
SOURCE=..\Common\A3d.cpp

!IF  "$(CFG)" == "T1 - Win32 Release"

DEP_CPP_A3D_C=\
	"..\..\..\inc\ia3d.h"\
	"..\common\a3d.h"\
	"..\common\wave.h"\
	

"$(INTDIR)\A3d.obj" : $(SOURCE) $(DEP_CPP_A3D_C) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "T1 - Win32 Debug"

DEP_CPP_A3D_C=\
	"..\..\..\inc\ia3d.h"\
	"..\common\a3d.h"\
	"..\common\wave.h"\
	

"$(INTDIR)\A3d.obj"	"$(INTDIR)\A3d.sbr" : $(SOURCE) $(DEP_CPP_A3D_C)\
 "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Common\Guids.c

!IF  "$(CFG)" == "T1 - Win32 Release"


"$(INTDIR)\Guids.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "T1 - Win32 Debug"


"$(INTDIR)\Guids.obj"	"$(INTDIR)\Guids.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\t1.cpp

!IF  "$(CFG)" == "T1 - Win32 Release"

DEP_CPP_T1_CP=\
	"..\..\..\inc\ia3d.h"\
	"..\common\a3d.h"\
	"..\common\wave.h"\
	

"$(INTDIR)\t1.obj" : $(SOURCE) $(DEP_CPP_T1_CP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "T1 - Win32 Debug"

DEP_CPP_T1_CP=\
	"..\..\..\inc\ia3d.h"\
	"..\common\a3d.h"\
	"..\common\wave.h"\
	

"$(INTDIR)\t1.obj"	"$(INTDIR)\t1.sbr" : $(SOURCE) $(DEP_CPP_T1_CP) "$(INTDIR)"


!ENDIF 

SOURCE=..\Common\tutorials.rc
DEP_RSC_TUTOR=\
	"..\Common\genicon.ico"\
	

!IF  "$(CFG)" == "T1 - Win32 Release"


"$(INTDIR)\tutorials.res" : $(SOURCE) $(DEP_RSC_TUTOR) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\tutorials.res" /i "\Anzio\src\Tutorials\Common"\
 /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "T1 - Win32 Debug"


"$(INTDIR)\tutorials.res" : $(SOURCE) $(DEP_RSC_TUTOR) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\tutorials.res" /i "\Anzio\src\Tutorials\Common"\
 /d "_DEBUG" $(SOURCE)


!ENDIF 


!ENDIF 

