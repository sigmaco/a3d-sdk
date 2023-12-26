//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//      T1: Tutorial One - Creating a 3D-Audio application.
//
//      Illustrates:    Creating an A3D render object.
//                                              Creating an A3D primary buffer.
//                                              Creating an A3D listener object.
//                                              Creating an A3D 3d-source buffer object.
//
//      Also contains:  2d-source buffer object creation.
//                                              Wavefile loading code.
//                                              Application-window creation code.
//
//      Created:                        25th November, 1996, by Suneil Mishra.
//                                              Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved.   
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
// FILE: t1.cpp Main application code for tutorial one.
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************/
/*                                                                                                                                                                                                                               */
/*      NB: CAPITALIZED COMMENTS REFER TO NECESSARY CALLS FOR 3D-AUDIO TO BE ENABLED.            */
/*               SEARCH FOR THE KEYWORD "TUTORIAL:" WITHIN THIS FILE FOR AUDIO EXAMPLE CODE.     */
/*                                                                                                                                                                                                                               */
/*************************************************************************************/


// Include header to 3d-audio definitions.
#include "..\Common\A3d.h"
#include <math.h>

extern "C"      // Cover the windowing calls for C++.
{

//////////////////////////////////////////////////////////////////////////////////////
// Application routines to create a window and display information/error messages.
//////////////////////////////////////////////////////////////////////////////////////

int  PASCAL                     WinMain(HANDLE hInstance, HANDLE hPrevInstance,
                                                                  LPSTR lpszCmdParam, int nCmdShow);
long FAR PASCAL WndProc(HWND hwnd, UINT message, UINT wParam, LONG lParam);


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

                        DrawText(hdc, "A3D: Tutorial One", -1, &rect,
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
        static char szAppName[] = "Tutorial-One";
        HWND hWnd;
        MSG msg;
        WNDCLASS wndclass;

        /* initialize IA3d library */

        A3dInitialize();

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

        hWnd = CreateWindow(szAppName, 
			                "A3D Tutorial", 
							WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							200, 
							100, 
							NULL,
                            NULL, 
							hInstance, 
							NULL);

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        // Instance the audio-renderer.
        CA3dRenderer *pA3d = new CA3dRenderer;

        // TUTORIAL: INITIALIZE THE AUDIO-RENDERER.
        HRESULT hr = pA3d->CreateA3dRenderer(hWnd,          // Use application's window-handle for scope.
                                             RENDER_A3D);   // Use A3D as renderer
                
        //
        // IMPORTANT NOTE!!!
        // You should ALWAYS check the return-codes for any calls.
        //
        if (FAILED(hr)) // Check if we were successful
        {
                // Something went wrong in the initialization.
                pA3d->DestroyA3dRenderer();     // Cleanup before exiting.
                delete pA3d;
                pA3d = NULL;
                return(-1);
        }

        // Successfully created a 3D-audio renderer. Now create a listener and a sound source.
        CA3dListener *pA3dListener = new CA3dListener;
        CA3dSource       *pA3dSourceA   = new CA3dSource;

        // TUTORIAL: CREATE AND INITIALIZE A 3D-LISTENER.
        if (FAILED(pA3dListener->CreateListener(pA3d->m_p3DAudioRenderer)))
        {
                pA3dListener->DestroyListener();        // Cleanup before exiting.
                pA3d->DestroyA3dRenderer();             

                delete pA3dListener;
                delete pA3d;
                pA3dListener = NULL;
                pA3d = NULL;
                return(-1);
        }
        
        // TUTORIAL: CREATE AND INITIALIZE A 3D-SOURCE.
        // Make this source optimally-processed, looping, and play it immediately.
        if ((hr = pA3dSourceA->CreateSource(pA3d->m_p3DAudioRenderer, "Heli.wav",
                                                                                                        BUFFER_OPTIMAL, PLAY_IMMEDIATE, PLAY_LOOPING)) != DS_OK)
        {
                pA3dSourceA->DestroySource();           // Cleanup before exiting.
                pA3dListener->DestroyListener();
                pA3d->DestroyA3dRenderer();
                
                delete pA3dSourceA;
                delete pA3dListener;
                delete pA3d;
                pA3dSourceA      = NULL;
                pA3dListener = NULL;
                pA3d                     = NULL;
                return(-1);
        }

        // Main application-loop.
        // This basic example code is simply to demonstrate audio functionality.
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
                        // For this first example, we do nothing. We are already playing the sound.
                }
        }

        /* Uninitialize when done */

        A3dUninitialize();

        return (msg.wParam);
}


}       // extern "C".

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
