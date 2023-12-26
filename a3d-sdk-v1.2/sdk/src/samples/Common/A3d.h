//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	T1: Tutorial One - Creating a 3D-Audio application.
//
//	Illustrates:	Creating an A3D render object.
//						Creating an A3D primary buffer.
//						Creating an A3D listener object.
//						Creating an A3D 3d-source buffer object.
//
//	Also contains:	2d-source buffer object creation.
//						Wavefile loading code.
//						Application-window creation code.
//
//	Created:			25th November, 1996, by Suneil Mishra.
//						Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
// FILE: A3d.h		3d-audio-control header file.
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __A3D_H
#define __A3D_H

#include <stdio.h>
#include <windows.h>
#include <objbase.h>
#include <dsound.h>		// Include header for DS3d support.
#include "ia3d.h"		// Include header for  A3d support.
#include "Wave.h"		// Include header for wavefile structures.


// Enumerate the types of buffers: Forced software, forced hardware, or optimal-at-the-time.
typedef enum _BUFFER_TYPE
{
	BUFFER_SOFTWARE,
	BUFFER_HARDWARE,
	BUFFER_OPTIMAL
} BUFFER_TYPE;

// Utility enumeration for non-looping vs. looping buffers.
enum
{
	PLAY_NONLOOPING,
	PLAY_LOOPING
};

// Utility enumeration for non-3D vs. 3D buffers.
enum
{
	PLAY_NON3D,
	PLAY_3D
};

// Utility enumeration for COM vs. non-COM renderer initialization.
enum
{
	INIT_NONCOCREATE,
	INIT_COCREATE
};

// Utility enumeration for deferred playing vs. immediate playing of buffers.
enum
{
	PLAY_DEFERRED,
	PLAY_IMMEDIATE
};


// Enumerate the types of renderers: A3D or DS3D. Undefined otherwise.
typedef enum _RENDER3D_TYPE
{
	RENDER_A3D,
	RENDER_DS3D,
	RENDER_UNDEF
} RENDER3D_TYPE;


// Enumerate the play-status possibilities: Uninitialized, Inactive, or Actively playing.
typedef enum _PLAY_STATUS
{
	PLAY_UNINITED,
	PLAY_INACTIVE,
	PLAY_ACTIVE
} PLAY_STATUS;

// Enumerate the 3D-processing-status possibilities:-
// Uninitialized, 3D-Disabled, Normal-Mode, or Head-Relative-Mode 3D-processing.
typedef enum _PROC_STATUS
{
	PROC_UNINITED,
	PROC_DISABLED3D,
	PROC_NORMAL3D,
	PROC_HEADREL3D
} PROC_STATUS;


// This structure contains information about the listener.
// This is not necessary, but is useful for keeping track of listener characteristics.
typedef struct LISTENER3D_INFO
{
	D3DVALUE	fDoppler;		// Doppler exaggeration factor: 1.0 is normal.
	D3DVALUE	fDistance;		// Distance scaling factor: 1.0 means a unit is 1.0 meters.
	D3DVALUE	fRolloff;		// Rolloff exaggeration factor: 1.0 is normal.
} LISTENER3D_INFO;


// This structure contains information about the 3d-audio-renderer.
// This is not necessary, but is useful for keeping track of renderer characteristics.
typedef struct RENDERER3D_INFO
{
	RENDER3D_TYPE	eRenderType;
	HWND			hAppWindow;
} RENDERER3D_INFO;


// This structure contains information about the 3d-source.
// This is not necessary, but is useful for keeping track of source characteristics.
typedef struct SOURCE3D_INFO
{
	BOOL			bIsHardware;
	BOOL			bIsLooping;
	PROC_STATUS	e3DStatus;
	PLAY_STATUS	ePlayStatus;
} SOURCE3D_INFO;


// This class describes the A3d renderer.
// It is in charge of controlling the A3d-renderer object.
// The renderer-object is associated with the application-window of its calling app.
class CA3dRenderer
{
	friend class CA3dListener;
	friend class CA3dSource;

	public:
		CA3dRenderer();
		~CA3dRenderer();


		// In creating a 3d-sound renderer we need a window-handle to associate with the renderer
		// to ascertain its scope. We default here to use the foreground-window if no handle is
		// supplied. We also allow rendering to be in A3D or DS3D, with A3D by default.
		int CreateA3dRenderer(HWND hAppWindow = GetForegroundWindow(),
									 const RENDER3D_TYPE eRenderType = RENDER_A3D);

		// Releases an already created 3d-sound renderer. Will stop all sources and do all cleanup if necessary.
		int DestroyA3dRenderer(void);

		// Duplicate a sound source.
		int DuplicateSource(CA3dSource *pOrigSource, CA3dSource *pDuplicateSource);

		// Query the capabilities of the 3d-audio rendering device.
		int GetCapabilities(DSCAPS *pA3dCaps);

		// Two utility functions to provide debug information. Not a necessary part of the class, but useful.
		void	DisplayErrorBox(HRESULT hRes);
		LPSTR	TranslateDSError(HRESULT hRes);

	public:
		// Pointer to the 3d-renderer object under A3d or DS3d. This is the top-level control for all audio processing.
		LPDIRECTSOUND		m_p3DAudioRenderer;

	private:
		// Characteristics of the renderer stored here. Not a necessary part of the class, but useful.
		RENDERER3D_INFO	m_RendererInfo;
};



// This class describes the listener object. There is only one listener object.
// The listener is where your 'ears' are in the world.
// This class also controls globally applied effects. (Such as Doppler.) 
//
// Note that this class contains all the 2d-handling necessary for a listener.
class CA3dListener : CA3dRenderer
{
	public:
		CA3dListener();
		~CA3dListener();

		// Creates a listener in 3d along with the 2d support required to do this.
		// p3DAudioRenderer:	pointer to the audio-renderer object to request a buffer from.
		int CreateListener(LPDIRECTSOUND p3DAudioRenderer);
		
		// Releases a created listener, including all the 2d support.
		int DestroyListener();

	public:
		// 2d Primary buffer object. This is the final output buffer for all audio streams.
		LPDIRECTSOUNDBUFFER		m_pDS3DPrimaryBuffer;
		// 3d Listener object. The 3d-complement of the primary buffer. It is the virtual 'head' object for 3d rendering.
		LPDIRECTSOUND3DLISTENER	m_pDS3DListener;

	private:
		// Characteristics of the listener stored here. Not a necessary part of the class, but useful.
		LISTENER3D_INFO			m_3DListenerInfo;
};


// This class describes a 3d-sound source. There can be many 3d-sources.
// Sources require data to be supplied from wavefiles.
//
// Note that this class contains all the 2d-handling necessary for a 3d-source.
class CA3dSource : CA3dRenderer
{
	public:
		CA3dSource();
		~CA3dSource();

		// p3DAudioRenderer:	pointer to the audio-renderer object to request a buffer from.
		// szWaveFilePath:	name of the wavefile with its full path, to be opened and played for this source.
		// eBufferType:		create a 3d-buffer and force it hardware, software, or choose the optimal type.
		// bPlayImmediately: create the buffer and play immediately, or wait until later until we actually play it
		// bPlayLooping:		create the buffer and play it as a looping audio stream, or just play it one time.
		// bMake3D:				create the buffer and get its 3D-interface, or leave it as 2D.
		int CreateSource(LPDIRECTSOUND p3DAudioRenderer, LPSTR szWaveFilePath,
							  BUFFER_TYPE eBufferType, BOOL bPlayImmediately = PLAY_IMMEDIATE,
							  BOOL bPlayLooping = PLAY_NONLOOPING, BOOL bMake3D = PLAY_3D);

		// Removes the 3d buffer (and 2d support required for it). Stops the source if playing as well.
		int DestroySource(void);

		// Plays an already created 3d-buffer source.
		int PlaySource(void);

		// Stops an already playing source. NB: This does not destroy the source.
		int StopSource(void);

		// Queries a 3D-buffer interface for a source.
		int Make3DSource(void);
		
		// Disables 3D-processing for a source.
		int Disable3DSource(void);
		
		// Enables 3D-processing for a source. Do normal-mode 3D-processing by default.
		int Enable3DSource(PROC_STATUS e3DProcMode = PROC_NORMAL3D);
		
		// Load audio data from a wavefile. 
		void *LoadWaveData(LPSTR waveName, WavCtrl *waveInfo);

	public:
		LPDIRECTSOUNDBUFFER		m_pDS3DBuffer2D;
		LPDIRECTSOUND3DBUFFER	m_pDS3DBuffer3D;
		SOURCE3D_INFO				m_3DSourceInfo;
};


#endif // #ifndef __A3D_H

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
