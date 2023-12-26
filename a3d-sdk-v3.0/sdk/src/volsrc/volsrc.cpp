/////////////////////////////////////////////////////////////////////////////
// VolSrc.cpp
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
#define D3D_OVERLOADS

#include <math.h>
#include <time.h>
#include <stdio.h>
#include "dx\D3DTextr.h"
#include "dx\D3DUtil.h"
#include "dx\D3DMath.h"
#include "macro3d.h"

#include <initguid.h>
#include "ia3dapi.h"
#include "ia3dutil.h"


/////////////////////////////////////////////////////////////////////////////
// Declare the application globals for use in WinMain.cpp
//
TCHAR*	g_strAppTitle       = TEXT("A3D Volumetric Source Sample");
BOOL	g_bAppUseZBuffer    = TRUE;
BOOL	g_bAppUseBackBuffer = TRUE;
TCHAR	g_szAppStatus[64];

/////////////////////////////////////////////////////////////////////////////
// Externs declared in WinMain
//
extern int g_WindowCenter_x;
extern int g_WindowCenter_y;

/////////////////////////////////////////////////////////////////////////////
// Function prototypes and global (or static) variables
//
void	App_Pause(BOOL);
HRESULT	App_InitDeviceObjects(LPDIRECT3DDEVICE3, LPDIRECT3DVIEWPORT3);
void	App_DeleteDeviceObjects(void);
void	App_OnKey(TCHAR chKey, BOOL bDown);

LPDIRECT3DMATERIAL3	g_pmtrlObjectMtrl	= NULL;
LPDIRECT3DLIGHT		g_pLight			= NULL;

#define NUM_GRID			32
#define NUM_GRID_VERTICES	(NUM_GRID * NUM_GRID)
#define NUM_GRID_INDICES	((NUM_GRID - 1) * (NUM_GRID - 1) * 2 * 3)
#define GRID_WIDTH			200.0f

D3DVERTEX g_Grid[NUM_GRID][NUM_GRID];
WORD      g_GridIndices[NUM_GRID - 1][NUM_GRID - 1][2][3];

// Textures file names
#define CLOUD_BMP		"Cloud3.bmp"
#define TERRAIN_BMP		"Terrain.bmp"

#define NUM_GRANDSTAND_VERTICES		8
#define NUM_GRANDSTAND_TRIANGLES	12
#define NUM_GRANDSTAND_INDICES		(NUM_GRANDSTAND_TRIANGLES * 3)

// The grand stand dimensions
#define GS_WIDTH	20.0f
#define GS_HEIGHT	5.0f
#define GS_DEPTH	5.0f

D3DMATRIX g_GrandStandMat;
D3DVERTEX g_GrandStand[NUM_GRANDSTAND_VERTICES];
WORD      g_GrandStandIndices[NUM_GRANDSTAND_TRIANGLES][3];

// Add sign
#define OCCPOLY_WIDTH			5
#define OCCPOLY_HEIGHT			2
#define NUM_OCCPOLY_VERTICES	4
#define NUM_OCCPOLY_INDICES		(NUM_OCCPOLY_VERTICES * 3)

D3DMATRIX g_OccPolyMat;
D3DVERTEX g_OccPoly[NUM_OCCPOLY_VERTICES];
WORD	  g_OccPolyIndices[NUM_OCCPOLY_VERTICES][3];


// The bill board background
D3DTLVERTEX g_BackgroundMesh[4];
BOOL		g_bDoVolume = TRUE;
BOOL		g_bUseTexture = FALSE;

/////////////////////////////////////////////////////////////////////////////
// Camera controls
//
typedef struct CAMERA_s
{
	float  fYaw;
	float  fPitch;
	float  Pos[3];
	float  Vel[3];
} CAMERA;

CAMERA g_Camera;
BOOL   g_bForward;
BOOL   g_bBackward;
BOOL   g_bLeft;
BOOL   g_bRight;

D3DMATRIX g_matView;
D3DVECTOR g_up   = D3DVECTOR(0.0f, 1.0f, 0.0f);
D3DVECTOR g_dir;

/////////////////////////////////////////////////////////////////////////////
// A3D COM Interfaces.
//
IA3d5        *g_pA3d			= NULL;
IA3dSource2  *g_pA3dSource		= NULL;
IA3dGeom2	 *g_pA3dGeom		= NULL;
IA3dListener *g_pA3dListener	= NULL;



/////////////////////////////////////////////////////////////////////////////
// A3D / AUDIO CODE 
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// InitializeA3D()
// ===============
//
// Description:
//		Intializes A3D.
//
// Parameters:
//		HWND hWnd:
//			Handle to the application window.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		E_FAIL:
//
//		E_INVALIDARG:
//			One of the arguments was passed an invalid value.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT InitializeA3D(HWND hWnd)
{
	HRESULT hResult = S_OK;

	try
	{
		// Initialize A3D using the exported A3dCreate() function from the
		// A3DAPI.DLL
		hResult = A3dCreate(NULL, (void**)&g_pA3d, NULL, A3D_OCCLUSIONS | A3D_1ST_REFLECTIONS);
		if(FAILED(hResult))
		{
			throw "Failed to retrieve the A3D COM interface";
		}

		// If we don't call g_pA3d->InitEx() then we have to call manually
		// call SetCooperativeLevel().
		hResult = g_pA3d->SetCooperativeLevel(hWnd, A3D_CL_NORMAL);
		if(FAILED(hResult))
		{
			throw "Failed to Set the Cooperative Level";
		}

		// D3D uses left handed Coodirnate System for graphics.
		g_pA3d->SetCoordinateSystem(A3D_LEFT_HANDED_CS);

		// Query for all of the A3D interfaces that we will be using.
		hResult = g_pA3d->QueryInterface(IID_IA3dGeom2, (void **)&g_pA3dGeom);
		if(FAILED(hResult))
		{
			throw "Failed to retrieve the A3D Geometry interface";
		}

		// Enable Occlusions and Reflections.
		hResult = g_pA3dGeom->Enable(A3D_OCCLUSIONS | A3D_1ST_REFLECTIONS);
		if(FAILED(hResult))
		{
			throw "Failed to enable Occlusions and Reflections";
		}

		// Create a new source.  At this stage the source is empty - no wave data.
		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &g_pA3dSource);
		if(FAILED(hResult))
		{
			throw "Failed to create the A3D Source";
		}
			
		// Load the data
		hResult = g_pA3dSource->LoadFile("file0038.wav", A3DSOURCE_FORMAT_WAVE);
		if(FAILED(hResult))
		{
			throw "Failed to Load file0038.wav data into the A3D Source";
		}

		g_pA3dSource->SetMinMaxDistance(10.f, 100.f, FALSE);
		g_pA3dSource->Play(A3D_LOOPED);

		// Query for the listener object used for 3D sound positioning.
		hResult = g_pA3d->QueryInterface(IID_IA3dListener, (void **)&g_pA3dListener);
		if(FAILED(hResult))
		{
			throw "Failed to create listener";
		}
	}
	catch(char* szError)
	{
		// Display the message.
		char szMsg[256];
		wsprintf(szMsg, "AudioInit() - %s.\n", szError);
		MessageBox(hWnd, szMsg, "A3D Sample", MB_OK | MB_ICONEXCLAMATION);
	}

	return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// UpdateA3D()
// ===========
//
// Description:
//		Updates the A3D engine with positional data.
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
void UpdateA3D(void)
{
	// Clear from the last frame.
	g_pA3d->Clear();

	// Set the listener's position
	g_pA3dListener->SetPosition3fv(g_Camera.Pos);
	g_pA3dListener->SetOrientation6f(g_dir.x, g_dir.y, g_dir.z, g_up.x, g_up.y, g_up.z);

	// Render grandstand as a volumetric or point source?
	if(g_bDoVolume)
	{
		// Specify the full width of the vol src box.
		// Note the grandstand graphically is really a wedge shape, so just approximated
		// bounding volume.
		g_pA3dSource->SetVolumetricBounds(GS_WIDTH * 2.0f, GS_HEIGHT, GS_DEPTH * 2.0f);
	}
	else
	{
		// A point src has no volume.
		g_pA3dSource->SetVolumetricBounds(0.0f, 0.0f, 0.0f);
	}

    // Define the Occluding polygon.
	g_pA3dGeom->LoadIdentity();
	g_pA3dGeom->Translate3f(g_OccPolyMat._41,g_OccPolyMat._42,g_OccPolyMat._43);
	g_pA3dGeom->Begin(A3D_QUADS);
		g_pA3dGeom->Vertex3f(-OCCPOLY_WIDTH, -0.1f ,			0.0f);
		g_pA3dGeom->Vertex3f( OCCPOLY_WIDTH, -0.1f ,			0.0f);
		g_pA3dGeom->Vertex3f( OCCPOLY_WIDTH, OCCPOLY_HEIGHT,	0.0f);
		g_pA3dGeom->Vertex3f(-OCCPOLY_WIDTH, OCCPOLY_HEIGHT,	0.0f);
	g_pA3dGeom->End();

	g_pA3d->Flush();
}


///////////////////////////////////////////////////////////////////////////// 
// APPLICATION / GRAPHICS CODE 
///////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////
// void BuildBackground()
// ======================
//
// Description:
//		Fills data for the background cloud billboard.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
static void BuildBackground(void)
{
    // Create the cloud texture.
    D3DTextr_CreateTexture(CLOUD_BMP);

    g_BackgroundMesh[0] = D3DTLVERTEX(D3DVECTOR(0, 0, 0.99f), 0.5f, -1, 0, 0, 1);
    g_BackgroundMesh[1] = D3DTLVERTEX(D3DVECTOR(0, 0, 0.99f), 0.5f, -1, 0, 0, 0);
    g_BackgroundMesh[2] = D3DTLVERTEX(D3DVECTOR(0, 0, 0.99f), 0.5f, -1, 0, 1, 1);
    g_BackgroundMesh[3] = D3DTLVERTEX(D3DVECTOR(0, 0, 0.99f), 0.5f, -1, 0, 1, 0);
}


/////////////////////////////////////////////////////////////////////////////
// void BuildModels()
// ==================
//
// Description:
//		Fills the data for models in the scene.
//
// Parameters:
//		None.
//
// Returns: 
//		Nothing
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
static void BuildModels(void)
{
	// Bottom.
	g_GrandStand[0] = D3DVERTEX(D3DVECTOR( GS_WIDTH,        -0.1f , GS_DEPTH), D3DVECTOR(0, -1, 0), 0.0f, 0.0f);
	g_GrandStand[1] = D3DVERTEX(D3DVECTOR( GS_WIDTH * 1.5f, -0.1f ,-GS_DEPTH), D3DVECTOR(0, -1, 0), 0.0f, 0.0f);
	g_GrandStand[2] = D3DVERTEX(D3DVECTOR(-GS_WIDTH * 1.5f, -0.1f ,-GS_DEPTH), D3DVECTOR(0, -1, 0), 0.0f, 0.0f);
	g_GrandStand[3] = D3DVERTEX(D3DVECTOR(-GS_WIDTH,        -0.1f , GS_DEPTH), D3DVECTOR(0, -1, 0), 0.0f, 0.0f);

	// Top.
	g_GrandStand[4] = D3DVERTEX(D3DVECTOR( GS_WIDTH,        GS_HEIGHT        , GS_DEPTH), D3DVECTOR(0, 1, 0), 0.0f, 0.0f);
	g_GrandStand[5] = D3DVERTEX(D3DVECTOR( GS_WIDTH * 1.5f, GS_HEIGHT * 1.5f ,-GS_DEPTH), D3DVECTOR(0, 1, 0), 0.0f, 0.0f);
	g_GrandStand[6] = D3DVERTEX(D3DVECTOR(-GS_WIDTH * 1.5f, GS_HEIGHT * 1.5f ,-GS_DEPTH), D3DVECTOR(0, 1, 0), 0.0f, 0.0f);
	g_GrandStand[7] = D3DVERTEX(D3DVECTOR(-GS_WIDTH,        GS_HEIGHT        , GS_DEPTH), D3DVECTOR(0, 1, 0), 0.0f, 0.0f);

	// Indices.
	g_GrandStandIndices[0][0] = 0;	g_GrandStandIndices[0][1] = 2;	g_GrandStandIndices[0][2] = 1;
	g_GrandStandIndices[1][0] = 0;	g_GrandStandIndices[1][1] = 3;	g_GrandStandIndices[1][2] = 2;

	g_GrandStandIndices[2][0] = 0;	g_GrandStandIndices[2][1] = 1;	g_GrandStandIndices[2][2] = 5;
	g_GrandStandIndices[3][0] = 0;	g_GrandStandIndices[3][1] = 5;	g_GrandStandIndices[3][2] = 4;

	g_GrandStandIndices[4][0] = 3;	g_GrandStandIndices[4][1] = 7;	g_GrandStandIndices[4][2] = 6;
	g_GrandStandIndices[5][0] = 3;	g_GrandStandIndices[5][1] = 6;	g_GrandStandIndices[5][2] = 2;

	g_GrandStandIndices[6][0] = 1;	g_GrandStandIndices[6][1] = 2;	g_GrandStandIndices[6][2] = 6;
	g_GrandStandIndices[7][0] = 1;	g_GrandStandIndices[7][1] = 6;	g_GrandStandIndices[7][2] = 5;

	g_GrandStandIndices[8][0] = 4;	g_GrandStandIndices[8][1] = 5;	g_GrandStandIndices[8][2] = 6;
	g_GrandStandIndices[9][0] = 4;	g_GrandStandIndices[9][1] = 6;	g_GrandStandIndices[9][2] = 7;

	g_GrandStandIndices[10][0] = 0;	g_GrandStandIndices[10][1] = 4;	g_GrandStandIndices[10][2] = 7;
	g_GrandStandIndices[11][0] = 0;	g_GrandStandIndices[11][1] = 7;	g_GrandStandIndices[11][2] = 3;

	// Build the Occluding Polygon.
	g_OccPoly[0] = D3DVERTEX(D3DVECTOR(-OCCPOLY_WIDTH,	-0.1f,			0.0f), D3DVECTOR(0, 0, 1), 0.0f, 0.0f);
	g_OccPoly[1] = D3DVERTEX(D3DVECTOR( OCCPOLY_WIDTH,	-0.1f,			0.0f), D3DVECTOR(0, 0, 1), 1.0f, 0.0f);
	g_OccPoly[2] = D3DVERTEX(D3DVECTOR( OCCPOLY_WIDTH,	OCCPOLY_HEIGHT,	0.0f), D3DVECTOR(0, 0, 1), 1.0f, 1.0f);
	g_OccPoly[3] = D3DVERTEX(D3DVECTOR(-OCCPOLY_WIDTH,	OCCPOLY_HEIGHT,	0.0f), D3DVECTOR(0, 0, 1), 0.0f, 1.0f);

	// A single poly face, front and back.
	g_OccPolyIndices[0][0] = 0; g_OccPolyIndices[0][1] = 1; g_OccPolyIndices[0][0] = 2;
	g_OccPolyIndices[1][0] = 0; g_OccPolyIndices[1][1] = 2; g_OccPolyIndices[1][0] = 3;
	g_OccPolyIndices[2][0] = 0; g_OccPolyIndices[2][1] = 2; g_OccPolyIndices[2][0] = 1;
	g_OccPolyIndices[3][0] = 0; g_OccPolyIndices[3][1] = 3; g_OccPolyIndices[3][0] = 2;

	// Place the add poly
	D3DUtil_SetTranslateMatrix(g_OccPolyMat, 5.0f ,0.0f, 15.0f);
}


/////////////////////////////////////////////////////////////////////////////
// void BuildGrid()
// ================
//
// Description:
//		Builds the square grid on the ground.
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
static void BuildGrid(void)
{
	for(WORD i = 0; i < NUM_GRID; i++)
	{
		for(WORD j = 0; j < NUM_GRID; j++)
		{
			FLOAT x = i * GRID_WIDTH / (NUM_GRID - 1) - GRID_WIDTH / 2;
			FLOAT z = j * GRID_WIDTH / (NUM_GRID - 1) - GRID_WIDTH / 2;
			
			g_Grid[i][j] = D3DVERTEX(D3DVECTOR(x, 0, z), D3DVECTOR(0, 1, 0), j / (FLOAT)NUM_GRID, i / (FLOAT)NUM_GRID);
		}
	}

	for(i = 0; i < NUM_GRID - 1; i++)
	{
		for(WORD j = 0; j < NUM_GRID - 1; j++)
		{
			g_GridIndices[i][j][0][0] = (i + 0) + (j + 0) * NUM_GRID;
			g_GridIndices[i][j][0][1] = (i + 1) + (j + 0) * NUM_GRID;
			g_GridIndices[i][j][0][2] = (i + 1) + (j + 1) * NUM_GRID;
			g_GridIndices[i][j][1][0] = (i + 0) + (j + 0) * NUM_GRID;
			g_GridIndices[i][j][1][1] = (i + 1) + (j + 1) * NUM_GRID;
			g_GridIndices[i][j][1][2] = (i + 0) + (j + 1) * NUM_GRID;
		}
	}

    // Create the terrain texture.
    D3DTextr_CreateTexture( TERRAIN_BMP );
}


/////////////////////////////////////////////////////////////////////////////
// App_OneTimeSceneInit()
// ======================
//
// Description:
//		Called during the initial application startup.  This function
//		performs all the initialization required to get the application
//		ready for use.
//
// Parameters:
//		HWND hWnd:
//			Handle to the application window.
//
// Returns: 
//		Values returned from InitializeA3D().
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT App_OneTimeSceneInit(HWND hWnd)
{
	// Initialize A3D.
	HRESULT hResult = InitializeA3D(hWnd);

	// Build background (rolling sky), the grid for the ground and the
	// grandstand and occluding wall models.
	BuildBackground();
	BuildGrid();
	BuildModels();

	// Set the Camera's initial position.
	g_Camera.Pos[0] = 20.0f;
	g_Camera.Pos[1] = 1.0f;
	g_Camera.Pos[2] = 20.0f;
	
	g_Camera.fYaw = 220.0f;
	return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// App_OnKey()
// ===========
//
// Description:
//		Handler for keyboard events.
//
// Parameters:
//		TCHAR chKey:
//			Character code of the key pressed.
//
//		BOOL bDown:
//			TRUE if the key is being pressed down;  FALSE if the key is being
//			released.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void App_OnKey(TCHAR chKey, BOOL bDown)
{
	// Convert to uppercase.
	TCHAR chUpperKey = toupper(chKey);	
	switch(chUpperKey)
	{
		// Forward
		case 'W':
			g_bForward = bDown;
			break;

		// Back
		case 'S':
			g_bBackward = bDown;
			break;

		// Left
		case 'A':
			g_bLeft = bDown;
			break;

		// Right
		case 'D':
			g_bRight = bDown;
			break;

		case 'V':
			g_bDoVolume = !bDown;
			break;

		default:
			break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// App_FrameMove()
// ===============
//
// Description:
//		Called once per frame.  This function is the entry point for
//		animating the scene.
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
void App_FrameMove(void)
{
	// Poll the mouse for heading and pitch
	POINT ptCurPos;
	GetCursorPos(&ptCurPos);
	
	int dx = ptCurPos.x - g_WindowCenter_x;
	int dy = ptCurPos.y - g_WindowCenter_y;

	g_Camera.fYaw += dx * 0.5f;

    g_Camera.fPitch = 0.0f;
	if(g_Camera.fPitch > 80.0f)
	{
		g_Camera.fPitch = 80.0f;
	}

    if(g_Camera.fPitch < -80.0f)
	{
		g_Camera.fPitch = -80.0f;
	}

	float fPitch = g_Camera.fPitch * A3D_DEG2RAD;
	float fYaw = g_Camera.fYaw * A3D_DEG2RAD;

	g_dir = D3DVECTOR(D3DVALUE(sin(fYaw)), D3DVALUE(sin(fPitch)), D3DVALUE(cos(fYaw)));
	D3DVECTOR d3dNorm = CrossProduct(g_dir,g_up);
	float fSpeed = 0.1f;

	// Move the camera
	if(g_bForward)
	{
		g_Camera.Pos[0] += g_dir.x * fSpeed;
		g_Camera.Pos[2] += g_dir.z * fSpeed;
	}

	if(g_bBackward)
	{
		g_Camera.Pos[0] -= g_dir.x * fSpeed;
		g_Camera.Pos[2] -= g_dir.z * fSpeed;
	}

	if(g_bLeft)
	{
		g_Camera.Pos[0] += d3dNorm.x * fSpeed;
		g_Camera.Pos[2] += d3dNorm.z * fSpeed;
	}

	if(g_bRight)
	{
		g_Camera.Pos[0] -= d3dNorm.x * fSpeed;
		g_Camera.Pos[2] -= d3dNorm.z * fSpeed;
	}

	// Compute the view matrix
	D3DVECTOR d3dFrom = D3DVECTOR(g_Camera.Pos[0], g_Camera.Pos[1], g_Camera.Pos[2]);
	D3DVECTOR d3dAt = d3dFrom + g_dir;
	D3DUtil_SetViewMatrix(g_matView, d3dFrom, d3dAt, g_up);
	
	// Set it back to the center, for the next frame
	SetCursorPos(g_WindowCenter_x, g_WindowCenter_y);

	float tu = float(fYaw) / A3D_TWO_PI;
	tu -= int(tu);

    g_BackgroundMesh[0].tu = g_BackgroundMesh[1].tu = tu - 1.0f;
    g_BackgroundMesh[2].tu = g_BackgroundMesh[3].tu = tu;

	// Do the audio.
	UpdateA3D();
}


/////////////////////////////////////////////////////////////////////////////
// App_Render()
// ============
//
// Description:
//		Called once per frame.  This function is the entry point for 3D
//		rendering.  This function sets up the render states, clears the
//		viewport, and renders the scene.
//
// Parameters:
//		LPDIRECT3DDEVICE3 lpD3dDevice:
//			Pointer to a Direct3D device to render the scene on.
//
//		LPDIRECT3DVIEWPORT3 lpD3dViewport:
//			Pointer to a Direct3D viewport.  Conceptually, a viewport is a
//			2D rectangle into which a three dimensional scene is projected.
//
//		LPD3DRECT lpD3dViewRect:
//			Pointer to the bounding rectangle of the viewport area to clear.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void App_Render(LPDIRECT3DDEVICE3 lpD3dDevice, LPDIRECT3DVIEWPORT3 lpD3dViewport, LPD3DRECT lpD3dViewRect)
{
    // Clear the viewport
    lpD3dViewport->Clear2(1UL, lpD3dViewRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L);

	D3DPRIMITIVETYPE d3dRenderType = D3DPT_LINESTRIP;
	if(g_bUseTexture)
	{
		d3dRenderType = D3DPT_TRIANGLELIST;	
	}

    // Begin the scene
    if(SUCCEEDED(lpD3dDevice->BeginScene()))
    {
		if(g_bUseTexture)
		{
			// Draw the sky.
			lpD3dDevice->SetTexture(0, D3DTextr_GetTexture(CLOUD_BMP));
			lpD3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			lpD3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
			lpD3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
			lpD3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, g_BackgroundMesh, 4, 0);

			// Draw the ground.
			lpD3dDevice->SetTexture(0, D3DTextr_GetTexture(TERRAIN_BMP));
		}
		
		lpD3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &g_matView);
		lpD3dDevice->DrawIndexedPrimitive(d3dRenderType, D3DFVF_VERTEX, (void*)g_Grid, NUM_GRID_VERTICES, (WORD*)g_GridIndices, NUM_GRID_INDICES, 0);
		
		// Do the rest of the scene.
		lpD3dDevice->SetTexture(0, NULL);

		// Draw the Grandstand.
		lpD3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_VERTEX, (void*)g_GrandStand, NUM_GRANDSTAND_VERTICES, (WORD*)g_GrandStandIndices, NUM_GRANDSTAND_INDICES, 0);
				
		// Draw the Occluding Poly.
		D3DMATRIX d3dAddMat;
		D3DMath_MatrixMultiply(d3dAddMat, g_matView, g_OccPolyMat);
		lpD3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &d3dAddMat);
		
        // Set the color of the Occluding Poly.
	    D3DMATERIAL d3dMaterial;
        D3DUtil_InitMaterial(d3dMaterial, 0.2f, 0.2f, 0.75f);
        g_pmtrlObjectMtrl->SetMaterial(&d3dMaterial);
		
		lpD3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, D3DFVF_VERTEX, (void*)g_OccPoly, NUM_OCCPOLY_VERTICES, (WORD*)g_OccPolyIndices, NUM_OCCPOLY_INDICES, 0);
		
        // Set the color of the Occlude Poly
        D3DUtil_InitMaterial(d3dMaterial, 0.75, 0.75, 0.25f);
        g_pmtrlObjectMtrl->SetMaterial(&d3dMaterial);
						
		// End the scene.
		lpD3dDevice->EndScene();
    }
}


/////////////////////////////////////////////////////////////////////////////
// App_InitDeviceObjects()
// =======================
//
// Description:
//		Initializes the scene objects.
//
// Parameters:
//		LPDIRECT3DDEVICE3 lpD3dDevice:
//			Pointer to a Direct3D device to render the scene on.
//
//		LPDIRECT3DVIEWPORT3 lpD3dViewport:
//			Pointer to a Direct3D viewport.  Conceptually, a viewport is a
//			2D rectangle into which a three dimensional scene is projected.
//
// Returns: 
//		S_OK:
//			The method succeeded.
//
//		Errors returned by the following functions:
//			IDirect3DDevice3_GetDirect3D()
//			IDirect3D3_CreateMaterial()
//			IDirect3D3_CreateLight()
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
HRESULT App_InitDeviceObjects(LPDIRECT3DDEVICE3 lpD3dDevice, LPDIRECT3DVIEWPORT3 lpD3dViewport )
{
	// Get a ptr to the ID3D object to create materials and/or lights. Note:
	// the Release() call just serves to decrease the ref count.
    LPDIRECT3D3 pD3D = NULL;
	HRESULT hResult = S_OK;

	try
	{
		hResult = lpD3dDevice->GetDirect3D(&pD3D);
		if(FAILED(hResult))
		{
			throw "Failed to get the Direct3D interface pointer";
		}
    
		// Create and set up the object material
		hResult = pD3D->CreateMaterial(&g_pmtrlObjectMtrl, NULL);
		if(FAILED(hResult))
		{
			throw "Failed to create the material";
		}

		D3DMATERIAL d3dMaterial;
		D3DUtil_InitMaterial(d3dMaterial, 1.0f, 1.0f, 1.0f);
		d3dMaterial.power = 40.0f;
		g_pmtrlObjectMtrl->SetMaterial(&d3dMaterial);

		D3DMATERIALHANDLE hMaterial = NULL;
		g_pmtrlObjectMtrl->GetHandle(lpD3dDevice, &hMaterial);
		lpD3dDevice->SetLightState(D3DLIGHTSTATE_MATERIAL, hMaterial);
		lpD3dDevice->SetLightState(D3DLIGHTSTATE_AMBIENT, 0x0a0a0a0a);

		// Set up a texture.
		D3DTextr_RestoreAllTextures(lpD3dDevice);
		lpD3dDevice->SetTexture(0, D3DTextr_GetTexture(TERRAIN_BMP));
		lpD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		lpD3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		lpD3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		lpD3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
		lpD3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);

		// Set the transform matrices.
		D3DMATRIX d3dMatWorld;
		D3DUtil_SetIdentityMatrix(d3dMatWorld);

		D3DMATRIX d3dMatProj;
		D3DUtil_SetProjectionMatrix(d3dMatProj, g_PI / 3.5f, 1.0f, 1.0f, 500.0f);
		lpD3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &d3dMatWorld);
		lpD3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &d3dMatProj);

		// Set up the light.
		hResult = pD3D->CreateLight(&g_pLight, NULL);
		if(FAILED(hResult))
		{
			throw "Failed to create the light";
		}

		D3DLIGHT d3dLight;
		D3DUtil_InitLight(d3dLight, D3DLIGHT_DIRECTIONAL, 0.0, 0.0, -12.0);
		d3dLight.dvDirection = Normalize(D3DVECTOR(-0.5f, -1.0f, -0.3f));
		d3dLight.dcvColor.r = 1.0f;
		d3dLight.dcvColor.g = 1.0f;
		d3dLight.dcvColor.b = 1.0f;
		d3dLight.dvAttenuation0 = 1.0f;
		d3dLight.dvRange = D3DLIGHT_RANGE_MAX;

		g_pLight->SetLight(&d3dLight);
		lpD3dViewport->AddLight(g_pLight);

		FLOAT fFogEnd = 100.0f;

	#ifdef _USE_FOG
		lpD3dDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, 1);
		lpD3dDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, TRUE);
		lpD3dDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE);

		// Turn on fog.
		lpD3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, TRUE);
		lpD3dDevice->SetLightState(D3DLIGHTSTATE_FOGMODE, D3DFOG_LINEAR);
		lpD3dDevice->SetLightState(D3DLIGHTSTATE_FOGEND, *((DWORD *)(&fFogEnd)));
		lpD3dDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, 0x00b5b5ff);
	#endif

		// Set up the dimensions for the background image
		D3DVIEWPORT2 d3dViewport;
		d3dViewport.dwSize = sizeof(d3dViewport);
		lpD3dViewport->GetViewport2(&d3dViewport);
		
		g_BackgroundMesh[0].sy = (FLOAT)d3dViewport.dwHeight;
		g_BackgroundMesh[2].sy = (FLOAT)d3dViewport.dwHeight;
		g_BackgroundMesh[2].sx = (FLOAT)d3dViewport.dwWidth;
		g_BackgroundMesh[3].sx = (FLOAT)d3dViewport.dwWidth;
	}
	catch(char* szError)
	{
		// Display the message.
		char szMsg[256];
		wsprintf(szMsg, "App_InitDeviceObjects() - %s.\n", szError);
		MessageBox(NULL, szMsg, "A3D Sample", MB_OK | MB_ICONEXCLAMATION);
	}

    // Cleanup.
	if(pD3D)
	{
		pD3D->Release();
		pD3D = NULL;
	}

	return hResult;
}


/////////////////////////////////////////////////////////////////////////////
// App_FinalCleanup()
// ==================
//
// Description:
//		Called before the application exits.  This function gives the
//		application the opportunity to cleanup after itself.
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
void App_FinalCleanup(void)
{
    App_DeleteDeviceObjects();

    // Tell A3D to destroy all of it's objects that we allocated and release
	// all of the interfaces that we requested.
	g_pA3d->Shutdown();
}


/////////////////////////////////////////////////////////////////////////////
// App_DeleteDeviceObjects()
// =========================
//
// Description:
//		Called when the application is exiting, or the device is being
//		changed.  This function deletes any device dependent objects.
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
void App_DeleteDeviceObjects(void)
{
    D3DTextr_InvalidateAllTextures();

    SAFE_RELEASE(g_pLight);
    SAFE_RELEASE(g_pmtrlObjectMtrl);
}


/////////////////////////////////////////////////////////////////////////////
// App_ConfirmDevice()
// ===================
//
// Description:
//		Called during device initialization and device enumeration.  The code
//		simply checks for some minimum device capabilities.
//
// Parameters:
//		LPDDCAPS:
//			This parameter is unused but is required by the Direct3D
//			enumerate callback function.
//
//		LPD3DDEVICEDESC lpD3dDeviceDesc:
//			Pointer to the currently enumerated device description.
//
// Returns: 
//		S_OK:
//			The device supports the capabilities that we require to properly
//			render our scene to the display.
//
//		E_FAIL:
//			The device does not support the capabilities that we require to
//			properly render our scene to the display.
//
// Remarks:
//		See D3DEnum_EnumerateDevices() in D3DEnum.cpp for more info.
/////////////////////////////////////////////////////////////////////////////
HRESULT App_ConfirmDevice(LPDDCAPS, LPD3DDEVICEDESC lpD3dDeviceDesc)
{
    if(lpD3dDeviceDesc->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX)
	{
		return S_OK;
	}

	return E_FAIL;
}

