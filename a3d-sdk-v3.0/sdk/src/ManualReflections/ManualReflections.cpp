/////////////////////////////////////////////////////////////////////////////
// ManualReflections.cpp
// =====================
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to show how to to generate and manage manual
//		reflections.
//
// Remarks:
//		 None.
/////////////////////////////////////////////////////////////////////////////

#include <objbase.h>
#include <cguid.h>

// A3D Include Files.
#include <initguid.h>
#include "ia3dutil.h"
#include "ia3dapi.h"

#include "..\common\samplecommon.h"

#include <stdio.h>
#include <conio.h>

/////////////////////////////////////////////////////////////////////////////
// Function Prototypes.
//
HRESULT	AudioInit(void);
void	AudioExit(void);
void	AudioUpdate(void);

/////////////////////////////////////////////////////////////////////////////
// Global A3D Objects.
//
const int		g_nReflections = 4;
BOOL			g_bPlayReflection = TRUE;

IA3d5*			g_pA3d			= NULL;
IA3dListener*	g_pListener		= NULL;
IA3dSource2*	g_pA3dSource	= NULL;
IA3dGeom2*		g_pA3dGeom		= NULL;
IA3dReflection*	g_pA3dReflections[g_nReflections];


/////////////////////////////////////////////////////////////////////////////
// Program Functions.
//
void main(int argc, char** argv)
{
	// Initialize the A3D audio engine.
	HRESULT hResult = AudioInit();
	if(FAILED(hResult))
	{
		printf("Failed to initialize audio.\n");
		return;
	}

	printf("Hit the SPACE BAR to toggle playing of the manual reflections. Any other key to quit.\n");

	// Keep going until the user presses a key other than the space bar.
	BOOL bExit = FALSE;
	while(!bExit)
	{
		if(_kbhit())
		{
			if(_getch() == ' ')
			{
				g_bPlayReflection = !g_bPlayReflection;
			}
			else
			{
				bExit = TRUE;
			}
		}

		// Update the audio source positions.
		AudioUpdate();
	}

	// Cleanup.
	AudioExit();
}



/////////////////////////////////////////////////////////////////////////////
// AudioInit()
// ===========
//
// Description:
//		Initializes the COM and A3D libraries.
//
// Parameters:
//		None.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		Any Other Value < 0:
//			The method was unsuccessful.  This return value will be qualified
//			by an error message in the debug window.			
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT AudioInit(void)
{
	HRESULT hResult = S_OK;

	try
	{
		// Initialize COM libraries.
		hResult = CoInitialize(NULL);
		if(FAILED(hResult))
		{
			throw "Failed to initialize the COM libraries";
		}

		// Create and instantiate the A3D interface.
		hResult = CoCreateInstance(CLSID_A3dApi, NULL, CLSCTX_INPROC_SERVER, IID_IA3d5, (void**)&g_pA3d);
		if(FAILED(hResult))
		{
			throw "Failed to CoCreate() IID_IA3d5 interface";
		}

		// Get the window handle to this window.
		HWND hWnd = SampleCommon::GetConsoleHwnd();
		if(hWnd == NULL)
		{
			throw "Unable to retrieve the window handle";
		}

		// Initialize the interface.
		// The second argument, A3D_1ST_REFLECTIONS, is the feature requests flag. 
		hResult = g_pA3d->InitEx(NULL, A3D_1ST_REFLECTIONS, A3DRENDERPREFS_DEFAULT, hWnd, A3D_CL_NORMAL);
		if(FAILED(hResult))
		{
			throw "Failed to Initialize IID_IA3d5 interface";
		}

		// Query for the listener object used for 3D sound positioning.
		hResult = g_pA3d->QueryInterface(IID_IA3dListener, (void **)&g_pListener);
		if(FAILED(hResult))
		{
			throw "Failed to create listener";
		}

		// Query for the Geometry interface. This interface is necessary to process
		// automatically generated and manually calculated reflections.
		hResult = g_pA3d->QueryInterface(IID_IA3dGeom2, (void**)&g_pA3dGeom);
		if(FAILED(hResult))
		{
			throw "Failed to create the Geometry interface";
		}

		// Make a new source. at this stage the source is empty - no wave data
		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &g_pA3dSource);
		if(FAILED(hResult))
		{
			throw "Failed to create the A3D source";
		}

		// LoadFile() supports a variety of different file formats and file streaming.
		// 
		// No need to pass flags - all buffers have 3D capabilities.  If you don't
		// want 3D processing you can turn it off with g_pA3dSource->Mode().
		hResult = g_pA3dSource->LoadFile("heli.wav", A3DSOURCE_FORMAT_WAVE);
		if(FAILED(hResult))
		{
			throw "Failed to Load heli.wav";
		}

		/////////////////////////////////////////////////////////////////////
		// Geometry Stuff Begins
		//

		// Need to enable the features that were requested above in init.
		hResult = g_pA3dGeom->Enable(A3D_1ST_REFLECTIONS);
		if(FAILED(hResult))
		{
			throw "Failed to enable reflections";
		}

		// Create the array of manual reflections.
		for(int i = 0; i < g_nReflections; ++i)
		{
			hResult = g_pA3dSource->NewManualReflection(&(g_pA3dReflections[i]));
			if(FAILED(hResult))
			{
				throw "Unable to create manual reflection";
			}

			// Set the default transformation mode. The other option A3DSOURCE_TRANSFORMMODE_HEADRELATIVE
			// means that all translations are relative to the listener's position.
			g_pA3dReflections[i]->SetTransformMode(A3DSOURCE_TRANSFORMMODE_NORMAL);
		}

		/////////////////////////////////////////////////////////////////////
		// End geometry section
		//

		// Start playing the source.
		hResult = g_pA3dSource->Play(A3D_LOOPED);
		if(FAILED(hResult))
		{
			throw "Failed to play the source";
		}
	}
	catch(char* szError)
	{
		// Display the message.
		printf("AudioInit() - %s.\n", szError);
	}

	return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// AudioUpdate()
// =============
//
// Description:
//		Positions and Moves the audio sources around.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void AudioUpdate(void)
{
	static float xpos = -10.0f;
	static float incr = 0.002f; // scale this depending on your cpu speed

	// Clear the audio frame.
	g_pA3d->Clear();

	// Locate and orientate the listener.
	g_pListener->SetPosition3f(0.0f, 0.0f, 0.0f);
	g_pListener->SetOrientationAngles3f(0.0f, 0.0f, 0.0f);

	// Locate the source.
	g_pA3dSource->SetPosition3f(xpos, 0.0f, -5.0f);
	
	// Bounce the source between -10 and +10m in x.
	xpos += incr;
	if(xpos > 10.0f || xpos < -10.0f)
	{
		incr = -incr;
	}

	/////////////////////////////////////////////////////////////////////////
	// Begin Manual Reflections.
	//

	for(int i = 0; i < g_nReflections; ++i)
	{
		// Do "dumb" placement of the reflections.
		
		// Divide the X position by the index + 1. This keeps the sources 
		// primarily clumped in front of the listener. 
		// Place them a little further behind the source.
		g_pA3dReflections[i]->SetPosition3f(-xpos / (i+1), 0.0f, -6.0f);
		
		// Set the delay time to the default times the index + 1.
		// Normally one would calculate these values based on distance.
		g_pA3dReflections[i]->SetDelay(0.3f * (i+1));

		if(g_bPlayReflection)
		{
			// Set the gain per reflection.
			// Normally this would be attenuated for distance.
			g_pA3dReflections[i]->SetGainScale(0.2f);
		}
		else
		{
			// Turn off the reflection volume.
			g_pA3dReflections[i]->SetGainScale(0.f);
		}
		
		// See documentation for other manual reflection controls.
	}
	
	/////////////////////////////////////////////////////////////////////////
	// End Manual Reflections.
	//

	// Send this frame to be rendered by A3D.
	g_pA3d->Flush();
}


/////////////////////////////////////////////////////////////////////////////
// AudioExit()
// ===========
//
// Description:
//		Shutsdown the A3D and COM libraries.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void AudioExit(void)
{
	// Although shutdown below cleans up left over manual reflections automatically,
	// here is how it is manually done.
	if(g_pA3dSource)
	{
		g_pA3dSource->FreeManualReflections();
	}

	// Shutdown releases all the A3D interfaces.
	if(g_pA3d)
	{
		g_pA3d->Shutdown();
	}

	// Cleanup COM libraries.
    CoUninitialize();
}

