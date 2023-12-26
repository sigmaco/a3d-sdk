/////////////////////////////////////////////////////////////////////////////
// WinMain.cpp
// ===========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to demonstrate the use of Volumetric Sources through
//		the A3D interface.
//
// Remarks:
//		Application Movement Keys:
//			ESC		- Quits the program.
//			W		- Move Forward
//			S		- Move Backward
//			A		- Strafe Left
//			D		- Strafe Right
//			Mouse	- Turns Left or Right
//
//		Pressing the left mouse button will render the grandstand as a point
//		source instead of a volumetric source.  To bring up the menu commands
//		press the right mouse button.
/////////////////////////////////////////////////////////////////////////////

#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "dx\D3DFrame.h"
#include "dx\D3DEnum.h"
#include "dx\D3DUtil.h"
#include "resource.h"

enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHTOSOFTWARE };

/////////////////////////////////////////////////////////////////////////////
// Function prototypes and global (or static) variables
//
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int     CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);
HRESULT Initialize3DEnvironment(HWND);
HRESULT Change3DEnvironment(HWND);
void	Render3DEnvironment(void);
void    Cleanup3DEnvironment(void);
void    DisplayFrameworkError(HRESULT, APPMSGTYPE);
void    AppShowStats(void);
void    AppOutputText(LPDIRECT3DDEVICE3, DWORD, DWORD, LPSTR);
void    AppPause(BOOL);

/////////////////////////////////////////////////////////////////////////////
// External function-prototypes
//
HRESULT App_ConfirmDevice(LPDDCAPS, LPD3DDEVICEDESC);
HRESULT	App_OneTimeSceneInit(HWND);
void    App_DeleteDeviceObjects(void);
HRESULT App_InitDeviceObjects(LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3);
void	App_FrameMove(void);
void	App_Render(LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3, LPD3DRECT);
void	App_FinalCleanup(void);
void	App_OnKey(TCHAR chKey, BOOL bDown);


/////////////////////////////////////////////////////////////////////////////
// Global variables for using the D3D sample framework class
//
CD3DFramework* g_pFramework        = NULL;
BOOL           g_bActive           = FALSE;
BOOL           g_bReady            = FALSE;
BOOL           g_bFrameMoving      = TRUE;
BOOL           g_bSingleStep       = FALSE;
BOOL           g_bWindowed         = TRUE;
BOOL           g_bShowStats        = TRUE;
RECT           g_rcWindow;
int			   g_WindowCenter_x;
int			   g_WindowCenter_y;
DWORD          g_dwBaseTime;
DWORD          g_dwPauseTime;
extern BOOL    g_bAppUseZBuffer;
extern BOOL    g_bAppUseBackBuffer;
extern TCHAR*  g_strAppTitle;
extern BOOL    g_bDoVolume;
extern BOOL	   g_bUseTexture;




/////////////////////////////////////////////////////////////////////////////
// WinMain()
// =========
//
// Description:
//		The WinMain function is called by the system as the initial entry
//		point for a Win32-based application.
//
// Parameters:
//		HINSTANCE hInstance:
//			Handle to the current instance of the application.
//
//		HINSTANCE hPrevInstance:
//			Handle to the previous instance of the application.  For a Win32-
//			based application, this parameter is always NULL.
//
//		LPSTR lpCmdLine:
//			Pointer to a null-terminated string specifying the command line
//			for the application, excluding the program name.  To retrieve the
//			entire command line, use the GetCommandLine() function.
//
//		int nCmdShow:
//			Specifies how the window is to be shown.  This parameter can be
//			any one of the values found in the WinMain() documenation.
//
// Returns: 
//		If the function succeeds, terminating when it receives a WM_QUIT
//		message, it should return the exit value contained in that message's
//		wParam parameter.  If the function terminates before entering the
//		message loop, it should return zero.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class
    char szWindowClass[] = { TEXT("Render Window") };
	WNDCLASS wndClass;
	
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndClass.lpszClassName = szWindowClass;
    
	if(RegisterClass(&wndClass) == 0)
	{
		DisplayFrameworkError(E_FAIL, MSGERR_APPMUSTEXIT);
		return 0;
	}

    // Create our main window.
    HWND hWnd = CreateWindow(szWindowClass, g_strAppTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, 0L, 0L, hInstance, 0L);
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    // Save the window size/pos for switching modes.
    GetWindowRect(hWnd, &g_rcWindow);
	g_WindowCenter_x = (g_rcWindow.left + g_rcWindow.right) / 2;
	g_WindowCenter_y = (g_rcWindow.top  + g_rcWindow.bottom) / 2;
	SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);

    // Load the keyboard accelerators.
    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAIN_ACCEL));

    // Enumerate available D3D devices, passing a callback that allows devices
    // to be accepted/rejected based on what capabilities the app requires.
    HRESULT hResult = D3DEnum_EnumerateDevices(App_ConfirmDevice);
    if(FAILED(hResult))
    {
        DisplayFrameworkError(hResult, MSGERR_APPMUSTEXIT);
        return 0;
    }

    // Check if we could not get a device that renders into a window, which
    // means the display must be 16- or 256-color mode. If so, let's bail.
    D3DEnum_DriverInfo* pDriverInfo = NULL;
    D3DEnum_DeviceInfo* pDeviceInfo = NULL;
    D3DEnum_GetSelectedDriver(&pDriverInfo, &pDeviceInfo);
    if(!pDeviceInfo->bWindowed)
    {
        Cleanup3DEnvironment();
        DisplayFrameworkError(D3DFWERR_INVALIDMODE, MSGERR_APPMUSTEXIT);
        return 0;
    }

    // Initialize the 3D environment for the application.
    hResult = Initialize3DEnvironment(hWnd);
	if(FAILED(hResult))
    {
        Cleanup3DEnvironment();
        DisplayFrameworkError(hResult, MSGERR_APPMUSTEXIT);
        return 0;
    }

    // Setup the app so it can support single-stepping.
    g_dwBaseTime = timeGetTime();

    // Now we're ready to recieve and process Windows messages.
    BOOL bGotMsg;
    MSG msg;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    g_bReady = TRUE;

    while(WM_QUIT != msg.message)
    {
        // Use PeekMessage() if the app is active, so we can use idle time to
        // render the scene. Else, use GetMessage() to avoid eating CPU time.
        if(g_bActive)
		{
            bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		}
        else
		{
			bGotMsg = GetMessage(&msg, NULL, 0U, 0U);
		}

        if(bGotMsg)
        {
            // Translate and dispatch the message
            if(!TranslateAccelerator(hWnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Render a frame during idle time (no messages are waiting)
            if(g_bActive && g_bReady)
            {
                Render3DEnvironment();
            }
        }
    }

    return msg.wParam;
}


/////////////////////////////////////////////////////////////////////////////
// CALLBACK WndProc()
// ==================
//
// Description:
//		The WindowProc function is an application-defined function that
//		processes messages sent to a window.  The WNDPROC type defines a
//		pointer to this callback function in the WNDCLASS structure for the
//		RegisterWindow() function. 
//
// Parameters:
//		HWND hWnd:
//			Handle to the window.
//
//		UINT uMsg:
//			Specifies the message.
//
//		WPARAM wParam:
//			Specifies additional message information. The contents of this
//			parameter depend on the value of the uMsg parameter.
//
//		LPARAM lParam:
//			Specifies additional message information. The contents of this
//			parameter depend on the value of the uMsg parameter.
//
// Returns: 
//		The return value is the result of the message processing and depends
//		on the message sent.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
		case WM_LBUTTONDOWN:
			g_bDoVolume = FALSE;
			break;

		case WM_LBUTTONUP:
			g_bDoVolume = TRUE;
			break;

        case WM_PAINT:
            if(g_pFramework)
            {
                // If we are paused, and in fullscreen mode, give the dialogs
                // a GDI surface to draw on.
                if(!g_bReady && !g_bWindowed)
				{
                    g_pFramework->FlipToGDISurface(TRUE);
				}
                else
				{
					// Simply repaint the frame's contents.
                    g_pFramework->ShowFrame();
				}
            }
            break;

        case WM_MOVE:
            if(g_bActive && g_bReady && g_bWindowed)
            {
                GetWindowRect(hWnd, &g_rcWindow);

				g_WindowCenter_x = (g_rcWindow.left + g_rcWindow.right) / 2;
				g_WindowCenter_y = (g_rcWindow.top  + g_rcWindow.bottom) / 2;
				SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);
                
				g_pFramework->Move((SHORT)LOWORD(lParam), (SHORT)HIWORD(lParam));
            }
            break;

        case WM_SIZE:
            // Check to see if we are losing our window...
            if(wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED)
			{
                g_bActive = FALSE;
			}
            else
			{
				g_bActive = TRUE;
			}

            // A new window size will require a new viewport and backbuffer
            // size, so the 3D structures must be changed accordingly.
            if(g_bActive && g_bReady && g_bWindowed)
            {
                g_bReady = FALSE;
                GetWindowRect(hWnd, &g_rcWindow);
				
				g_WindowCenter_x = (g_rcWindow.left + g_rcWindow.right) / 2;
				g_WindowCenter_y = (g_rcWindow.top  + g_rcWindow.bottom) / 2;
				SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);
                
				Change3DEnvironment(hWnd);
                g_bReady = TRUE;
            }
            break;

        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
            break;

        case WM_SETCURSOR:
            if(g_bActive && g_bReady)
            {
                SetCursor(NULL);
                return TRUE;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;
        
        case WM_DESTROY:
            Cleanup3DEnvironment();
            PostQuitMessage(0);
            return 0L;

        case WM_ENTERMENULOOP:
            if(g_bFrameMoving)
			{
                g_dwPauseTime = timeGetTime();
			}

            AppPause(TRUE);
            break;

        case WM_EXITMENULOOP:
            if(g_bFrameMoving)
			{
                g_dwBaseTime += timeGetTime() - g_dwPauseTime;
			}

            AppPause(FALSE);
            break;

        case WM_CONTEXTMENU:
            {
                // Display the context menu.
				HMENU hMenu = LoadMenu(0, MAKEINTRESOURCE(IDR_POPUP));
                TrackPopupMenuEx(GetSubMenu(hMenu, 0), TPM_VERTICAL, LOWORD(lParam), HIWORD(lParam), hWnd, NULL);
            }
            break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			App_OnKey(TCHAR(wParam), TRUE);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			App_OnKey(TCHAR(wParam), FALSE);
			break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case SC_MONITORPOWER:
                    // Prevent potential crashes when the monitor powers down
                    return 1;

                case IDM_TOGGLESTART:
                    g_bFrameMoving = !g_bFrameMoving;

                    if(g_bFrameMoving)
					{
                        g_dwBaseTime += timeGetTime() - g_dwPauseTime;
					}
                    else
					{
                        g_dwPauseTime = timeGetTime();
					}
                    break;

                case IDM_SINGLESTEP:
                    if(g_bFrameMoving)
                    {
                        g_bFrameMoving = FALSE;
                        g_dwPauseTime = timeGetTime();
                    }
                    else
                    {
                        g_bSingleStep = TRUE;
                        g_dwBaseTime += timeGetTime() - (g_dwPauseTime + 100);
                        g_dwPauseTime = timeGetTime();
                    }
                    break;

                case IDM_CHANGEDEVICE:
                    // Display the driver-selection dialog box.
                    if(g_bActive && g_bReady)
                    {
                        AppPause(TRUE);
                        if(g_bWindowed)
						{
                            GetWindowRect(hWnd, &g_rcWindow);

							g_WindowCenter_x = (g_rcWindow.left + g_rcWindow.right) / 2;
							g_WindowCenter_y = (g_rcWindow.top  + g_rcWindow.bottom) / 2;
							SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);
						}

                        if(D3DEnum_UserDlgSelectDriver(hWnd, g_bWindowed) == IDOK)
                        {
                            D3DEnum_DriverInfo* pDriverInfo = NULL;
                            D3DEnum_DeviceInfo* pDeviceInfo = NULL;
                            D3DEnum_GetSelectedDriver(&pDriverInfo, &pDeviceInfo);
                            
							g_bWindowed = pDeviceInfo->bWindowed;
                            Change3DEnvironment(hWnd);
                        }

                        AppPause(FALSE);
                    }

                    return 0;

                case IDM_TOGGLEFULLSCREEN:
                    // Toggle the fullscreen/window mode
                    if(g_bActive && g_bReady)
                    {
                        g_bReady = FALSE;
                        if(g_bWindowed)
						{
                            GetWindowRect(hWnd, &g_rcWindow);

							g_WindowCenter_x = (g_rcWindow.left + g_rcWindow.right) / 2;
							g_WindowCenter_y = (g_rcWindow.top  + g_rcWindow.bottom) / 2;
							SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);
						}

                        g_bWindowed = !g_bWindowed;
                        Change3DEnvironment(hWnd);
                        
						g_bReady = TRUE;
                    }
                    return 0;

                case IDM_HELP:
                    AppPause(TRUE);
                    DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutProc);
                    AppPause(FALSE);
                    return 0;

                case IDM_EXIT:
                    // Recieved key/menu command to exit app
                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0;
            }
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
            

/////////////////////////////////////////////////////////////////////////////
// CALLBACK AboutProc()
// ====================
//
// Description:
//		The DialogProc function is an application-defined callback function
//		used with the DialogBox() function.  It processes messages sent to a
//		modal or modeless dialog box.  The DLGPROC type defines a pointer to
//		this callback function.
//
// Parameters:
//		HWND hWndDialog:
//			Handle to the dialog box.
//
//		UINT uMsg:
//			Specifies the message.
//
//		WPARAM wParam:
//			Specifies additional message-specific information.
//
//		LPARAM lParam:
//			Specifies additional message-specific information.
//
// Returns: 
//		TRUE:
//			This dialog box procedure processed the message.
//
//		FALSE:
//			This dialog box procedure did not process the message.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK AboutProc(HWND hWndDialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_COMMAND)
	{
        if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
            EndDialog(hWndDialog, TRUE);
		}
	}

    return (uMsg == WM_INITDIALOG) ? TRUE : FALSE;
}




/////////////////////////////////////////////////////////////////////////////
// NOTE:	From this point on, the code is DirectX specific support for the
//			application.
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// AppInitialize()
// ===============
//
// Description:
//		Initializes the sample framework and calls the specific functions to
//		initialize device objects.
//
// Parameters:
//		HWND hWnd:
//			Description of the parameter here.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		Return Values from:
//			CD3DFramework::Initialize()
//			App_InitDeviceObjects()
//
// Remarks:
//		This function is recursive if it attempts to use a software renderer.
/////////////////////////////////////////////////////////////////////////////
HRESULT AppInitialize(HWND hWnd)
{
    D3DEnum_DriverInfo* pDriverInfo = NULL;
    D3DEnum_DeviceInfo* pDeviceInfo = NULL;
    D3DEnum_GetSelectedDriver( &pDriverInfo, &pDeviceInfo );

    DWORD dwFrameworkFlags = 0;
    dwFrameworkFlags |= (!g_bWindowed         ? D3DFW_FULLSCREEN : 0);
    dwFrameworkFlags |= ( g_bAppUseZBuffer    ? D3DFW_ZBUFFER    : 0);
    dwFrameworkFlags |= ( g_bAppUseBackBuffer ? D3DFW_BACKBUFFER : 0);

    // Initialize the D3D framework
    HRESULT hResult = g_pFramework->Initialize(hWnd, &pDriverInfo->guid, &pDeviceInfo->guid, &pDeviceInfo->pCurrentMode->ddsd, dwFrameworkFlags);
    if(SUCCEEDED(hResult))
    {
        // Let the app run its startup code which creates the 3D scene.
        hResult = App_InitDeviceObjects(g_pFramework->GetD3DDevice(), g_pFramework->GetViewport());
		if(SUCCEEDED(hResult))
		{
			// If the device is software, dont do texture.
			if(pDeviceInfo->bIsHardware)
			{
				g_bUseTexture = TRUE;
			}
			else
			{
				g_bUseTexture = FALSE;
			}

            return S_OK;
		}
        else
        {
            App_DeleteDeviceObjects();
            g_pFramework->DestroyObjects();
        }
    }

    // If we get here, the first initialization passed failed. If that was with a
    // hardware device, try again using a software rasterizer instead.
    if(pDeviceInfo->bIsHardware)
    {
        // Try again with a software rasterizer
        DisplayFrameworkError(hResult, MSGWARN_SWITCHTOSOFTWARE);
        D3DEnum_SelectDefaultDriver(D3DENUM_SOFTWAREONLY);
        return AppInitialize(hWnd);
    }

    return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// Initialize3DEnvironment()
// =========================
//
// Description:
//		Called when the application window is initially created.  This
//		triggers creation of the remaining portion (the 3D stuff).
//
// Parameters:
//		HWND hWnd:
//			Handle to the window.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		E_OUTOFMEMORY:
//			Failed to allocate memory for the CD3DFramework class.
//
//		Return Values from:
//			App_OneTimeSceneInit()
//			AppInitialize()
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT Initialize3DEnvironment(HWND hWnd)
{
    HRESULT hResult = S_OK;
	
	try
	{
		// Initialize the application scene.
		hResult = App_OneTimeSceneInit(hWnd);
		if(FAILED(hResult))
		{
			throw "Failed App_OneTimeSceneInit()";
		}

		// Create a new CD3DFramework class.  This class does all of our D3D
		// initialization and manages the common D3D objects.
		g_pFramework = new CD3DFramework;
		if(g_pFramework == NULL)
		{
			hResult = E_OUTOFMEMORY;
			throw "Failed to allocate memory for the D3D Framework class";
		}

		// Initialize the framework and scene.
		hResult = AppInitialize(hWnd);
	}
	catch(char* szError)
	{
		// Display the message.
		char szMsg[256];
		wsprintf(szMsg, "Initialize3DEnvironment() - %s.\n", szError);
		MessageBox(NULL, szMsg, "A3D Sample", MB_OK | MB_ICONEXCLAMATION);
	}

    return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// Change3DEnvironment()
// =====================
//
// Description:
//		Handles driver, device and/or mode changes for the application.
//
// Parameters:
//		HWND hWnd:
//			Handle to the window.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		Return Values From:
//			CD3DFramework::DestroyObjects()
//			AppInitialize()
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT Change3DEnvironment(HWND hWnd)
{
    HRESULT hResult = S_OK;

	try
	{
		// Release all objects that need to be re-created for the new device.
		App_DeleteDeviceObjects();

		// Release the current framework objects (they will be recreated
		// later on).
		hResult = g_pFramework->DestroyObjects();
		if(FAILED(hResult))
		{
			throw MSGERR_APPMUSTEXIT;
		}

		// In case we're coming from a fullscreen mode, restore the window
		// size.
		if(g_bWindowed)
		{
			SetWindowPos(hWnd, HWND_NOTOPMOST, g_rcWindow.left, g_rcWindow.top,
						  (g_rcWindow.right - g_rcWindow.left), 
						  (g_rcWindow.bottom - g_rcWindow.top), SWP_SHOWWINDOW);
		}

		// Inform the framework class of the driver change. It will internally
		// recreate valid surfaces, a d3ddevice, and a viewport.
		hResult = AppInitialize(hWnd);
		if(FAILED(hResult))
		{
			throw MSGERR_APPMUSTEXIT;
		}

		// If the app is paused, trigger the rendering of the current frame.
		if(!g_bFrameMoving)
		{
			g_bSingleStep = TRUE;
			g_dwBaseTime += timeGetTime() - g_dwPauseTime;
			g_dwPauseTime = timeGetTime();
		}
	}
	catch(APPMSGTYPE appMsg)
	{
		// Display the error and shutdown.
		DisplayFrameworkError(hResult, appMsg);
		DestroyWindow(hWnd);
	}

    return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// Render3DEnvironment()
// =====================
//
// Description:
//		Renders the 3D scene.
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
void Render3DEnvironment(void)
{
    // Check the cooperative level before rendering.
    if(FAILED(g_pFramework->GetDirectDraw()->TestCooperativeLevel()))
	{
        return;
	}

    // Get the relative time, in seconds.
    FLOAT fTime = (timeGetTime() - g_dwBaseTime ) * 0.001f;

    // FrameMove (animate) the scene.
    if(g_bFrameMoving || g_bSingleStep)
    {
        App_FrameMove();
        g_bSingleStep = FALSE;
    }

    // Render the scene.
    App_Render(g_pFramework->GetD3DDevice(), g_pFramework->GetViewport(), (LPD3DRECT)g_pFramework->GetViewportRect());
    
    // Show the frame rate, etc...
    if(g_bShowStats)
	{
        AppShowStats();
	}

    // Show the frame on the primary surface.
    if(g_pFramework->ShowFrame() == DDERR_SURFACELOST)
    {
        g_pFramework->RestoreSurfaces();
    }
}


/////////////////////////////////////////////////////////////////////////////
// Cleanup3DEnvironment()
// ======================
//
// Description:
//		Cleans up the scene objects.
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
void Cleanup3DEnvironment(void)
{
    if(g_pFramework)
    {
        App_FinalCleanup();
        SAFE_DELETE(g_pFramework);
    }

    D3DEnum_FreeResources();
    
	g_bActive = FALSE;
    g_bReady  = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// AppPause()
// ==========
//
// Description:
//		Called in to toggle the pause state of the application.
//
// Parameters:
//		BOOL bPause:
//			TRUE if the application is paused, FALSE otherwise.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		This function brings the GDI surface to the front of the display so
//		drawing output like message boxes and menus may be displayed.
/////////////////////////////////////////////////////////////////////////////
void AppPause(BOOL bPause)
{
    static DWORD dwAppPausedCount = 0;

    if(bPause && (dwAppPausedCount == 0))
	{
        if(g_pFramework)
		{
            g_pFramework->FlipToGDISurface(TRUE);
		}
	}

    dwAppPausedCount += (bPause ? +1 : -1);
    g_bReady = (dwAppPausedCount == 0);
}


/////////////////////////////////////////////////////////////////////////////
// AppShowStats()
// ==============
//
// Description:
//		Shows frame rate and dimensions of the rendering device.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		A real application would not query for the dimensions of the render
//		surface every frame.
/////////////////////////////////////////////////////////////////////////////
void AppShowStats(void)
{
    static FLOAT fFPS = 0.0f;
    static FLOAT fLastTime = 0.0f;
    static DWORD dwFrames = 0;

    // Keep track of the time lapse and frame count.
    FLOAT fTime = timeGetTime() * 0.001f; // Get current time in seconds
    ++dwFrames;

    // Update the frame rate once per second.
    if(fTime - fLastTime > 1.0f)
    {
        fFPS = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames = 0;
    }

    // Get dimensions of the render surface 
    DDSURFACEDESC2 ddSurfaceDesc;
    ddSurfaceDesc.dwSize = sizeof(ddSurfaceDesc);
    g_pFramework->GetRenderSurface()->GetSurfaceDesc(&ddSurfaceDesc);

    // Setup the text buffer to write out
    CHAR szBuffer[80];
	sprintf(szBuffer, "%7.02f FPS (%dx%dx%d) %s", 
			fFPS, 
			ddSurfaceDesc.dwWidth,
			ddSurfaceDesc.dwHeight, 
			ddSurfaceDesc.ddpfPixelFormat.dwRGBBitCount,
			g_bDoVolume ? "Vol Src" : "Point Src");

    // Display it on the device.
	AppOutputText(g_pFramework->GetD3DDevice(), 0, 0, szBuffer);
}


/////////////////////////////////////////////////////////////////////////////
// AppOutputText()
// ===============
//
// Description:
//		Draws text on the window.
//
// Parameters:
//		LPDIRECT3DDEVICE3 lpD3dDevice:
//			Pointer to a Direct3D device to render the scene on.
//
//		DWORD x:
//			The X coordinate that the text should start at.
//
//		DWORD y:
//			The Y coordinate that the text should start at.
//
//		LPSTR lpStr:
//			The text string to render on the device.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void AppOutputText(LPDIRECT3DDEVICE3 lpD3dDevice, DWORD x, DWORD y, LPSTR lpStr)
{
    // Get the DirectDraw surface to render to.
	LPDIRECTDRAWSURFACE4 lpDDSRenderSurface = NULL;
    if(FAILED(lpD3dDevice->GetRenderTarget(&lpDDSRenderSurface)))
	{
        return;
	}

    // Get a DC for the surface. Then, write out the buffer to it.
    HDC hDC = NULL;
    if(SUCCEEDED(lpDDSRenderSurface->GetDC(&hDC)))
    {
        SetTextColor(hDC, RGB(255, 255, 0));
        SetBkMode(hDC, TRANSPARENT);
        ExtTextOut(hDC, x, y, 0, NULL, lpStr, strlen(lpStr), NULL);
    
        lpDDSRenderSurface->ReleaseDC(hDC);
    }

	lpDDSRenderSurface->Release();
}




//-----------------------------------------------------------------------------
// Name: DisplayFrameworkError()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
void DisplayFrameworkError(HRESULT hResult, APPMSGTYPE errType)
{
    CHAR strMsg[512];

    switch(hResult)
    {
        case D3DENUMERR_NOCOMPATIBLEDEVICES:
            strcpy(strMsg, TEXT("Could not find any compatible Direct3D devices."));
            break;

        case D3DENUMERR_SUGGESTREFRAST:
            strcpy(strMsg, TEXT("Could not find any compatible devices.\n\n"
                    "Try enabling the reference rasterizer using\n"
                    "EnableRefRast.reg."));
            break;

        case D3DENUMERR_ENUMERATIONFAILED:
            strcpy(strMsg, TEXT("Enumeration failed. Your system may be in an\n"
                    "unstable state and need to be rebooted"));
            break;

        case D3DFWERR_INITIALIZATIONFAILED:
            strcpy(strMsg, TEXT("Generic initialization error.\n\nEnable "
                    "debug output for detailed information."));
            break;

        case D3DFWERR_NODIRECTDRAW:
            strcpy(strMsg, TEXT("No DirectDraw"));
            break;

        case D3DFWERR_NODIRECT3D:
            strcpy(strMsg, TEXT("No Direct3D"));
            break;

        case D3DFWERR_INVALIDMODE:
            strcpy(strMsg, TEXT("This sample requires a 16-bit (or higher) "
                    "display mode\nto run in a window.\n\nPlease switch "
                    "your desktop settings accordingly."));
            break;

        case D3DFWERR_COULDNTSETCOOPLEVEL:
            strcpy(strMsg, TEXT("Could not set Cooperative Level"));
            break;

        case D3DFWERR_NO3DDEVICE:
            strcpy(strMsg, TEXT("No 3D Device"));
            break;

        case D3DFWERR_NOZBUFFER:
            strcpy(strMsg, TEXT("No ZBuffer"));
            break;

        case D3DFWERR_NOVIEWPORT:
            strcpy(strMsg, TEXT("No Viewport"));
            break;

        case D3DFWERR_NOPRIMARY:
            strcpy(strMsg, TEXT("No primary"));
            break;

        case D3DFWERR_NOCLIPPER:
            strcpy(strMsg, TEXT("No Clipper"));
            break;

        case D3DFWERR_BADDISPLAYMODE:
            strcpy(strMsg, TEXT("Bad display mode"));
            break;

        case D3DFWERR_NOBACKBUFFER:
            strcpy(strMsg, TEXT("No backbuffer"));
            break;

        case D3DFWERR_NONZEROREFCOUNT:
            strcpy(strMsg, TEXT("Nonzero Ref Count"));
            break;

        case D3DFWERR_NORENDERTARGET:
            strcpy(strMsg, TEXT("No render target"));
            break;

        case E_OUTOFMEMORY:
            strcpy(strMsg, TEXT("Not enough memory!"));
            break;

        case DDERR_OUTOFVIDEOMEMORY:
            strcpy(strMsg, TEXT("There was insufficient video memory "
                    "to use the\nhardware device."));
            break;

        default:
            strcpy(strMsg, TEXT("Generic application error.\n\nEnable "
                    "debug output for detailed information."));
    }

    if(errType == MSGERR_APPMUSTEXIT)
    {
        strcat(strMsg, TEXT("\n\nThis sample will now exit."));
        MessageBox(NULL, strMsg, g_strAppTitle, MB_ICONERROR | MB_OK);
    }
    else
    {
        if(errType == MSGWARN_SWITCHTOSOFTWARE)
		{
            strcat(strMsg, TEXT("\n\nSwitching to software rasterizer."));
		}

        MessageBox(NULL, strMsg, g_strAppTitle, MB_ICONWARNING | MB_OK);
    }
}




