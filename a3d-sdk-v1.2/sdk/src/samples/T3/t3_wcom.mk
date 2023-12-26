!include ..\watcom.mk
A3DSDK_LIB_DIR=..\..\..\lib\Release
A3DSDK_INC_DIR=..\..\..\inc
INC_DIRS=$WATCOM_INC_DIR;$A3DSDK_INC_DIR;$DXSDK_INC_DIR
OBJECTS=guids.obj a3d.obj t3.obj tutorials.res

!define BLANK ""
guids.obj : ..\common\guids.c &
.AUTODEPEND
 @c:
 *wcc386 ..\common\guids.c &
-i=$INC_DIRS &
-w4 -e25 -zq -otexan -d2 -5r -bt=nt -mf

a3d.obj : ..\common\a3d.cpp &
.AUTODEPEND
 @c:
 *wpp386 ..\common\a3d.cpp &
-i=$INC_DIRS &
-w4 -e25 -zq -otexan -d2 -5r -bt=nt -mf

t3.obj : t3.cpp &
.AUTODEPEND
 @c:
 *wpp386 t3.cpp &
-i=$INC_DIRS &
-w4 -e25 -zq -otexan -d2 -5r -bt=nt -mf

tutorials.res : ..\common\tutorials.rc &
.AUTODEPEND
 @c:
 *wrc ..\common\tutorials.rc -bt=nt &
-i=$[:;$WATCOM_INC_DIR -q -ad -r -fo=tutorials.res

t3.exe : $OBJECTS .AUTODEPEND
 @c:
 @%write t3.lk1 NAME t3
 @%append t3.lk1 FIL guids.obj,a3d.obj,t3.obj
 @%append t3.lk1 
!ifneq BLANK ""
 *wlib -q -n -b t3.imp 
 @%append t3.lk1 LIBR t3.imp
!endif
 *wlink d all SYS nt_win op m libp $DXSDK_LIB_DIR; libp $A3DSDK_LIB_DIR &
libr dsound.lib libf ia3d.lib op maxe=25 op q op symf @t3.lk1
!ifneq BLANK "tutorials.res"
 wrc -q -ad tutorials.res t3.exe
!endif
 copy t3.exe ..\bin\t3Release.exe
