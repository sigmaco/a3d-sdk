/////////////////////////////////////////////////////////////////////////////
// SampleCommon.cpp
// ================
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Provides common functions that are used in many of the sample
//		applications provided with the SDK.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "samplecommon.h"


/////////////////////////////////////////////////////////////////////////////
// SampleCommon::GetConsoleHwnd()
// ==============================
//
// Description:
//		Retrieves the Window Handle for a Win32 console application.
//
// Parameters:
//		None.
//
// Returns: 
//		HWND:
//			Handle to the console window if found.  Otherwise, it will return
//			NULL.
//
// Remarks:
//		Win32 applications do not have access to the same selection of Win32
//		API's that native Win32 applications enjoy.  Determining the HWND for
//		a console application can done by have the Win32 system find the
//		window and return it's handle.
/////////////////////////////////////////////////////////////////////////////
HWND SampleCommon::GetConsoleHwnd(void)
{
    const int CHAR_BUF_SIZE = 256;

	// Get the current window title.
	char pszOldWindowTitle[CHAR_BUF_SIZE];
	GetConsoleTitle(pszOldWindowTitle, CHAR_BUF_SIZE);

	// Format a unique New Window Title.
	char pszNewWindowTitle[CHAR_BUF_SIZE];
	wsprintf(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());

	// Change the current window title.
	SetConsoleTitle(pszNewWindowTitle);

	// Ensure window title has changed.
	Sleep(40);

	// Look for the new window.
	HWND hWnd = FindWindow(NULL, pszNewWindowTitle);

	// Restore orignal name
	SetConsoleTitle(pszOldWindowTitle);
	return hWnd;
}
