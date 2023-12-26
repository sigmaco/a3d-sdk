//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	T2: Tutorial Two - Further 3D-Audio processes.
//
//	Illustrates:	Creating an A3D render object.				(T1)
//						Creating an A3D primary buffer.				(T1)
//						Creating an A3D listener object.				(T1)
//						Creating an A3D 3d-source buffer object.	(T1)
//
//						Positioning an A3D 3d-source object.
//						Orienting an A3D 3d-source object.
//						Positioning an A3D listener object.
//						Orienting an A3D listener object.
//						Setting a velocity for an A3D 3d-source.
//						Setting a velocity for an A3D listener.
//						Exaggerating Doppler and Rolloff effects.
//
//
//	Created:			30th November, 1996, by Suneil Mishra.
//						Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
// FILE: t2.cpp	Main application code for tutorial two.
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************/
/*																												 */
/*	NB: CAPITALIZED COMMENTS REFER TO NECESSARY CALLS FOR 3D-AUDIO TO BE ENABLED.		 */
/*		 SEARCH FOR THE KEYWORD "TUTORIAL:" WITHIN THIS FILE FOR AUDIO EXAMPLE CODE.	 */
/*																												 */
/*************************************************************************************/


// Include header to 3d-audio definitions.
#include "..\Common\A3d.h"
#include <math.h>

extern "C"	// Cover the windowing calls for C++.
{

//////////////////////////////////////////////////////////////////////////////////////
// Application routines to create a window and display information/error messages.
//////////////////////////////////////////////////////////////////////////////////////

int  PASCAL			WinMain(HANDLE hInstance, HANDLE hPrevInstance,
								  LPSTR lpszCmdParam, int nCmdShow);
long FAR PASCAL	WndProc(HWND hwnd, UINT message, UINT wParam, LONG lParam);


//////////////////////////////////////////////////////////////////////////////////////
// WinProc: Windows-message handling.
//////////////////////////////////////////////////////////////////////////////////////
long FAR PASCAL WndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	HDC hdc;
	HPEN hpen, hpenOld;
	PAINTSTRUCT ps;
	RECT rect;
 
	switch (message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);
			hpen = CreatePen(PS_SOLID, 6, RGB(0,0,255));
			hpenOld = SelectObject(hdc, hpen);
			Rectangle(hdc, rect.left+10, rect.top+10,
							  rect.right-10, rect.bottom-10);

			DrawText(hdc, "A3D: Tutorial Two", -1, &rect,
					  DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			SelectObject(hdc, hpenOld);
			DeleteObject(hpen);
			EndPaint(hWnd, &ps);
			return (0);
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return (0);
		break;

		default:
		break;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}


//////////////////////////////////////////////////////////////////////////////////////
// WinMain: Main application-loop for windows.
//////////////////////////////////////////////////////////////////////////////////////
int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
						 LPSTR lpszCmdParam, int nCmdShow)
{
	static char szAppName[] = "Tutorial-Two";
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
  
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;

		RegisterClass(&wndclass);
	}

	hWnd = CreateWindow(szAppName, "A3D Tutorial", WS_OVERLAPPEDWINDOW,
							  CW_USEDEFAULT, CW_USEDEFAULT, 200, 100, NULL,
							  NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Instance the audio-renderer.
	CA3dRenderer *pA3d = new CA3dRenderer;

	// TUTORIAL: INITIALIZE THE AUDIO-RENDERER.
	HRESULT hr = pA3d->CreateA3dRenderer(hWnd,					// Use application's window-handle for scope.
										 RENDER_A3D);			// Use A3D as renderer
	
	//
	// IMPORTANT NOTE!!!
	// You should ALWAYS check the return-codes for any calls.
	//
	if (hr < DS_OK)	// Check if we were successful
	{
		// Something went wrong in the initialization.
		pA3d->DestroyA3dRenderer();	// Cleanup before exiting.
		delete pA3d;
		pA3d = NULL;
		return(-1);
	}

	// Successfully created a 3D-audio renderer. Now create a listener and a sound source.
	CA3dListener *pA3dListener = new CA3dListener;
	CA3dSource	 *pA3dSourceA	= new CA3dSource;

	// TUTORIAL: CREATE AND INITIALIZE A 3D-LISTENER.
	if ((hr = pA3dListener->CreateListener(pA3d->m_p3DAudioRenderer)) != DS_OK)
	{
		pA3dListener->DestroyListener();	// Cleanup before exiting.
		pA3d->DestroyA3dRenderer();		

		delete pA3dListener;
		delete pA3d;
		pA3dListener = NULL;
		pA3d			 = NULL;
		return(-1);
	}
	
	// NB: When setting values to their defaults, this is simply to illustrate the specific call.
	//		 Default values, of course, are set "by default" implicitly.

	// TUTORIAL: SET LISTENER POSITION TO AN ARBITRARY POSITION OF (0, 0, -5).
	if ((pA3dListener) && (pA3dListener->m_pDS3DListener))
		pA3dListener->m_pDS3DListener->SetPosition(0.0f, 0.0f, -5.0f, DS3D_IMMEDIATE);
	
	// TUTORIAL: SET LISTENER ORIENTATION TO DEFAULT OF (0, 0, 1)-FRONT, (0, 1, 0)-TOP.
	if ((pA3dListener) && (pA3dListener->m_pDS3DListener))
		pA3dListener->m_pDS3DListener->SetOrientation(0.0f, 0.0f, 1.0f,	// Normalized front-vector. 
																	 0.0f, 1.0f, 0.0f,	// Normalized top-vector.
																	 DS3D_IMMEDIATE);
	
	// TUTORIAL: SET LISTENER VELOCITY FOR STATIONARY LISTENER.
	// Note that the velocity setting is independent of position.
	// The listener may have a velocity and not actually be changing position.
	// This allows for special effects to be applied. The same applies for 3d-sources.
	if ((pA3dListener) && (pA3dListener->m_pDS3DListener))
		pA3dListener->m_pDS3DListener->SetVelocity(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
	
	// TUTORIAL: SET ROLLOFF FACTOR TO ONE-TENTH NORMAL, TO SHOW SPECIAL EFFECT. (DISTANCE CUES.)
	// Adjust this value to see the effect of varying distance cues; 1.0 is the default value.
	if ((pA3dListener) && (pA3dListener->m_pDS3DListener))
		pA3dListener->m_pDS3DListener->SetRolloffFactor(0.1f, DS3D_IMMEDIATE);

	// TUTORIAL: SET DOPPLER FACTOR TO TWICE NORMAL, TO SHOW SPECIAL EFFECT. (VELOCITY CUES.)
	// Adjust this value to see the effect of varying velocity cues; 1.0 is the default value.
	if ((pA3dListener) && (pA3dListener->m_pDS3DListener))
		pA3dListener->m_pDS3DListener->SetDopplerFactor(0.1f, DS3D_IMMEDIATE);

	// TUTORIAL: CREATE AND INITIALIZE A 3D-SOURCE.
	// Make this source optimally-processed, looping, and play it immediately.
	if ((hr = pA3dSourceA->CreateSource(pA3d->m_p3DAudioRenderer, "Heli.wav",
													BUFFER_OPTIMAL, PLAY_IMMEDIATE, PLAY_LOOPING)) != DS_OK)
	{
		pA3dSourceA->DestroySource();		// Cleanup before exiting.
		pA3dListener->DestroyListener();
		pA3d->DestroyA3dRenderer();
		
		delete pA3dSourceA;
		delete pA3dListener;
		delete pA3d;
		pA3dSourceA	 = NULL;
		pA3dListener = NULL;
		pA3d			 = NULL;
		return(-1);
	}

	// Main application-loop.
	// This basic example code is simply to demonstrate audio functionality.

	// Debug output string.
	char szTmpDebug[50];

	// Setup some variables and values for having a source rotate around the listener.
	// Note: this is pretty inefficient, but is meant only to illustrate a simple motion.
	const float fRadius = 10.0f;
	const float fHeight = 5.0f;
	const float fRotate = 0.4f;
	const float fPI	  = 3.1415927f;

	static DWORD	 last_frame_time = GetCurrentTime();
	static float	 current_position = 0.0f;
	static D3DVALUE xPos = 0.0f;
	static D3DVALUE yPos = fHeight;
	static D3DVALUE zPos = fRadius;
	D3DVALUE			 xVel, yVel, zVel;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If exiting...
			if (msg.message == WM_QUIT)
			{
				// TUTORIAL: DESTROY SOURCE.
				if (pA3dSourceA)
				{
					pA3dSourceA->DestroySource();
					delete pA3dSourceA;
					pA3dSourceA = NULL;
				}

				// TUTORIAL: DESTROY LISTENER.
				if (pA3dListener)
				{
					pA3dListener->DestroyListener();
					delete pA3dListener;
					pA3dListener = NULL;
				}

				// TUTORIAL: DESTROY RENDERER.
				if (pA3d)
				{
					pA3d->DestroyA3dRenderer();
					delete pA3d;
					pA3d = NULL;
				}
				
				break;
			}
  
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// increment angular position.
			if (current_position > 360)
				current_position = 0.0f;
			else	
				current_position += fRotate;

			// Calculate the velocity and new position for each coordinate.
			xVel = xPos; // Temporary storage of previous position.
			yVel = yPos;
			zVel = zPos;
			
			// Calculate frame-time in seconds.
			float time_elapsed = (GetCurrentTime() - last_frame_time) / 1000.0f;

			// Rotate sound source around the head; calculate new position of the source.
			xPos = fRadius * (float) cos((current_position / 180.0f * fPI));
			yPos = fHeight;
			zPos = fRadius * (float) sin((current_position / 180.0f * fPI));
			
			// Update velocity.
			xVel = (xPos - xVel) / time_elapsed;
			yVel = (yPos - yVel) / time_elapsed;
			zVel = (zPos - zVel) / time_elapsed;

			// Update this frame-time as last frame-time.
			last_frame_time = GetCurrentTime();

			// TUTORIAL: POSITION A SOURCE IN 3-SPACE.
			if ((pA3dSourceA) && (pA3dSourceA->m_pDS3DBuffer3D))
				pA3dSourceA->m_pDS3DBuffer3D->SetPosition(xPos, yPos, zPos, DS3D_IMMEDIATE);
			
			// TUTORIAL: SET SOURCE ORIENTATION TO DEFAULT OF (0, 0, 1)-FRONT, (0, 1, 0)-TOP.
			// Setting the source-orientation has no effect unless the source is directional.
			// Examples of this can be seen in Tutorial-Three under cone-handling calls.
			if ((pA3dSourceA) && (pA3dSourceA->m_pDS3DBuffer3D))
				pA3dSourceA->m_pDS3DBuffer3D->SetConeOrientation(0.0f, 0.0f, 1.0f,	// Normalized-vector. 
																				 DS3D_IMMEDIATE);
	
			// TUTORIAL: SET A SOURCE-VELOCITY IN 3-SPACE.
			// Note that the velocity setting is independent of position.
			// The 3d-source may have a velocity and not actually be changing position.
			// This allows for special effects to be applied. The same applies for the listener.
			if ((pA3dSourceA) && (pA3dSourceA->m_pDS3DBuffer3D))
				pA3dSourceA->m_pDS3DBuffer3D->SetVelocity(xVel, yVel, zVel, DS3D_IMMEDIATE);
			
			// Enable debug of positions and velocities if interested in seeing them change.
			BOOL bDebug = FALSE;
			if(bDebug)
			{
				sprintf(szTmpDebug, "XPos: %3.2f YPos: %3.2f ZPos: %3.2f\n", xPos, yPos, zPos);
				OutputDebugString(szTmpDebug);
				sprintf(szTmpDebug, "XVel: %3.2f YVel: %3.2f ZVel: %3.2f\n\n", xVel, yVel, zVel);
				OutputDebugString(szTmpDebug);
			}
		}
	}

	return (msg.wParam);
}


}	// extern "C".

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
