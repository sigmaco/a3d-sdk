/////////////////////////////////////////////////////////////////////////////
// Loading.cpp
// ===========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to show how to get started with A3D 3.0, and use the
//		LoadFile() methods with different types of files.
//
// Remarks:
//		 Escape quits.
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
IA3d5*			g_pA3d			= NULL;
IA3dListener*	g_pListener		= NULL;
IA3dSource2*	g_pSrcWav		= NULL;
IA3dSource2*	g_pSrcWavStream	= NULL;
IA3dSource2*	g_pSrcMP3		= NULL;
IA3dSource2*	g_pSrcMp3Stream	= NULL;
IA3dSource2*	g_pSrcAc3Stream	= NULL;


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
		AudioExit();

		return;
	}

	// A highly interactive audio experience.
	printf("Aureal A3D Wave/WaveStream/MP3 Interactive audio experience.\n"
		   "  Press '1', '2', and '3'to toggle the three sounds (anything else to exit) :\n"
		   "    1 - linn48.wav (static wave file)\n"
		   "    2 - heli.wav (streamed wave file)\n"
		   "    3 - Scooter.mp3 (static mp3 file)\n"
		   "    4 - Scooter.mp3 (streamed mp3 file)\n"
		   "    5 - spoken_chan1.ac3 (streamed ac3 file)\n"
		   "\n> ");

	IA3dSource2 *pSrc = NULL;

	BOOL bNotDone = TRUE;
	while(bNotDone)
	{
		pSrc = NULL;

		while(!_kbhit())
		{
			AudioUpdate();
			g_pA3d->Flush();
			Sleep(10);
		}

		char ch = _getch();
		switch(ch)
		{
		case '1':
			pSrc = g_pSrcWav;
			break;

		case '2':
			pSrc = g_pSrcWavStream;
			break;

		case '3':
			pSrc = g_pSrcMP3;
			break;

		case '4':
			pSrc = g_pSrcMp3Stream;
			break;

		case '5':
			pSrc = g_pSrcAc3Stream;
			break;

		default:
			bNotDone = FALSE;
			break;
		}
		
		// Find out about the current play status of the source.
		// dwStatus will be 0 if the source is stopped.
		if(pSrc)
		{
			DWORD dwStatus = 0;
			pSrc->GetStatus(&dwStatus);
			
			if(dwStatus)
			{
				pSrc->Stop();
			}
			else
			{
				pSrc->Play(A3D_LOOPED);
			}
		}
			
		g_pA3d->Flush();
	}

	// shutdown the audio engine.
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
		hResult = g_pA3d->InitEx(NULL, NULL, A3DRENDERPREFS_DEFAULT, hWnd, A3D_CL_NORMAL);
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

		DWORD dwStreamBufferLength = 0;
		DWORD dwThreadPriority = 0;
		hResult = g_pA3d->GetStreamingProperties(&dwStreamBufferLength, &dwThreadPriority);
		if(FAILED(hResult))
		{
			throw "Failed to get streaming properties";
		}

		// Default buffer length is 0.9 s. This number is derrived from the maximum
		// reflection delay time (0.3 s) * 3.
		// The default thread priority is 'normal'.
		printf("Default streaming buffer length: %d\n", dwStreamBufferLength);
		
		// We just re-set the buffer length, and raise the thread priority.
		// Note that setting the buffer length does not mean that will be the exact length
		// of the streaming buffer. The buffer size when translated to bytes will be re-aligned to
		// depending on the wave file and be evenly divisible by three.
		hResult = g_pA3d->SetStreamingProperties(dwStreamBufferLength, A3D_STREAMING_PRIORITY_HIGH);	
		if(FAILED(hResult))
		{
			// This isn't a serious error. we'll just notify the user and
			// continue on.
			printf("AudioInit() - Failed to set the streaming properties.  Continuing anyways.\n");
		}

		// Make a new source. at this stage the source is empty - no wave data
		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_NATIVE, &g_pSrcWav);
		if(FAILED(hResult))
		{
			throw "Failed to create the unmanaged WAV source";
		}

		// This guy will be a 3D source.
		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &g_pSrcWavStream);
		if(FAILED(hResult))
		{
			throw "Failed to create the streaming WAV source";
		}

		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_NATIVE, &g_pSrcMP3);
		if(FAILED(hResult))
		{
			throw "Failed to create the MP3 source";
		}

		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_NATIVE, &g_pSrcMp3Stream);
		if(FAILED(hResult))
		{
			throw "Failed to create the streaming MP3 source";
		}

		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_NATIVE, &g_pSrcAc3Stream);
		if(FAILED(hResult))
		{
			throw "Failed to create the AC-3 source";
		}

		// This function call here is the equivalent of the old function LoadWaveFile().
		// These are convenience functions. There are other ways of getting sounds
		// loaded.
		hResult = g_pSrcWav->LoadFile("linn48.wav", A3DSOURCE_FORMAT_WAVE);
		if(FAILED(hResult))
		{
			throw "Failed to load linn48.wav";
		}

		// To stream a wave...
		// This source will be a 3D source. See function AudioUpdate() on how that works.
		// Note that typically one would stream a large wave file to save memory, but in our
		// SDK we didn't want to ship a huge wave file that people would have to download.
		hResult = g_pSrcWavStream->LoadFile("heli.wav", A3DSOURCE_FORMAT_WAVE | A3DSOURCE_STREAMING);
		if(FAILED(hResult))
		{
			throw "Failed to load heli.wav";
		}

		// And to load an MP3 file...
		hResult = g_pSrcMP3->LoadFile("Scooter.mp3", A3DSOURCE_FORMAT_MP3);
		if(FAILED(hResult))
		{
			throw "Failed to load Scooter.mp3";
		}

		// And to load a streamed MP3 file...
		hResult = g_pSrcMp3Stream->LoadFile("Scooter.mp3", A3DSOURCE_FORMAT_MP3 | A3DSOURCE_STREAMING);
		if(FAILED(hResult))
		{
			throw "Failed to load Scooter.mp3";
		}

		// You need to unlock the fallback AC-3 decoder if you want to be
		// guaranteed AC3 decoding.  You need to get a fallback key from
		// Dolby as per their licensing agreement.
		// ie: g_pA3d->UnlockFallbackAC3Decoder("YourCompnay Inc,{11111111-2222-3333-4444-555555555555},YourCompany Inc,YourCompany Programs", NULL);

		// And to load an AC3 file...
		// Note that AC3 files can only be loaded for streaming. If you omit the A3DSOURCE_STREAMING flag,
		// that flag is implicitly set.
		hResult = g_pSrcAc3Stream->LoadFile("spoken_chan1.ac3", A3DSOURCE_FORMAT_AC3 | A3DSOURCE_STREAMING);
		if(FAILED(hResult))
		{
			throw "Failed to load spoken_chan1.ac3";
		}

		// Request the sources to play looping...
		g_pSrcWav->Play(A3D_LOOPED);
		g_pSrcWavStream->Play(A3D_LOOPED);
		g_pSrcMP3->Play(A3D_LOOPED);
		g_pSrcMp3Stream->Play(A3D_LOOPED);
		g_pSrcAc3Stream->Play(A3D_LOOPED);
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
	static float incr = 0.25f;

	// Clear the audio frame.
	g_pA3d->Clear();

	// Locate and orientate the listener.
	g_pListener->SetPosition3f(0.0f, 0.0f, 0.0f);
	g_pListener->SetOrientationAngles3f(0.0f, 0.0f, 0.0f);

	// Locate the source.
	g_pSrcWavStream->SetPosition3f(xpos, 0.0f, -5.0f);
	
	// Bounce the source between -10 and +10m in x.
	xpos += incr;
	if(xpos > 10.0f || xpos < -10.0f)
	{
		incr = -incr;
	}
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
	// Shutdown releases all the A3D interfaces.
	if(g_pA3d)
	{
		g_pA3d->Shutdown();
	}

	// Cleanup COM libraries.
    CoUninitialize();
}
