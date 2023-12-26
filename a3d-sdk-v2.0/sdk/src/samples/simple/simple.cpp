/*
 * simple.cpp
 *
 *
 * Copyright (c) 1999 Aureal Semiconductor, Inc. - All rights reserved.
 *
 * This code may be used, copied or distributed in accordance with the terms
 * described in the A3D2.0 SDK License Agreement.
 *
 *
 * Sample program to show how to get started with A3D2.0, get the listener,
 * make a source, and move the source around the listener. Escape quits.
 *
*/

#include <objbase.h>
#include <cguid.h>

/* the A3D include file */
#include <initguid.h>
#include "ia3dutil.h"
#include "ia3dapi.h"

/* this is needed for _kbhit() */
#include <conio.h>

/* -------------------------------------------------------------------------- */

/* function prototypes */
void main(int argc, char **argv);
int  audio_init(void);
void audio_exit(void);
void audio_update(void);

/* -------------------------------------------------------------------------- */

/* some global variables */
IA3d3        *a3droot = NULL;
IA3dListener *a3dlis  = NULL;
IA3dSource   *a3dsrc  = NULL;

/* -------------------------------------------------------------------------- */

void
main(int argc, char **argv)
{
int hr;

	/* initialize the audio */
	hr = audio_init();
	if (hr == S_OK)
	{
		/* go into the main loop */
		while (!_kbhit())
			audio_update();
	}

	/* clean up the audio */
	audio_exit();

}

int
audio_init(void)
{
int hr;

    /* Use ia3d.lib A3dCreate, to get an initialized A3d object */
    hr = A3dCreate(NULL, (void **) &a3droot, NULL, NULL);

    if (FAILED(hr))
		return(hr);
	
	/*
	 * query for the listener
	*/
	hr = a3droot->QueryInterface(IID_IA3dListener, (void **)&a3dlis);
    if (FAILED(hr))
		return(hr);

	/* The cooperative level Must be set, else you cannot create sources */
	hr = a3droot->SetCooperativeLevel( GetForegroundWindow(), A3D_CL_NORMAL);

	/* make a new source. at this stage the source is empty - no wave data */
	hr = a3droot->NewSource( A3DSOURCE_INITIAL_RENDERMODE_A3D , &a3dsrc);
    if (FAILED(hr))
		return(hr);

	/*
	 * load wave data into the source. LoadWaveFile() is a convenience
	 * function - you don't have to use it. other functions are available
	 * to load wave data from memory and set its format.
	 * 
	 * no need to pass flags - all buffers have 3D capabilities. if you don't
	 * want 3D processing you can turn it off with a3dsrc->Mode().
	 */
	hr = a3dsrc->LoadWaveFile("heli.wav");
    if (FAILED(hr))
		return(hr);

	/* start playing the source */
	hr = a3dsrc->Play(A3D_LOOPED);

	return(S_OK);

}

void
audio_update(void)
{
static float xpos = -10.0f;
static float incr = 0.002f; /* scale this depending on your cpu speed */
HRESULT hr;

	/* clear the audio frame */
	hr = a3droot->Clear();

    // Time critical code, just do ASSERTs on hr.
	// ASSERT(SUCCEED(hr));

	/* locate and orientate the listener */
	hr = a3dlis->SetPosition3f(0.0f, 0.0f, 0.0f);
	hr = a3dlis->SetOrientationAngles3f(0.0f, 0.0f, 0.0f);

	/* locate the source */
	hr = a3dsrc->SetPosition3f(xpos, 0.0f, -5.0f);
	
	/* bounce the source between -10 and +10m in x */
	xpos += incr;
	if (xpos > 10.0f || xpos < -10.0f)
		incr = -incr;

	/* send this frame to be rendered */
	hr = a3droot->Flush();

}

void
audio_exit(void)
{

	/*
	 * clean up the audio. the order is important if you are not complelely
	 * shutting everything down and hope to create more sources later.
	 */

	/* stop playing the source (not entirely necessary) */
	if (a3dsrc)
	{
		a3dsrc->Stop();
		a3dsrc->Release();
	}

	if (a3dlis)
		a3dlis->Release();
	
	if (a3droot)
		a3droot->Release();

    /* free up COM (we could have done that at the end of audio_init() ) */
    CoUninitialize();

}

