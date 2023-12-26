//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//      A3d.cpp         This file contains all the 3d-audio functionality for the tutorials.
//                                      Each tutorial refers to calls within this file. In addition, other
//                                      calls are made directly within each tutorial to show A3d functionality.
//                                      When referring to any audio-handling, everything will be in this file
//                                      or in the header file: A3d.h.
//
//                                      NB: Wavefile utility structures can be found in Wave.h.
//
//
//      Created:                25th November, 1996, by Suneil Mishra.
//                                      Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved.   
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************/
/*                                                                                                                                                                                                                               */
/*      NB: CAPITALIZED COMMENTS REFER TO NECESSARY CALLS FOR 3D-AUDIO TO BE ENABLED.    */
/*                                                                                                                                                                                                                               */
/*************************************************************************************/

#include <objbase.h>
#include <cguid.h>

// Include header file containing class information and utility structures.
#include "A3d.h"


//////////////////////////////////////////////////////////////////////////////////////
// CA3dRenderer: 3d-audio renderer functions.
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Constructor - setup renderer characteristics and class members. 
CA3dRenderer::CA3dRenderer()
{
        // Setup the renderer characteristics as undefined so far.
        m_p3DAudioRenderer = NULL;
        m_RendererInfo.eRenderType = RENDER_UNDEF;
        m_RendererInfo.hAppWindow = NULL;
}


//////////////////////////////////////////////////////////////////////////////////////
// Destructor - cleanup renderer and class members. 
CA3dRenderer::~CA3dRenderer()
{
        // Reset the renderer characteristics.
        m_p3DAudioRenderer = NULL;
        m_RendererInfo.eRenderType = RENDER_UNDEF;
        m_RendererInfo.hAppWindow = NULL;
}


//////////////////////////////////////////////////////////////////////////////////////
// CreateA3dRenderer - Create the renderer object and initialize it for use.
int CA3dRenderer::CreateA3dRenderer(HWND hAppWindow, const RENDER3D_TYPE eRenderType)
{
        HRESULT hr;

        // Store renderer information.
        m_RendererInfo.hAppWindow = hAppWindow;
        m_RendererInfo.eRenderType = eRenderType;

        // ALWAYS CHECK RETURN CODES AND HANDLE APPROPRIATELY.
        if (FAILED(hr = A3dCreate(NULL,(void **)&m_p3DAudioRenderer,NULL)))
        {
                // We failed to create the renderer object.
                DisplayErrorBox(hr);    // Display what went wrong before exiting.
                DestroyA3dRenderer();   // Cleanup before exiting.
                return(hr);
        }

        /* Handle appropriately for A3dCrate */

        switch (hr)
        {
         case A3D_OK_OLD_DLL:
                MessageBox(NULL,"Older A3d drivers detected.  You should consider downloading new drivers from www.aureal.com","A3d Warning",MB_OK);
                break;
         case DS_OK:
                MessageBox(NULL,"A3d Direct Sound failed to initialize.  You are running Microsoft Direct Sound.","A3d Warning",MB_OK);
                break;
         default:
                break;
    }
        
        // Initialize the renderer.
        // First, set the co-operative level for the renderer. This sets the level of access-control we have.
        // Priority-level access control allows the application to use SetFormat() and Compact() calls.
        // Unless the application specifically needs these, it should use Normal-level control.
        // If you are not sure which to use, go with NORMAL. Refer to the DirectSound documentation for more
        // on priority-levels, SetFormat() and Compact().
        
        // SET THE COOPERATIVE-LEVEL FOR THE RENDERER.
        DWORD dwLevel = DSSCL_NORMAL; // Use "DSSCL_PRIORITY" if SetFormat() or Compact() calls are needed.
        if (FAILED(hr = m_p3DAudioRenderer->SetCooperativeLevel(m_RendererInfo.hAppWindow, dwLevel)))
        {
                // We failed to set the cooperative-level for the renderer object.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyA3dRenderer();   // Cleanup before exiting.
        }

        return(hr);

        // We are finished creating the renderer object.
        // The next step would be to create the listener...see CreateListener().
}


//////////////////////////////////////////////////////////////////////////////////////
// DestroyA3dRenderer - Release and destroy the renderer object.
int CA3dRenderer::DestroyA3dRenderer(void)
{
        HRESULT hr;

        // RELEASE THE RENDERER OBJECT.
        if (m_p3DAudioRenderer)
        {
                if ((hr = m_p3DAudioRenderer->Release()) < DS_OK)
                        return(hr);

                m_p3DAudioRenderer = NULL;
        }

        // Reset the renderer.
        m_RendererInfo.eRenderType = RENDER_UNDEF;
        m_RendererInfo.hAppWindow = NULL;

        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// DuplicateSource - Make a duplicate of a buffer.
int CA3dRenderer::DuplicateSource(CA3dSource *pOrigSource, CA3dSource *pDuplicateSource)
{
        HRESULT hr = m_p3DAudioRenderer->DuplicateSoundBuffer(pOrigSource->m_pDS3DBuffer2D,
                                                                                                                                                   &pDuplicateSource->m_pDS3DBuffer2D);

        if(hr < DS_OK)
                return(hr);
        else
        {
                // Duplicate succeeded. Now copy info. on original source to duplicate source.
                pDuplicateSource->m_3DSourceInfo.bIsHardware = pOrigSource->m_3DSourceInfo.bIsHardware;
                pDuplicateSource->m_3DSourceInfo.bIsLooping  = pOrigSource->m_3DSourceInfo.bIsLooping;

                // Duplicate has not been triggered, so set it inactive (but initialized).
                pDuplicateSource->m_3DSourceInfo.ePlayStatus = PLAY_INACTIVE;
                return(hr);
        }
}


//////////////////////////////////////////////////////////////////////////////////////
// GetCapabilities - Get the capabilities of the renderer object.
int CA3dRenderer::GetCapabilities(DSCAPS *pA3dCaps)
{
        // Get capabilities of the renderer object.
        if (m_p3DAudioRenderer)
                return(m_p3DAudioRenderer->GetCaps(pA3dCaps));
        else
        {
                // No renderer object but that's okay. The structure will be NULL on returning anyway.
                pA3dCaps = NULL;
                return(DS_OK);
        }
}


//////////////////////////////////////////////////////////////////////////////////////
// The following two utility functions are for debugging purposes only.
// These can be moved where appropriate. They are here simply for ease of inclusion
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Displays an error-message box for the specific error condition.
void    CA3dRenderer::DisplayErrorBox(HRESULT hRes)
{
        char szTmp[80];
        sprintf( szTmp, "A3d Error ---> %s\n", TranslateDSError(hRes));
        MessageBox( GetForegroundWindow(), szTmp, "A3D-Error Information", MB_OK | MB_ICONSTOP );
}


//////////////////////////////////////////////////////////////////////////////////////
// Translates an error code into a more helpful error-string.
LPSTR   CA3dRenderer::TranslateDSError(HRESULT hRes)
{
        switch (hRes)
        {
                case DS_OK: return "No Error";
                case DSERR_ALLOCATED: return "DSERR_ALLOCATED";
                case DSERR_CONTROLUNAVAIL: return "DSERR_CONTROLUNAVAIL";
                case DSERR_INVALIDPARAM: return "DSERR_INVALIDPARAM";
                case DSERR_INVALIDCALL: return "DSERR_INVALIDCALL";
                case DSERR_GENERIC: return "DSERR_GENERIC";
                case DSERR_PRIOLEVELNEEDED: return "DSERR_PRIOLEVELNEEDED";
                case DSERR_OUTOFMEMORY: return "DSERR_OUTOFMEMORY";
                case DSERR_BADFORMAT: return "DSERR_BADFORMAT";
                case DSERR_UNSUPPORTED: return "DSERR_UNSUPPORTED";
                case DSERR_NODRIVER: return "DSERR_NODRIVER";
                case DSERR_ALREADYINITIALIZED: return "DSERR_ALREADYINITIALIZED";
                case DSERR_NOAGGREGATION: return "DSERR_NOAGGREGATION";
                case DSERR_BUFFERLOST: return "DSERR_BUFFERLOST";
                case DSERR_OTHERAPPHASPRIO: return "DSERR_OTHERAPPHASPRIO";
                case DSERR_UNINITIALIZED: return "DSERR_UNINITIALIZED";
                default: return "Unknown HRESULT";
        }
}


//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// CA3dListener: 3d-listener functions.
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Constructor - setup listener characteristics and class members. 
CA3dListener::CA3dListener()
{
        // Setup defaults for listener.
        m_pDS3DPrimaryBuffer = NULL;
        m_pDS3DListener = NULL;
        m_3DListenerInfo.fDoppler = 1.0f;
        m_3DListenerInfo.fDistance = 1.0f;
        m_3DListenerInfo.fRolloff = 1.0f;
}


//////////////////////////////////////////////////////////////////////////////////////
// Destructor - cleanup listener and class members. 
CA3dListener::~CA3dListener()
{
        // Everything should have been cleaned up by the DestroyListener() call.
}


//////////////////////////////////////////////////////////////////////////////////////
// CreateListener - Create 2d and 3d listener related objects.
int CA3dListener::CreateListener(LPDIRECTSOUND p3DAudioRenderer)
{
        HRESULT hr;
        
        // Copy the renderer pointer to the class.
        m_p3DAudioRenderer = p3DAudioRenderer;

        // Make sure the renderer is valid.
        if (!m_p3DAudioRenderer)
                return(-1);

        // CREATE THE 2D-PRIMARY BUFFER OBJECT.
        DSBUFFERDESC    dsbd;
        ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);

        // VERY IMPORTANT! YOU MUST USE THESE FLAGS.
        // THE FIRST FLAG SPECIFIES THIS IS THE PRIMARY BUFFER. (There is only ever one primary buffer.)
        // THE SECOND FLAG SPECIFIES THAT THE PRIMARY BUFFER HAS 3D-CONTROLS ENABLED.
        // WITHOUT THIS SECOND FLAG, YOU CANNOT HEAR ANY 3D AUDIO SOURCES...
        dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;

        if ((hr = m_p3DAudioRenderer->CreateSoundBuffer(&dsbd, &m_pDS3DPrimaryBuffer, NULL)) != DS_OK)
        {
                // We failed to create the primary buffer.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyA3dRenderer();   // Cleanup before exiting.
                return(hr);
        }

        // QUERY THE PRIMARY BUFFER TO GET THE 3D-LISTENER OBJECT.
        // NB: THIS WILL ALWAYS FAIL IF WE DID NOT SPECIFY THE CTRL3D FLAG EARLIER.
        if ((hr = m_pDS3DPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener, (void **) &m_pDS3DListener)) != DS_OK)
        {
                // We failed to get the 3d-listener.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyListener();              // Cleanup listener if necessary.
                DestroyA3dRenderer();   // Cleanup before exiting.
                return(hr);
        }
        
        // At this point, we have a valid listener object.
        // We can do many operations on the listener including positioning and orienting it...
        // and also set values for effects...

        // NB: In the following, if we fail, we are destroying all the 3d-audio support.
        //               This is not always necessary. How to handle such errors is up to the application.
        //               For example, failing 
        // Set the Doppler factor.
        if ((hr = m_pDS3DListener->SetDopplerFactor(m_3DListenerInfo.fDoppler, DS3D_IMMEDIATE)) != DS_OK)
        {
                // We failed to set the Doppler factor.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyListener();              // Cleanup listener.
                DestroyA3dRenderer();   // Cleanup before exiting.
                return(hr);
        }

        // Set the Distance factor.
        if ((hr = m_pDS3DListener->SetDistanceFactor(m_3DListenerInfo.fDistance, DS3D_IMMEDIATE)) != DS_OK)
        {
                // We failed to set the Distance factor.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyListener();              // Cleanup listener.
                DestroyA3dRenderer();   // Cleanup before exiting.
                return(hr);
        }
        
        // Set the Rolloff factor.
        if ((hr = m_pDS3DListener->SetRolloffFactor(m_3DListenerInfo.fRolloff, DS3D_IMMEDIATE)) != DS_OK)
        {
                // We failed to set the Rolloff factor.
                DisplayErrorBox(hr);            // Display what went wrong before exiting.
                DestroyListener();              // Cleanup listener.
                DestroyA3dRenderer();   // Cleanup before exiting.
        }
        
        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// DestroyListener - Release and destroy 2d and 3d listener related objects. 
int CA3dListener::DestroyListener(void)
{
        HRESULT hr;
        
        // Confirm we have a listener object.
        if (m_pDS3DListener)
        {
                // RELEASE THE 3D LISTENER FIRST.
                if ((hr = m_pDS3DListener->Release()) < DS_OK)
                        return(hr);
                        
                m_pDS3DListener = NULL;
        }

        if (m_pDS3DPrimaryBuffer)       // This should always be true here, but check to be sure.
        {
                // STOP THE PRIMARY BUFFER FROM PLAYING.
                if ((hr = m_pDS3DPrimaryBuffer->Stop()) < DS_OK)
                        return(hr);

                // AND THEN RELEASE THE PRIMARY BUFFER.
                if ((hr = m_pDS3DPrimaryBuffer->Release()) < DS_OK)
                        return(hr);

                m_pDS3DPrimaryBuffer = NULL;

                // At this point there should be no audible sound sources if we didn't error.
        }

        // Reset the listener.
        m_3DListenerInfo.fDoppler = 1.0f;
        m_3DListenerInfo.fDistance = 1.0f;
        m_3DListenerInfo.fRolloff = 1.0f;
        
        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// CA3dSource: 3d-source functions.
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Constructor - setup source characteristics and class members. 
CA3dSource::CA3dSource()
{
        // Setup the source.
        m_pDS3DBuffer2D = NULL;
        m_pDS3DBuffer3D = NULL;
        m_3DSourceInfo.bIsHardware = FALSE;
        m_3DSourceInfo.bIsLooping  = PLAY_NONLOOPING;
        m_3DSourceInfo.ePlayStatus = PLAY_UNINITED;
        m_3DSourceInfo.e3DStatus   = PROC_UNINITED;
}


//////////////////////////////////////////////////////////////////////////////////////
// Destructor - cleanup 3d-source and class members. 
CA3dSource::~CA3dSource()
{
        // Everything should have been cleaned up by the DestroySource() call.
}


//////////////////////////////////////////////////////////////////////////////////////
// CreateSource - Create 2d and 3d source related objects.
int CA3dSource::CreateSource(LPDIRECTSOUND p3DAudioRenderer,
                                                                          LPSTR szWaveFilePath, BUFFER_TYPE eBufferType,
                                                                          BOOL bPlayImmediately, BOOL bPlayLooping, BOOL bMake3D)
{
        HRESULT                 hr;
        DSBUFFERDESC    dsbd;
        VOID                            *pbWrite1 = NULL, *pbWrite2 = NULL;
        DWORD                           cbLen1, cbLen2;
        
        // Copy the renderer pointer to the class.
        m_p3DAudioRenderer = p3DAudioRenderer;

        // Make sure the renderer is valid.
        if (!m_p3DAudioRenderer)
                return(-1);

        // CREATE THE 2D SECONDARY-BUFFER FOR THIS SOURCE.
        ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        
        // THE DSBCAPS_STATIC FLAG DENOTES THAT THE AUDIO DATA IS LOADED ONLY ONCE. IT IS NOT A LIVE DATA STREAM.
        // A static buffer can be hardware accelerated for better performance.
        // THE DSBCAPS_CTRL3D FLAG DENOTES THAT WE WANT 3D-AUDIO ENABLED FOR THIS BUFFER. THIS MUST BE SPECIFIED.
        dsbd.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLDEFAULT | DSBCAPS_CTRL3D;

        // IMPORTANT NOTE!
        // IT IS UP TO THE APPLICATION TO CHECK IF HARDWARE BUFFERS ARE AVAILABLE IF USING THE DSBCAPS_LOCHARDWARE FLAG.
        // A CreateSoundBuffer() request with the DSBCAPS_LOCHARDWARE flag will fail
        // if there are no available hardware buffers.
        switch (eBufferType)
        {
                case BUFFER_HARDWARE:
                        // FORCE HARDWARE-MIXING. Note that this forces hardware regardless of the DSBCAPS_STATIC flag above. 
                        dsbd.dwFlags |= DSBCAPS_LOCHARDWARE;
                break;

                case BUFFER_SOFTWARE:
                        // FORCE SOFTWARE-MIXING. Note that this forces software regardless of whether hardware buffers are available.
                        dsbd.dwFlags |= DSBCAPS_LOCSOFTWARE;
                break;

                case BUFFER_OPTIMAL:
                default:
                        // WE DEFAULT TO THE OPTIMAL CONDITION WHERE HARDWARE ACCELERATION IS USED IF AVAILABLE.
                        // This requires no extra flags...
                break;
        }

        // GET THE WAVEFILE DATA.
        // NB:  szWaveFilePath stores the name of the wavefile with its complete path.
        //                      This wavefile loading utility can be replaced by one of your own.
        
        // Initialize the wave structure used to process the file.
        WavCtrl wave_file;
        wave_file.nSamplesPerSec = 0L;
        wave_file.wBitsPerSample = 0;
        wave_file.samples = 0L;
        wave_file.nBlockAlign = 1;
        wave_file.nChannels = 1;
        wave_file.wFormatTag = WAVE_FORMAT_PCM;
        wave_file.nAvgBytesPerSec = 0L;
        strcpy(wave_file.path, szWaveFilePath); // Copy the file-path to the info. structure.
        wave_file.data = (void *) NULL;
        
        if ((wave_file.data = LoadWaveData(&wave_file.path[0], &wave_file)) == NULL) 
        {
                // We failed to load the specified file. We error-out of the creation process.
                char buffer[40];
                wsprintf(buffer, "Error, %s could not be loaded", wave_file.path);
                MessageBox(NULL, buffer, "A3D Tutorial", MB_OK | MB_ICONSTOP);
                return(-1);
        }

        // PUT THE DATA INTO THE 2D-BUFFER OBJECT.
        // SET LENGTH OF DATA.
        dsbd.dwBufferBytes = wave_file.samples;

        // FILL THE FORMAT STRUCTURE.
        WAVEFORMATEX    wfxFormat;
        wfxFormat.wFormatTag                    = wave_file.wFormatTag;
        wfxFormat.nChannels                     = wave_file.nChannels;
        wfxFormat.nSamplesPerSec        = wave_file.nSamplesPerSec;
        wfxFormat.nAvgBytesPerSec       = wave_file.nAvgBytesPerSec;
        wfxFormat.nBlockAlign           = wave_file.nBlockAlign;
        wfxFormat.wBitsPerSample        = wave_file.wBitsPerSample;
        wfxFormat.cbSize                                = 0;
        dsbd.lpwfxFormat = &wfxFormat;
        
        // Set these to NULL before we start.
        m_pDS3DBuffer2D = NULL;
        m_pDS3DBuffer3D = NULL;

        // CREATE THE 2D-BUFFER BY REQUEST TO THE RENDER OBJECT.
        if ((hr = m_p3DAudioRenderer->CreateSoundBuffer(&dsbd, &m_pDS3DBuffer2D, NULL)) != DS_OK)
        {
                // Failed to get the 2d-buffer object for this source.
                if (m_pDS3DBuffer2D)
                        m_pDS3DBuffer2D = NULL;
                return(hr);
        }
        
        // LOCK THE BUFFER IN ORDER TO SAFELY WRITE THE AUDIO DATA TO IT.
        if ((hr = m_pDS3DBuffer2D->Lock(0, wave_file.samples, &pbWrite1, &cbLen1, &pbWrite2, &cbLen2, 0L)) != DS_OK)
        {
                // Failed to lock the buffer for writing the audio data.

                // Cleanup if we got far enough into the lock before failing.
                if (pbWrite1 != NULL)
                {
                        m_pDS3DBuffer2D->Unlock(pbWrite1, wave_file.samples, pbWrite2, 0);
                        pbWrite1 = NULL;
                }

                if (m_pDS3DBuffer2D != NULL)
                {
                        m_pDS3DBuffer2D->Release();
                        m_pDS3DBuffer2D = NULL;
                }
                
                return(hr);
        }

        // WRITE THE AUDIO DATA TO THE 2D-BUFFER NOW THAT IT IS SAFELY LOCKED.
        CopyMemory(pbWrite1, wave_file.data, wave_file.samples);

        // UNLOCK THE BUFFER NOW THE DATA HAS BEEN WRITTEN.
        if ((hr = m_pDS3DBuffer2D->Unlock(pbWrite1, wave_file.samples, pbWrite2, 0)) != DS_OK)
        {
                // Failed to unlock the buffer after writing the audio data.

                // Cleanup if we got far enough into the unlock before failing.
                if (pbWrite1 != NULL)
                {
                        // Try again but his time get out regardless of if we error or not.
                        m_pDS3DBuffer2D->Unlock(pbWrite1, wave_file.samples, pbWrite2, 0);
                        pbWrite1 = NULL;
                }

                if (m_pDS3DBuffer2D != NULL)
                {
                        m_pDS3DBuffer2D->Release();
                        m_pDS3DBuffer2D = NULL;
                }

                return(hr);
        }

        pbWrite1 = NULL;
        pbWrite2 = NULL;
        free(wave_file.data);
        wave_file.data = (void *) NULL;

        // At this point, m_pDS3DBuffer2D is a valid 2D buffer capable of rendering 2D-audio.

        // Make the 3D-buffer for the source, if requested to.
        if (bMake3D)
        {
                if ((hr = Make3DSource()) < DS_OK)
                        return(hr);

                // At this point, the 3d-buffer object, m_pDS3DBuffer3D,
                // is valid and available for all 3d-processing.
        }


        // NOTE: The source will not be rendered unless it is actively being played.

        // Set information on whether the file is looping or not.
        m_3DSourceInfo.bIsLooping = bPlayLooping;

        // Set the play status to show an inactive but initialized source.
        m_3DSourceInfo.ePlayStatus = PLAY_INACTIVE;
        
        // Start playing the sound if requested to.
        if (bPlayImmediately)
        {
                // Successfully created the buffer and we're activating it now.
                return(PlaySource());
        }
        
        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// Make3DSource - Initialize 3D-processing for a source.
int CA3dSource::Make3DSource(void)
{
        HRESULT hr;

        if (!m_pDS3DBuffer2D)
                return (-1);            // Special error-code indicating an invalid object to query from.

        if (m_pDS3DBuffer3D)    // If a valid 3D-buffer interface already, return success.
                return (DS_OK); // NB: This does not enable a valid, 3D-disabled source.

        // QUERY THE 2D-BUFFER TO GET THE 3D-BUFFER OBJECT.
        if((hr = m_pDS3DBuffer2D->QueryInterface(IID_IDirectSound3DBuffer, (void **) &m_pDS3DBuffer3D)) < DS_OK)
        {
                // Failed the query, so set 3D-flag off.
                m_3DSourceInfo.e3DStatus = PROC_DISABLED3D;
        }
        else
        {
                // Succeeded the query, so set 3D-flag to NORMAL_MODE which is the default.
                m_3DSourceInfo.e3DStatus = PROC_NORMAL3D;
        }

        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// Enable3DSource - Enable 3D-processing for a source.
int CA3dSource::Enable3DSource(PROC_STATUS e3DProcMode)
{
        HRESULT hr;
        DWORD           dwMode;

        if (!m_pDS3DBuffer2D)
        {
                m_3DSourceInfo.e3DStatus = PROC_UNINITED;
                return (-1);            // Special error-code indicating an invalid object to query from.
        }

        if (m_pDS3DBuffer3D)
        {
                m_3DSourceInfo.e3DStatus = e3DProcMode;

                switch (e3DProcMode)
                {
                        case PROC_NORMAL3D:
                                dwMode = DS3DMODE_NORMAL;
                        break;

                        case PROC_HEADREL3D:
                                dwMode = DS3DMODE_HEADRELATIVE;
                        break;

                        case PROC_DISABLED3D:
                        case PROC_UNINITED:
                        default:
                                dwMode = DS3DMODE_DISABLE;
                        break;
                }

                if ((hr = m_pDS3DBuffer3D->SetMode(dwMode, DS3D_IMMEDIATE)) < DS_OK)
                {
                        m_3DSourceInfo.e3DStatus = PROC_DISABLED3D;
                }

                return(hr);
        }
        else    // Make the source 3D, and then enable the 3D-source with the requested mode.
        {
                if((hr = Make3DSource()) < DS_OK)
                        return(hr);     // Failed to make the source 3D, so error out.
                else
                        return(Enable3DSource(e3DProcMode));
        }
}


//////////////////////////////////////////////////////////////////////////////////////
// Disable3DSource - Disable 3D-processing for a source.
int CA3dSource::Disable3DSource(void)
{
        if (!m_pDS3DBuffer2D)
                return (-1);            // Special error-code indicating an invalid object to query from.

        if (m_pDS3DBuffer3D)
                return (DS_OK); // Already have a 3D-buffer interface so return success.


        // QUERY THE 2D-BUFFER TO GET THE 3D-BUFFER OBJECT.
        return(m_pDS3DBuffer2D->QueryInterface(IID_IDirectSound3DBuffer, (void **) &m_pDS3DBuffer3D));
}


//////////////////////////////////////////////////////////////////////////////////////
// DestroySource - Release and destroy 2d and 3d source related objects. 
int CA3dSource::DestroySource(void)
{
        HRESULT hr;

        // Confirm we have a 3d buffer object.
        if (m_pDS3DBuffer3D)
        {
                // RELEASE THE 3D BUFFER FIRST.
                if ((hr = m_pDS3DBuffer3D->Release()) < DS_OK)
                        return(hr);

                m_pDS3DBuffer3D = NULL;
        }

        if (m_pDS3DBuffer2D) // This should always be true here, but check to be sure.
        {
                // STOP THE 2D BUFFER FROM PLAYING.
                if ((hr = m_pDS3DBuffer2D->Stop()) < DS_OK)
                        return(hr);

                // AND THEN RELEASE THE 2D BUFFER.
                if ((hr = m_pDS3DBuffer2D->Release()) < DS_OK)
                        return(hr);

                m_pDS3DBuffer2D = NULL;

                // At this point the sound source should be inaudible if we didn't error.
        }

        // Reset the source characteristics.
        m_3DSourceInfo.bIsHardware = FALSE;
        m_3DSourceInfo.ePlayStatus = PLAY_UNINITED;

        return(hr);
}


//////////////////////////////////////////////////////////////////////////////////////
// PlaySource - Starts a source playing.
int CA3dSource::PlaySource(void)
{
        // If the sound is uninitialized, we can't play it, so let the caller know that with the return code.
        if (m_3DSourceInfo.ePlayStatus == PLAY_UNINITED)
                return(-1);

        // If the sound is already playing, that's okay, we just don't start it again.
        // This is an application-level decision...ignoring this may not be what your app.
        // requires in handling a play call to an already playing buffer. Handle accordingly...
        else if (m_3DSourceInfo.ePlayStatus == PLAY_ACTIVE)
                return(DS_OK);

        // Source is valid to play, so set status-flag and then play the source.
        m_3DSourceInfo.ePlayStatus = PLAY_ACTIVE;
        
        // Play the sound looping or not as requested, since the source is all set to be activated.
        if (m_3DSourceInfo.bIsLooping)
                return(m_pDS3DBuffer2D->Play(0, 0, DSBPLAY_LOOPING));   // Looping flag
        else
                return(m_pDS3DBuffer2D->Play(0, 0, 0));                                 // Non-looping, no flag.
}


//////////////////////////////////////////////////////////////////////////////////////
// StopSource - Stops a source from playing. 
int CA3dSource::StopSource(void)
{
        // If the sound is uninitialized, we can't stop it, so let the caller know that with the return code.
        if (m_3DSourceInfo.ePlayStatus == PLAY_UNINITED)
                return(-1);

        // If the sound is already stopped, that's okay, we just don't stop it again.
        // This is an application-level decision...ignoring this may not be what your app.
        // requires in handling a stop call to an already stopped buffer. Handle accordingly...
        else if (m_3DSourceInfo.ePlayStatus == PLAY_INACTIVE)
                return(DS_OK);

        // Source is valid to stop, so set status-flag and then stop the source.
        m_3DSourceInfo.ePlayStatus = PLAY_INACTIVE;
        
        // Stop the source as requested.
        return(m_pDS3DBuffer2D->Stop());
}


//////////////////////////////////////////////////////////////////////////////////////
// LoadWaveData - Reads in audio-data from a given wavefile. 
#include <io.h>         // Include file-io header.
#include <fcntl.h>      // Include file-control header.

#ifdef __WATCOMC__
#define _open  open
#define _read  read
#define _tell  tell
#define _close close
#endif

void *CA3dSource::LoadWaveData(LPSTR waveName, WavCtrl *waveInfo)
{
        int                             fh, len;
        BYTE                            *mem;
        PCMWAVEFORMAT   pcmwf;
        CHUNK                           chunk;
        TAG                             tag;
        long                            ofs;

        // Open the binary wavefile for reading only.
        if ((fh = _open(waveName, O_BINARY | O_RDONLY)) == -1)
                return NULL;

        // Read the header information.
        len = _read(fh, &chunk, sizeof(CHUNK));
        mem = NULL;
        
        // Process the header information.
        while (len == sizeof(CHUNK))
        {
                // Confirm this is a wavefile by checking the header.
                if (memcmp((const void *) &chunk.Type, (const void *) "WAVE", 4) != 0)
                {
                        _lseek(fh, chunk.size, SEEK_CUR);
                        len = _read(fh, &chunk, sizeof(CHUNK));
                        continue;
                }

                // Read in the format information.
                _read(fh, &tag, sizeof(TAG));
                if (memcmp ((const void *) &tag.id, (const void *) "fmt ", 4) != 0)
                        break;

                ofs = _tell(fh);
                _read(fh, &pcmwf, sizeof(PCMWAVEFORMAT));
                _lseek(fh, ofs + tag.size, SEEK_SET);
                _read(fh, &tag, sizeof(TAG));

                // Read in the audio-data.
                if (memcmp((const void *) &tag.id, (const void *) "data", 4) != 0)
                        break;
                
                if ((mem = (BYTE *) malloc((size_t) tag.size)) == NULL)
                        break;
                
                // Set the information up. 
                waveInfo->wBitsPerSample  = pcmwf.wBitsPerSample;
                waveInfo->samples                         = _read(fh, mem, (size_t) tag.size);
                waveInfo->nSamplesPerSec  = pcmwf.wf.nSamplesPerSec;
                waveInfo->nBlockAlign     = pcmwf.wf.nBlockAlign;
                waveInfo->nChannels               = pcmwf.wf.nChannels;
                waveInfo->wFormatTag              = pcmwf.wf.wFormatTag;
                waveInfo->nAvgBytesPerSec = pcmwf.wf.nAvgBytesPerSec;
        }

        _close(fh);
        return mem;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
