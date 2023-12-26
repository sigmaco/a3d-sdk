/////////////////////////////////////////////////////////////////////////////
// IA3dUtil.h
// ==========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Function prototypes for the IA3dUtil library.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////

#ifndef _IA3DUTIL_H_
#define _IA3DUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

extern HRESULT A3dInitialize(void);
extern void    A3dUninitialize(void);
extern HRESULT A3dCreate(GUID *,void **,IUnknown FAR *,DWORD); 
extern HRESULT A3dRegister(void);
extern int A3dDetectHardware(void);    // Returns one of the following status's.

// Return values for A3dDetectHardware
#define NO_AUDIO_HARDWARE                0
#define A3D_HARDWARE_DETECTED            1
#define GENERIC_HARDWARE_DETECTED        2

#ifdef __cplusplus
};
#endif

#endif // _IA3DUTIL_H_
