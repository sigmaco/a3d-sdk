/////////////////////////////////////////////////////////////////////////////
// Reflect.cpp
// ===========
//
// Copyright (c) 1999-2000, Aureal Inc. - All rights reserved.
//
// This code may be used, copied or distributed in accordance with the terms
// described in the A3D 3.0 SDK License Agreement.
//
// Description:
//		Sample program to demonstrate reflections.
//
// Remarks:
//		Right mouse stops the source moving, left mouse starts it again.
//		Also uses OpenGL (opengl32.lib), the OpenGL utility library
//		(glu32.lib) and the OpenGL auxiliary library (glaux.lib).  These are
//		shipped with Win95 OSR2 and later and with Microsoft Visual C++.
/////////////////////////////////////////////////////////////////////////////

#define INITGUID

#include <objbase.h>
#include <stdlib.h>
#include <cguid.h>

// OpenGL Include Files.
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

// A3D Include Files.
#include "ia3dapi.h"

/////////////////////////////////////////////////////////////////////////////
// Global A3D Objects.
//
IA3d5*			g_pA3d			= NULL;
IA3dGeom2*		g_pA3dGeom		= NULL;
IA3dSource2*	g_pA3dSource	= NULL;
IA3dMaterial*	g_pA3dMaterial1 = NULL;
IA3dMaterial*	g_pA3dMaterial2 = NULL;

float			fSrcXYZ[3]		= { 8.0f, 0.0f, -8.0f };
float			fPolyXYZ[3]		= { 2.5f, 0.0f, 3.0f };

HWND			g_hWnd			= NULL;

float verts[8][3] =
{
	{-10.0f, -2.0f, -10.0f},
	{ 10.0f, -2.0f, -10.0f},
	{ 10.0f,  2.0f, -10.0f},
	{-10.0f,  2.0f, -10.0f},
	{ 10.0f, -2.0f,  10.0f},
	{-10.0f, -2.0f,  10.0f},
	{-10.0f,  2.0f,  10.0f},
	{ 10.0f,  2.0f,  10.0f}
};


/////////////////////////////////////////////////////////////////////////////
// Function Prototypes.
//
void			main(int argc, char** argv);
HRESULT			AudioInit(void);
void			AudioExit(void);
void			GraphicsInit(int argc, char **argv);
void CALLBACK	UpdateScene(void);
void CALLBACK	GraphicsReshape(int w, int h);
void CALLBACK	UpdateListener(void);
void CALLBACK	StartListener(AUX_EVENTREC *event);
void CALLBACK	StopListener(AUX_EVENTREC *event);


/////////////////////////////////////////////////////////////////////////////
// Program Functions.
//
void main(int argc, char** argv)
{
	// Initialize the OpenGL engine.
	GraphicsInit(argc, argv);

	// Initialize the A3D audio engine.
	HRESULT hResult = AudioInit();
	if(FAILED(hResult))
	{
		MessageBox(g_hWnd, "Failed to initialze the A3D engine.", "A3D Sample Program", MB_OK | MB_ICONEXCLAMATION);
		AudioExit();

		return;
	}

	// Start the graphics main update loop.
	auxMainLoop(UpdateScene);

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

		// When initializing A3D, request occlusions and reflections.
		DWORD dwFeatures = A3D_OCCLUSIONS | A3D_1ST_REFLECTIONS;

		// -- GEOMETRIC REVERB
		// To add geometric reverb, add the A3D_GEOMETRIC_REVERB flag to the
		// features flags.  You can query A3D to see if it supports features.
		// COM allows you to query for an interface, if the interface is
		// available it will return a pointer to the requested interface.  As
		// an example we know that geometric reverb was introduced in the
		// IA3dGeom2 interface.  If the QueryInterface() call succeeds, then
		// we will append the A3D_GEOMETRIC_REVERB flag to our requested
		// features.
		//
		// NOTE:	This is only an example of using COM to test for
		//			available functionality.  The InitEx() call would still
		//			succeed even if we didn't perform this check.
		hResult = g_pA3d->QueryInterface(IID_IA3dGeom2, (void **)&g_pA3dGeom);
		if(SUCCEEDED(hResult))
		{
			dwFeatures |= A3D_GEOMETRIC_REVERB;
		}

		// Initialize the interface.
		hResult = g_pA3d->InitEx(NULL, dwFeatures, A3DRENDERPREFS_DEFAULT, g_hWnd, A3D_CL_NORMAL);
		if(FAILED(hResult))
		{
			throw "Failed to Initialize IID_IA3d5 interface";
		}

		// Check for hardware reflections.  The InitEx() call simply requests
		// the features, there is no guarantee that they will be given to you.
		// Reflections for example, can only be rendered by one application
		// at a time.
		if(g_pA3d->IsFeatureAvailable(A3D_1ST_REFLECTIONS) == FALSE)
		{
			throw "No hardware supported for reflections";
		}

		// Make a new source. at this stage the source is empty - no wave data
		hResult = g_pA3d->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &g_pA3dSource);
		if(FAILED(hResult))
		{
			throw "Failed to create the A3D source";
		}

		// This function call here is the equivalent of the old function LoadWaveFile().
		// These are convenience functions. There are other ways of getting sounds
		// loaded.
		hResult = g_pA3dSource->LoadFile("linn1.wav", A3DSOURCE_FORMAT_WAVE);
		if(FAILED(hResult))
		{
			throw "Failed to load linn1.wav";
		}

		// Enable our geometry effects.
		g_pA3dGeom->Enable(A3D_OCCLUSIONS);
		g_pA3dGeom->Enable(A3D_1ST_REFLECTIONS);
		g_pA3dGeom->Enable(A3D_GEOMETRIC_REVERB);

		// Create our materials.
		g_pA3dGeom->NewMaterial(&g_pA3dMaterial1);
		g_pA3dGeom->NewMaterial(&g_pA3dMaterial2);

		// Setup the properties for our new materials.
		g_pA3dMaterial1->SetTransmittance(1.0f, 1.0f);
		g_pA3dMaterial1->SetReflectance(1.0f, 1.0f);

		g_pA3dMaterial2->SetTransmittance(0.3f, 1.0f);
		g_pA3dMaterial2->SetReflectance(1.0f, 0.5f);
		
		// Adjust the distance and scaling properties.
		g_pA3dGeom->SetReflectionDelayScale(10.0f);
		g_pA3dSource->SetMinMaxDistance(3.0f, 100.0f, A3D_AUDIBLE);

		// Start playing the source.
		g_pA3dSource->Play(A3D_LOOPED);
	}
	catch(char* szError)
	{
		// Display the message.
		char szMsg[256];
		wsprintf(szMsg, "AudioInit() - %s.\n", szError);
		MessageBox(g_hWnd, szMsg, "A3D Sample", MB_OK | MB_ICONEXCLAMATION);
	}

	return hResult;
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


/////////////////////////////////////////////////////////////////////////////
// UpdateScene()
// =============
//
// Description:
//		Updates the audio and graphics frames.
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
void CALLBACK UpdateScene(void)
{
	// Do the audio first since it's much cooler than graphics!

	// Clear the audio frame and put an identity matrix on the stack.
	g_pA3d->Clear();
	g_pA3dGeom->LoadIdentity();

	// Translate and rotate to the listener position. note that this is
	// slightly different from setting up the viewpoint in a graphics system.
	// You don't do the opposite xforms to translate the origin of the world
	// to the listener, you do them the same way as for sources.
	//
	// It's also a good idea to surround each block with a Push/PopMatrix()
	// pair so you can get back to the old matrix once you're finished.  An
	// alternative here would be to miss out the Push/PopMatrix calls and
	// replace the PopMatrix() after BindListener() with LoadIdentity(), but
	// this is a really simple case.  LoadIdentity() is very marginally quicker
	// than a Push/PopMatrix() pair, but you'd never notice unless you were
	// doing thousands of them per frame.
	g_pA3dGeom->PushMatrix();

		// Translate to position of listener (happens to be 0,0,0 here)
		g_pA3dGeom->Translate3f(0.0f, 0.0f, 0.0f);

		// Make these transformations apply to the listener.
		g_pA3dGeom->BindListener();

	g_pA3dGeom->PopMatrix();

	// Translate and rotates for source.
	g_pA3dGeom->PushMatrix();

		// Translate to where we want the source.
		g_pA3dGeom->Translate3fv(fSrcXYZ);

		// Make that xform apply to the source.
		g_pA3dGeom->BindSource(g_pA3dSource);

	g_pA3dGeom->PopMatrix();

	g_pA3dGeom->BindMaterial(g_pA3dMaterial1);
	g_pA3dGeom->Begin(A3D_QUADS);
		g_pA3dGeom->Tag(1);
		g_pA3dGeom->Vertex3fv(verts[0]);
		g_pA3dGeom->Vertex3fv(verts[1]);
		g_pA3dGeom->Vertex3fv(verts[2]);
		g_pA3dGeom->Vertex3fv(verts[3]);

		g_pA3dGeom->Tag(2);
		g_pA3dGeom->Vertex3fv(verts[0]);
		g_pA3dGeom->Vertex3fv(verts[3]);
		g_pA3dGeom->Vertex3fv(verts[6]);
		g_pA3dGeom->Vertex3fv(verts[5]);

		g_pA3dGeom->Tag(3);
		g_pA3dGeom->Vertex3fv(verts[6]);
		g_pA3dGeom->Vertex3fv(verts[7]);
		g_pA3dGeom->Vertex3fv(verts[4]);
		g_pA3dGeom->Vertex3fv(verts[5]);

		g_pA3dGeom->Tag(4);
		g_pA3dGeom->Vertex3fv(verts[4]);
		g_pA3dGeom->Vertex3fv(verts[1]);
		g_pA3dGeom->Vertex3fv(verts[2]);
		g_pA3dGeom->Vertex3fv(verts[7]);

		g_pA3dGeom->Tag(5);
		g_pA3dGeom->Vertex3fv(verts[1]);
		g_pA3dGeom->Vertex3fv(verts[0]);
		g_pA3dGeom->Vertex3fv(verts[5]);
		g_pA3dGeom->Vertex3fv(verts[4]);

		g_pA3dGeom->Tag(6);
		g_pA3dGeom->Vertex3fv(verts[3]);
		g_pA3dGeom->Vertex3fv(verts[2]);
		g_pA3dGeom->Vertex3fv(verts[7]);
		g_pA3dGeom->Vertex3fv(verts[6]);
	g_pA3dGeom->End();

	// Send this frame to be rendered.
	g_pA3d->Flush();

	/////////////////////////////////////////////////////////////////////////
	// And that's all there is to it, the rest of this function is just
	// graphics - anyone interested?
	//

	// Clear the frame buffer and Z-Buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		// Set up view position (this isn't where the listener is in this
		// example).  Remember to do -ve xforms and in reverse order for
		// graphics viewpoint.  This viewpoint is back and up from and
		// looking down at the scene.
		glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -15.0f, -30.0f);

		// Draw a poly where the listener is and orientate it the same way.
		glPushMatrix();

			// (instead of doing xforms here we could have done a GetMatrix()
			// in the audio part and a glMultMatrix() here. getting graphics
			// matrices with glGetFloatv(GL_MODELVIEW_MATRIX, ..) and using
			// them for audio is a bad idea - graphics gets usually involve
			// flushing the hardware graphics pipeline before the data can be
			// sent back up to the app, which is slow).
			glTranslatef(0.0f, 0.0f, 0.0f);

			// Draw a red arrow for the listener.
			glColor3f(1.0f, 0.3f, 0.3f);
			glBegin(GL_TRIANGLES);
				glVertex3f(-0.5f, 0.2f,  1.0f);
				glVertex3f( 0.5f, 0.2f,  1.0f);
				glVertex3f( 0.0f, 0.2f, -1.0f);

				glVertex3f( 0.0f, -0.2f, -1.0f);
				glVertex3f( 0.5f, -0.2f,  1.0f);
				glVertex3f(-0.5f, -0.2f,  1.0f);
			glEnd();
			
			glColor3f(0.7f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
				glVertex3f( 0.0f,  0.2f, -1.0f);
				glVertex3f( 0.0f, -0.2f, -1.0f);
				glVertex3f(-0.5f, -0.2f,  1.0f);
				glVertex3f(-0.5f,  0.2f,  1.0f);

				glVertex3f( 0.5f,  0.2f,  1.0f);
				glVertex3f( 0.5f, -0.2f,  1.0f);
				glVertex3f( 0.0f, -0.2f, -1.0f);
				glVertex3f( 0.0f,  0.2f, -1.0f);

				glVertex3f(-0.5f,  0.2f,  1.0f);
				glVertex3f(-0.5f, -0.2f,  1.0f);
				glVertex3f( 0.5f, -0.2f,  1.0f);
				glVertex3f( 0.5f,  0.2f,  1.0f);
			glEnd();

		glPopMatrix();

		/////////////////////////////////////////////////////////////////////
		// Draw a poly where the source is and a line to it from the listener
		//
		glPushMatrix();

			// Same note about xforms - could have done glMultMatrix().
			glTranslatef(fSrcXYZ[0], fSrcXYZ[1], fSrcXYZ[2]);

			// Draw a blue cube for the source.
			glBegin(GL_QUADS);
				
				glColor3f(0.5f, 0.5f, 1.0f);
				glVertex3f(-0.3f, 0.3f,  0.3f);
				glVertex3f( 0.3f, 0.3f,  0.3f);
				glVertex3f( 0.3f, 0.3f, -0.3f);
				glVertex3f(-0.3f, 0.3f, -0.3f);

				glColor3f(0.3f, 0.3f, 1.0f);
				glVertex3f(-0.3f, -0.3f,  0.3f);
				glVertex3f( 0.3f, -0.3f,  0.3f);
				glVertex3f( 0.3f,  0.3f,  0.3f);
				glVertex3f(-0.3f,  0.3f,  0.3f);
				
				glVertex3f( 0.3f, -0.3f,  0.3f);
				glVertex3f( 0.3f, -0.3f, -0.3f);
				glVertex3f( 0.3f,  0.3f, -0.3f);
				glVertex3f( 0.3f,  0.3f,  0.3f);

				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(-0.3f,  0.3f, -0.3f);
				glVertex3f( 0.3f,  0.3f, -0.3f);
				glVertex3f( 0.3f, -0.3f, -0.3f);
				glVertex3f(-0.3f, -0.3f, -0.3f);

				glVertex3f(-0.3f,  0.3f,  0.3f);
				glVertex3f(-0.3f,  0.3f, -0.3f);
				glVertex3f(-0.3f, -0.3f, -0.3f);
				glVertex3f(-0.3f, -0.3f,  0.3f);

			glEnd();

		glPopMatrix();

		// Turn on alpha blending.
		glEnable(GL_BLEND);

		// Draw the acoustic polygons now.
		glBegin(GL_QUADS);
			glColor4f(0.4f, 1.0f, 1.0f, 0.3f);
			glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
			glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
			glVertex3f(verts[5][0], verts[5][1], verts[5][2]);
			glVertex3f(verts[4][0], verts[4][1], verts[4][2]);

			glColor4f(0.0f, 0.6f, 0.6f, 0.3f);
			glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
			glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
			glVertex3f(verts[7][0], verts[7][1], verts[7][2]);
			glVertex3f(verts[6][0], verts[6][1], verts[6][2]);

			glColor4f(0.0f, 1.0f, 1.0f, 0.3f);
			glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
			glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
			glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
			glVertex3f(verts[3][0], verts[3][1], verts[3][2]);

			glVertex3f(verts[6][0], verts[6][1], verts[6][2]);
			glVertex3f(verts[7][0], verts[7][1], verts[7][2]);
			glVertex3f(verts[4][0], verts[4][1], verts[4][2]);
			glVertex3f(verts[5][0], verts[5][1], verts[5][2]);

			glColor4f(0.0f, 0.8f, 0.8f, 0.3f);
			glVertex3f(verts[0][0], verts[0][1], verts[0][2]);
			glVertex3f(verts[3][0], verts[3][1], verts[3][2]);
			glVertex3f(verts[6][0], verts[6][1], verts[6][2]);
			glVertex3f(verts[5][0], verts[5][1], verts[5][2]);

			glVertex3f(verts[4][0], verts[4][1], verts[4][2]);
			glVertex3f(verts[1][0], verts[1][1], verts[1][2]);
			glVertex3f(verts[2][0], verts[2][1], verts[2][2]);
			glVertex3f(verts[7][0], verts[7][1], verts[7][2]);

		glEnd();

		// Turn off alpha blending again since it's expensive.
		glDisable(GL_BLEND);

		// Draw line after the polys so it gets zbuffered out - alpha on the
		// line didn't look too good.
		glColor3f(0.0f, 0.7f, 0.0f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.5f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3fv(fSrcXYZ);
		glEnd();
	glPopMatrix();

	// Swap the graphics frame buffers to display what's just been drawn.
	auxSwapBuffers();
}


/////////////////////////////////////////////////////////////////////////////
// UpdateListener()
// ================
//
// Description:
//		Calculates and updates the listener position in the graphics
//		environment.
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
void CALLBACK UpdateListener(void)
{
	static int nMotion = 0;
	static float fPrevFrameTime = 0.0166f;
	static float fPrevTime = 0.0f;
	
	// Record time since last frame.
	float fTime = (float)timeGetTime();
	float fFrameTime = (fTime - fPrevTime) * 0.001f;
	
	// Check for fist frame and wrap-around.
	if(fFrameTime < 0.0f || fFrameTime > 5.0f)
	{
		fFrameTime = fPrevFrameTime;
	}
	
    fPrevFrameTime = fFrameTime;
    fPrevTime = fTime;
	
	// Move the source in a square around the origin (it assumes we want to
	// move at 0.5m per second and that we're updating at 30Hz). also change
	// the position of a polygon.
	switch (nMotion)
	{
	case 0:
		fSrcXYZ[0] -= 1.5f * fFrameTime;
		fPolyXYZ[0] -= 0.5f * fFrameTime;
		
		if(fSrcXYZ[0] < -8.0f)
		{
			nMotion++;
		}

		break;
		
	
	case 1:
		fSrcXYZ[2] += 1.5f * fFrameTime;
		fPolyXYZ[0] += 0.5f * fFrameTime;
		
		if(fSrcXYZ[2] > 8.0f)
		{
			nMotion++;
		}

		break;
		

	case 2:
		fSrcXYZ[0] += 1.5f * fFrameTime;
		fPolyXYZ[0] -= 0.5f * fFrameTime;
		
		if(fSrcXYZ[0] > 8.0f)
		{
			nMotion++;
		}

		break;
		

	case 3:
		fSrcXYZ[2] -= 1.5f * fFrameTime;
		fPolyXYZ[0] += 0.5f * fFrameTime;
		
		if(fSrcXYZ[2] < -8.0f)
		{
			nMotion = 0;
		}

		break;
	}

	// Render the new audio and graphics.
	UpdateScene();	
}


/////////////////////////////////////////////////////////////////////////////
// StartListener()
// ===============
//
// Description:
//		Allows the mouse buttons to start and stop the rotation of the
//		listener.
//
// Parameters:
//		AUX_EVENTREC *event:
//			Event structure sent by OpenGL.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void CALLBACK StartListener(AUX_EVENTREC *event)
{
	// Can use the mouse buttons to start and stop the listener rotation.
	auxIdleFunc(UpdateListener);
}


/////////////////////////////////////////////////////////////////////////////
// StopListener()
// ==============
//
// Description:
//		Stops the listener rotation.
//
// Parameters:
//		AUX_EVENTREC *event:
//			Event structure sent by OpenGL.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void CALLBACK StopListener(AUX_EVENTREC *event)
{
	// Stop the listener rotation - take out the idle function which updated
	// it.
	auxIdleFunc(NULL);
}


/////////////////////////////////////////////////////////////////////////////
// GraphicsInit()
// ==============
//
// Description:
//		Initializes the OpenGL graphics engine.
//
// Parameters:
//		int argc:
//			An integer specifying how many arguments are passed to the
//			program from the command line.  Because the program name is
//			considered an argument, argc is at least 1.
//
//		char **argv:
//			An array of null-terminated strings. The first string (argv[0])
//			is the program name, and each following string is an argument
//			passed to the program from the command line. The last pointer
//			(argv[argc]) is NULL.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void GraphicsInit(int argc, char **argv)
{
	// Create a window in RGB color mode with double buffers and Z-Buffer.
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);
	auxInitPosition(50, 400, 500, 280);
	auxInitWindow(argv[0]);

	g_hWnd = GetForegroundWindow();

	// Clear to pale blue when glClear() is called.
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	// Gouraud shading in case we ever have vertex colors or normals.
	glShadeModel(GL_SMOOTH);

	// Switch on the Z-Buffer.
	glEnable(GL_DEPTH_TEST);

	// Set up alpha mode for when it's enabled.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up functions to be called when window moves, mouse buttons are
	// pressed, or just when nothing happens at all, but don't actually start
	// to do anything yet.
	auxReshapeFunc(GraphicsReshape);
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, StartListener);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, StopListener);
	auxIdleFunc(UpdateListener);
}


/////////////////////////////////////////////////////////////////////////////
// GraphicsReshape()
// =================
//
// Description:
//		Called whenever the size or dimensions of the window are changed.
//
// Parameters:
//		int w:
//			Width of the viewport in pixels.
//
//		int h:
//			Height of the viewport in pixels.
//
// Returns: 
//		Nothing.
//
// Remarks:
//		None.
/////////////////////////////////////////////////////////////////////////////
void CALLBACK GraphicsReshape(int w, int h)
{
	// Handle the minimize case
	if((w == 0)||(h == 0))
	{
		return;
	}

	// This is called when the window is initialize, or when it moves or gets
	// resized.  It sets up the graphics viewport within the window, sets up
	// the projection matrix for a perspective view with 60 degress field of
	// view vertically, an aspect ration to match the shape of the window and
	// near and far clipping planes at 1 and 1000m.
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)(w / h), 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
