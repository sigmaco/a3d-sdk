/////////////////////////////////////////////////////////////////////////////
// PropSets.cpp
// ============
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to demonstrate the use of Property Sets through the
//		A3D interface.
//
// Remarks:
//		This sample uses EAX as an example of using property sets as a
//		method of communicating to the hardware.  We do not ship the EAX.H
//		header file for obvious reasons.  You can obtain this file free from
//		the EAX SDK from you know who's site.
/////////////////////////////////////////////////////////////////////////////

#define INITGUID

#include <conio.h>
#include <objbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <cguid.h>
#include <windows.h>

#include "ia3dapi.h"
#include "dsound.h"
#include "eax.h"		// The A3D SDK does not ship this header.  You can get this from the EAX SDK.
						// in order to build this application.

#include "..\common\samplecommon.h"


/////////////////////////////////////////////////////////////////////////////
// Program Functions.
//
void main(int argc, char** argv)
{
	HRESULT hResult = S_OK;

	IA3d5* pA3d = NULL;
	IA3dSource2* pSource = NULL;
	IA3dPropertySet* pPropertySet = NULL;

	try
	{
		// Initialize COM libraries.
		hResult = CoInitialize(NULL);
		if(FAILED(hResult))
		{
			throw "Failed to initialize the COM libraries";
		}

		// Create and instantiate the A3D interface.
		hResult = CoCreateInstance(CLSID_A3dApi, NULL, CLSCTX_INPROC_SERVER, IID_IA3d5, (void**)&pA3d);
		if(FAILED(hResult))
		{
			throw "Failed to CoCreate() IID_IA3d5 interface";
		}

		printf("Received A3D Interface.\n");

		// Get the window handle to this window.
		HWND hWnd = SampleCommon::GetConsoleHwnd();
		if(hWnd == NULL)
		{
			throw "Unable to retrieve the window handle";
		}

		// Initialize the interface.
		hResult = pA3d->InitEx(NULL, NULL, A3DRENDERPREFS_DEFAULT, hWnd, A3D_CL_NORMAL);
		if(FAILED(hResult))
		{
			throw "Failed to Initialize IID_IA3d5 interface";
		}

		printf("Initialized A3D.\n");

		// Create a new source.
		if(FAILED(pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &pSource)))
		{
			throw "Failed to allocate a new source";
		}

		printf("A3D Source Allocated.\n");

		// Load our wave file into our newly created source.
		if(FAILED(pSource->LoadWaveFile("Heli.wav")))
		{
			throw "Couldn't load the wave file.\n";
		}

		printf("\nLoaded Heli.wav, Playing without property sets...\n");

		// Play and Flush the buffers.
		pSource->Play(A3D_LOOPED);
		pA3d->Flush();

		// Tell the user to press any key to exit this demo.
		printf("\nPress any key to apply Property Sets...\n");
		getch();

		// Get the global property set interface.  All QuerySupport() calls should be made from the
		// property set interface from the IA3d5 interface.  This will ensure that the call gets
		// down to the hardware device immediately for processing.
		if(FAILED(pA3d->QueryInterface(IID_IA3dPropertySet, (void**)&pPropertySet)))
		{
			throw "Failed to query for the global A3D Property Set interface";
		}

		printf("Received IA3dPropertySet Interface.\n");

		// Query the hardware for EAX Property Set support.
		ULONG ulSupport = 0;
		if(FAILED(pPropertySet->QuerySupport(DSPROPSETID_EAX_ReverbProperties, DSPROPERTY_EAX_ALL, &ulSupport)))
		{
			throw "Failed QuerySupport() for property set";
		}
		else
		{
			// Check for both Get() and Set() support.
			if((ulSupport & (KSPROPERTY_SUPPORT_GET | KSPROPERTY_SUPPORT_SET)) != (KSPROPERTY_SUPPORT_GET | KSPROPERTY_SUPPORT_SET))
			{
				printf("Can't Get() and Set() all properties in the requested Property set.\n");
			}
			else
			{
				printf("We have Get() & Set() Support for the requested Property sets.\n");
			}
		}
		

		// Set the default value for buffer state so the Resource Manager knows how to reset the buffers.
		float fReverbMix = EAX_REVERBMIX_USEDISTANCE;
		if(FAILED(pPropertySet->AddInitialStateParameters(DSPROPSETID_EAXBUFFER_ReverbProperties, DSPROPERTY_EAXBUFFER_REVERBMIX, NULL, 0, &fReverbMix, sizeof(float))))
		{
			throw "Failed AddInitialStateParameters() call";
		}
		
		// Make a Set() call and follow it up with a Get() call to demonstrate.
		ULONG ulEnvironment = 0x19;
		ULONG ulBytesReturned = 0;
		if(FAILED(pPropertySet->Set(DSPROPSETID_EAX_ReverbProperties, DSPROPERTY_EAX_ENVIRONMENT, NULL, 0, &ulEnvironment, sizeof(ULONG), A3DPROPSET_WAITFORRESULTS)))
		{
			printf("Failed Set() call.\n");
		}

		ULONG ulEnvironmentGet = 0;
		if(FAILED(pPropertySet->Get(DSPROPSETID_EAX_ReverbProperties, DSPROPERTY_EAX_ENVIRONMENT, NULL, 0, &ulEnvironmentGet, sizeof(ULONG), &ulBytesReturned)))
		{
			printf("Failed EAX Get() call.\n");
		}
		else
		{
			printf("Get() Returned %u.\n", ulEnvironmentGet);
		}
	}
	catch(char* szError)
	{
		// Display the message.
		printf("ERROR - %s.\n", szError);
	}

	// Tell the user to press any key to exit this demo.
	printf("\nPress any key to exit...\n");
	getch();

	// Cleanup.
	if(pPropertySet)
	{
		pPropertySet->Release();
		pPropertySet = NULL;
	}
	
	if(pSource)
	{
		pSource->Stop();
		pSource->Release();
		pSource = NULL;
	}

	if(pA3d)
	{
		pA3d->Release();
		pA3d = NULL;
	}

	CoUninitialize();
	return;
}