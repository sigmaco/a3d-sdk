/////////////////////////////////////////////////////////////////////////////
// Reverb.cpp
// ===========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to demonstrate setting Preset Reverb through the A3D
//		interface.
//
// Remarks:
//		This sample uses EAX as an example of using property sets as a
//		method of communicating to the hardware.  We do not ship the EAX.H
//		header file for obvious reasons.  You can obtain this file free from
//		the EAX SDK from you know who's site.
/////////////////////////////////////////////////////////////////////////////


#include <conio.h>
#include <objbase.h>
#include <stdio.h>
#include <initguid.h>
#include "ia3dapi.h"

#include "..\common\samplecommon.h"


/////////////////////////////////////////////////////////////////////////////
// Program Functions.
//
void main(int argc, char** argv)
{
	HRESULT hResult = S_OK;

	IA3d5* pA3d = NULL;
	IA3dSource2* pSource = NULL;

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
		DWORD dwFeatures = A3D_DIRECT_PATH_A3D | A3D_REVERB;
		hResult = pA3d->InitEx(NULL, dwFeatures, A3DRENDERPREFS_DEFAULT, hWnd, A3D_CL_NORMAL);
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
		if(FAILED(pSource->LoadFile("Heli.wav", A3DSOURCE_FORMAT_WAVE)))
		{
			throw "Couldn't load the wave file.\n";
		}

		printf("\nLoaded Heli.wav.\n\t\t1) Playing without Reverb...\n");

		// Set it's position infront of you 3 meters away
		// Position affects reverb a bit.
		pSource->SetPosition3f(0.f, 0.f, -.1f);
		pSource->SetReverbMix(0.5f, 1.f);
		
		// Play and Flush the buffers.  Reverb takes affect at flush.
		pSource->Play(A3D_LOOPED);
		if(FAILED(pA3d->Flush()))
		{
			throw "Failed Flush";
		}
		
		// Check if we have Reverb
		if(pA3d->IsFeatureAvailable(A3D_REVERB) == FALSE)
		{
			throw "Reverb not supported on this device";
		}
		
		// Tell the user to press any key to exit this demo.
		printf("\t\tPress any Key to Set A3DREVERB_PRESET_BATHROOM\n");
		getch();
		
		// Create a Reverb object.
		IA3dReverb *pReverb = NULL;
		if(FAILED(pA3d->NewReverb(&pReverb)))
		{
			throw "Failed to create new Reverb object";
		}
		
		// Bind it to A3D.
		if(FAILED(pA3d->BindReverb(pReverb)))
		{
			throw "Failed to bind reverb to A3D.";
		}
		
		// Set the environment
		if(FAILED(pReverb->SetReverbPreset(A3DREVERB_PRESET_BATHROOM)))
		{
			throw "Failed to set A3DREVERB_PRESET_BATHROOM Preset";
		}
		
		// Reverb actions takes effect on Flush()
		if(FAILED(pA3d->Flush()))
		{
			throw "Failed Flush";
		}
		
		// Turn off the reverb
		printf("\t2) Playing with reverb\n\t\tPress any Key to Turn Off Reverb\n");
		getch();
		
		// To clear a global reverb, bind NULL
		if(FAILED(pA3d->BindReverb(NULL)))
		{
			throw "Fail to bind reverb to A3D";
		}

		// Reverb actions takes effect on Flush()
		if(FAILED(pA3d->Flush()))
		{
			throw "Failed Flush";
		}
	}
	catch(char* szError)
	{
		// Display the message.
		printf("AudioInit() - %s.\n", szError);
	}
	
	printf("\nPress any Key to Exit Program...\n");
	getch();

	// Clean up A3D and all the created A3D objects.
	if(pA3d)
	{
		pA3d->Shutdown();
	}

	// Tell COM we're done.
	CoUninitialize();
}
