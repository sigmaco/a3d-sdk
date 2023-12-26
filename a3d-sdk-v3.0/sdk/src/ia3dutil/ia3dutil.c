/////////////////////////////////////////////////////////////////////////////
// IA3dUtil.c
// ==========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		This is a utility library to wrap the details of COM and initializing
//		the A3D libraries.  There is also a utility function for determining
//		the level of A3D hardware acceleration support on the system.
//
// Remarks:
//		This file can be compiled as C or C++.  If the following two
//		undefined symbols occur during the link phase: CLSID_A3dApi and
//		IID_IA3d5, you need to include <initguid.h>.
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
//#include <initguid.h>

#include "ia3dutil.h"
#include "ia3dapi.h"
 
/* Use GUID_ instead of IID_ for both C or C++ */

#if !defined(__cplusplus) || defined(CINTERFACE)
#define GUID_IA3d			&IID_IA3d5
#define GUID_CLSID_A3dApi	&CLSID_A3dApi
#else
#define GUID_IA3d			IID_IA3d5
#define GUID_CLSID_A3dApi	CLSID_A3dApi
#endif

// Current interface version is IA3d5
#define IA3dX_Init(p,a,b,c)		IA3d5_Init(p,a,b,c)
#define IA3dX_Release(p)		IA3d5_Release(p)


/////////////////////////////////////////////////////////////////////////////
// A3dInitialize()
// ===============
//
// Description:
//		Initializes the COM and A3D libraries.
//
// Parameters:
//		None.
//
// Returns: 
//		S_OK:
//			Initialization and A3D registration succeeded.
//
//		S_FALSE 
//			The COM library is already initialized on this thread. 
//
//		E_FAIL:
//			Registration of one/both of the A3D libraries failed (A3D.DLL or
//			A3DAPI.DLL).
//
// Remarks:
//		You should only call this function once.  However, there is no harm
//		in calling it multiple times.  If you would just like to register
/////////////////////////////////////////////////////////////////////////////
HRESULT A3dInitialize(void)
{
	// Initialize the COM libraries.
	HRESULT hResult = CoInitialize(NULL);
	if(FAILED(hResult))
	{
		OutputDebugString("IA3dUtil: Failed to initialize the COM libraries.\n");
		return hResult;
	}

	// Register the A3D libraries.
	hResult = A3dRegister();
	if(FAILED(hResult))
	{
		OutputDebugString("IA3dUtil: Failed to register the A3D libraries.\n");
	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// A3dUninitialize()
// =================
//
// Description:
//		Informs COM that we are no longer using COM for this process.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		Call at the exit of your program if you use any function in this
//		module.  Once COM in unitialized, no further COM objects can be used
//		until COM is initialized again via the CoInitalize() or
//		A3dInitialize() functions.
/////////////////////////////////////////////////////////////////////////////
void A3dUninitialize(void)
{
	// Tell COM we're done using it.
	CoUninitialize();
}


/////////////////////////////////////////////////////////////////////////////
// A3dRegister()
// =============
//
// Description:
//		Registers the COM objects for A3D.
//
// Parameters:
//		None.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		E_FAIL:
//			The function couldn't complete because it was unable to find one
//			of the A3D.DLL's (A3D.DLL or A3DAPI.DLL), it couldn't call the
//			DllRegisterServer() exported function for either of these DLL's
//			or the DllRegisterServer() function returned a failure.
//
// Remarks:
//		You can call this function as many times as you like.  However, if
//		you call A3dInitialize(), this function will be called.  Also, you
//		must call A3dInitialize() prior to calling this function or you must
//		initialize the COM libraries yourself prior to calling this function
//		with the CoInitialize(NULL) function.
/////////////////////////////////////////////////////////////////////////////
HRESULT A3dRegister(void)
{
	PROC lpfnDllRegisterServer = NULL;
	HRESULT hResult = S_OK;

    // Register the A3D COM keys.  First load the A3D.DLL.
	HINSTANCE hA3dDLL = LoadLibrary("a3d.dll");
	if(hA3dDLL == NULL)
	{
		OutputDebugString("IA3dUtil: Failed to load the A3D.DLL for registration.\n");
		return E_FAIL;
	}

	// Get the address of the self-registering function.
	lpfnDllRegisterServer = GetProcAddress(hA3dDLL, "DllRegisterServer");
	if(lpfnDllRegisterServer == NULL)
	{
		OutputDebugString("IA3dUtil: Failed to get the DllRegisterServer() procedure address from the A3D.DLL.\n");
		return E_FAIL;
	}

	// Call the self-registering function so A3D.DLL will add all of the
	// required entries into the registry to function properly.
	hResult = lpfnDllRegisterServer();
	if(FAILED(hResult))
	{
		OutputDebugString("IA3dUtil: Failed to Register the A3D.DLL library.\n");
		return E_FAIL;
	}

	// Free the library.
	lpfnDllRegisterServer = NULL;
	FreeLibrary(hA3dDLL);

	// Register the A3DAPI.DLL COM Keys.  First load the A3DAPI.DLL.
	hA3dDLL = LoadLibrary("a3dapi.dll");
	if(hA3dDLL == NULL)
	{
		OutputDebugString("IA3dUtil: Failed to load the A3DAPI.DLL for registration.\n");
		return E_FAIL;
	}

	// Get the address of the self-registering function.
	lpfnDllRegisterServer = GetProcAddress(hA3dDLL, "DllRegisterServer");
	if(lpfnDllRegisterServer == NULL)
	{
		OutputDebugString("IA3dUtil: Failed to get the DllRegisterServer() procedure address from the A3DAPI.DLL.\n");
		return E_FAIL;
	}

	// Call the self-registering function so A3D.DLL will add all of the
	// required entries into the registry to function properly.
	hResult = lpfnDllRegisterServer();
	if(FAILED(hResult))
	{
		OutputDebugString("IA3dUtil: Failed to Register the A3DAPI.DLL library.\n");
		return E_FAIL;
	}

	lpfnDllRegisterServer = NULL;
	FreeLibrary(hA3dDLL);
	return S_OK;
}


/*****************************************************************************
*
*  A3dCreate()		Creates the A3D object. Basically uses COM
*					function CoCreateInstance() to initialize the IA3d4 inteface 
*                   and properly register it.  
*
*  Return:
*
*   >= 0            COM success
*
*   S_OK          
*
*   < 0             COM error
*
* 	E_INVALIDARG	ppA3d is NULL.
*   E_FAIL
*
******************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// A3dCreate()
// ===========
//
// Description:
//		Creates and instantiates the A3D COM object.
//
// Parameters:
//		LPGUID lpGUID:
//			Pointer to the sound device GUID you would like to use.  To use
//			the Primary Sound Device, you can pass in NULL.
//
//		void **ppA3d:
//			If successful, this is where the IA3d5 interface pointer will be
//			placed.  Otherwise, it will be NULL.
//
//		IUnknown FAR *pUnkOuter:
//			Outer COM object used only for aggregation.  A3D does not use
//			aggregation currently so this value must be NULL.
//
//		DWORD dwFeatures:
//			Features requested to be used on the sound device.  See the
//			IA3DAPI.H file for a list of the feature flags.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		E_FAIL:
//			TODO: Explain possible reasons why the method failed.
//
//		E_INVALIDARG:
//			One of the arguments was passed an invalid value.
//
//		E_POINTER:
//			TODO: If one of the parameters is a pointer, add the reason here.
//
// Remarks:
//		TODO: Add comments about the function and it's usage.
/////////////////////////////////////////////////////////////////////////////
HRESULT A3dCreate(LPGUID lpGUID, void **ppA3d, IUnknown FAR *pUnkOuter, DWORD dwFeatures)
{
	LPA3D5 pIA3d = NULL;
	HRESULT hResult = S_OK;

	// Parameter Checks
	if(!ppA3d)
	{
		OutputDebugString("IA3dUtil::A3dCreate() - ppA3d is NULL.\n");
		return E_POINTER;
	}
	
	// Set to NULL until successful instantiation of A3D interface.
	*ppA3d = NULL;
	
    // Initialize COM and A3D libraries for use.
	A3dInitialize();

	// COM create the A3d root interface.
	hResult = CoCreateInstance(
							GUID_CLSID_A3dApi,		// Class ID registerd to a3dapi.dll.
							pUnkOuter,				// Aggregate COM object.
							CLSCTX_INPROC_SERVER,	// Code runs on the same Machine.
							GUID_IA3d,				// Interface that we want.
							(void**)&pIA3d);		// Pointer to the requested interface.
	
	if(FAILED(hResult) || !pIA3d)
	{
		OutputDebugString("IA3dUtil::A3dCreate() - Failed to CoCreate() IA3d interface.\n");
		return hResult;
	}

	// Initialize A3D.
	hResult = IA3dX_Init(pIA3d, lpGUID, dwFeatures, A3DRENDERPREFS_DEFAULT);
	if(FAILED(hResult))
	{
		IA3dX_Release(pIA3d);
		return hResult;
	}
	
	// Success!  Return the A3D interface pointer to the caller.
	*ppA3d = pIA3d;
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// A3dDetectHardware()
// ===================
//
// Description:
//		Inspects the system for the presence of A3D Hardware.
//
// Parameters:
//		None.
//
// Returns: 
//		NO_AUDIO_HARDWARE:
//			There is no hardware acceleration present on the system.  A3D
//			will run in software.  We call this A2D.
//
//		A3D_HARDWARE_DETECTED:
//			Aureal hardware has been detected.  You will receive hardware
//			accelerated A3D.
//
//		GENERIC_HARDWARE_DETECTED:
//			Hardware acceleration has been detected and A3D will use it.
//			However, it is not Aureal hardware so not all features of A3D
//			will be available (for example, reflections).
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
int A3dDetectHardware(void)
{
	LPA3D5 pA3d = NULL;
	HRESULT hResult = S_OK;
	int nRetVal = NO_AUDIO_HARDWARE;
	A3DCAPS_HARDWARE a3dHwCaps;
	DWORD dwSize = sizeof(a3dHwCaps);

	// Initialize COM and A3D libraries.
	A3dInitialize();
	
	// Instantiate and get an A3D interface pointer.
	hResult = A3dCreate(NULL, (void**)&pA3d, NULL, A3D_DISABLE_SPLASHSCREEN);
	if(FAILED(hResult))
	{
		return nRetVal;
	}

	// Query A3D about the hardware (if any) that is available.
	ZeroMemory(&a3dHwCaps, dwSize);
	a3dHwCaps.dwSize = dwSize;
	hResult = IA3d5_GetHardwareCaps(pA3d, &a3dHwCaps);
	if(SUCCEEDED(hResult))
	{
		// Do we have hardware accelerated hardware 3D buffers?
		if(a3dHwCaps.dwMax3DBuffers > 0)
		{
			// If we've got hardware, check and see if its A3D accelerated.
			if(a3dHwCaps.dwFlags | A3D_DIRECT_PATH_A3D)
			{
				nRetVal = A3D_HARDWARE_DETECTED;
			}
			else if(a3dHwCaps.dwFlags | A3D_DIRECT_PATH_GENERIC)
			{
				nRetVal = GENERIC_HARDWARE_DETECTED;
			}
		}
	}

	// Release the A3D interface now that we're done with it and let COM know
	// that we're done with it too.
	IA3dX_Release(pA3d);
	A3dUninitialize();

	// Return the results to the caller.
	return nRetVal;
}
