/*
 * spinner.cpp
 *
 *
 * Copyright (c) 1999 Aureal Semiconductor, Inc. - All rights reserved.
 *
 * This code may be used, copied or distributed in accordance with the terms
 * described in the A3D2.0 SDK License Agreement.
 *
 *
 * Sample program to show matrix transformations applied to a listener and
 * source. Right mouse button stops the listener rotating, left button starts
 * it again, and escape quits.
 *
 * Also uses OpenGL (opengl32.lib), the OpenGL utility library (glu32.lib) and
 * the OpenGL auxiliary library (glaux.lib) - these are shipped with Win95 OSR2
 * and VC++, or you can get them from a large window system vendor based near
 * Seattle.
 *
*/

#define INITGUID

#include <objbase.h>
#include <stdlib.h>
#include <cguid.h>

/* need to include windows.h before the gl stuff */
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

/* the A3D include file */
#include "ia3dapi.h"

/* -------------------------------------------------------------------------- */

/* function prototypes */
void main(int argc, char **argv);
int  audio_init(void);
void audio_exit(void);
void CALLBACK update_scene(void);
void gfx_init(int argc, char **argv);
void CALLBACK gfx_reshape(int w, int h);
void CALLBACK update_listener(void);
void CALLBACK start_listener(AUX_EVENTREC *event);
void CALLBACK stop_listener(AUX_EVENTREC *event);

/* -------------------------------------------------------------------------- */

/* some global variables */
IA3d4        *a3droot = NULL;
IA3dGeom     *a3dgeom = NULL;
IA3dListener *a3dlis  = NULL;
IA3dSource   *a3dsrc  = NULL;

float        heading = 0.0f;

HWND hWin;

/* -------------------------------------------------------------------------- */

void
main(int argc, char **argv)
{
int hr;

	/* initialize the graphics */
	gfx_init(argc, argv);

	/* initialize the audio */
	hr = audio_init();
	if (SUCCEEDED(hr))
	{
		/* start the main update loop */
		auxMainLoop(update_scene);
	}

	/* clean up the audio stuff */
	audio_exit();

}

int
audio_init(void)
{
int hr;

	/* initialize COM */
	CoInitialize(NULL);

	/* get the base a3d interface */
	hr = CoCreateInstance(CLSID_A3dApi, NULL, CLSCTX_INPROC_SERVER,
	                      IID_IA3d4, (void **)&a3droot);
    if (FAILED(hr))
		return(hr);

	/* initialize a3d */
	a3droot->Init(NULL, NULL, A3DRENDERPREFS_DEFAULT);

	a3droot->SetCooperativeLevel(hWin, A3D_CL_NORMAL);
	
	/* query for the geometry interface (for matrix ops etc.) */
	hr = a3droot->QueryInterface(IID_IA3dGeom, (void **)&a3dgeom);
	if (FAILED(hr))
		return(hr);

	/*
	 * query for the listener (we won't actually use it in this
	 * example though)
	*/
	hr = a3droot->QueryInterface(IID_IA3dListener, (void **)&a3dlis);
	if (FAILED(hr))
		return(hr);

	/* make a new source. at this stage the source is empty - no wave data */
	hr = a3droot->NewSource(A3DSOURCE_INITIAL_RENDERMODE_A3D, &a3dsrc);
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
	a3dsrc->Play(A3D_LOOPED);

	return(S_OK);

}

void CALLBACK
update_scene(void)
{

	/*
	 * do the audio first since it's much cooler than graphics
	*/

	/* clear the audio frame */
	a3droot->Clear();

	/* put an identity matrix on the stack */
	a3dgeom->LoadIdentity();

	/*
	 * translate and rotate to the listener position. note that this is slightly
	 * different from setting up the viewpoint in a graphics system - you don't
	 * do the opposite xforms to translate the origin of the world to the
	 * listener, you do them the same way as for sources.
	 *
	 * it's also a good idea to surround each block with a Push/PopMatrix()
	 * pair so you can get back to the old matrix once you're finished. an
	 * alternative here would be to miss out the Push/PopMatrix calls and
	 * replace the PopMatrix() after BindListener() with LoadIdentity(), but
	 * this is a really simple case. (LoadIdentity() is very marginally quicker
	 * than a Push/PopMatrix() pair, but you'd never notice unless you were
	 * doing thousands of them per frame).
	*/
	a3dgeom->PushMatrix();

		/* translate to position of listener (happens to be 0,0,0 here) */
		a3dgeom->Translate3f(0.0f, 0.0f, 0.0f);

		/* rotate by 'heading' degrees about the Y (up) axis */
		a3dgeom->Rotate3f(heading, 0.0f, 1.0f, 0.0f);

		/* make these transformations apply to the listener */
		a3dgeom->BindListener();

	a3dgeom->PopMatrix();

	/* translate and rotates for source */
	a3dgeom->PushMatrix();

		/* translate to where we want the source */
		a3dgeom->Translate3f(3.0f, 0.0f, -3.0f);

		/* make that xform apply to the source */
		a3dgeom->BindSource(a3dsrc);

	a3dgeom->PopMatrix();

	/* send this frame to be rendered */
	a3droot->Flush();

	/*
	 * and that's all there is to it, the rest of this function is
	 * graphics - anyone interested?
 	*/

	/* clear frame buffer and zbuffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		/*
		 * set up view position (this isn't where the listener is
		 * in this example). remember to do -ve xforms and in reverse
		 * order for graphics viewpoint. this viewpoint is back and up
		 * from and looking down at the scene.
	 	*/
		glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -10.0f, -10.0f);

		/* draw a poly where the listener is and orientate it the same way */
		glPushMatrix();

			/*
			 * (instead of doing xforms here we could have done a GetMatrix()
			 * in the audio part and a glMultMatrix() here. getting graphics
			 * matrices with glGetFloatv(GL_MODELVIEW_MATRIX, ..) and using
			 * them for audio is a bad idea - graphics gets usually involve
			 * flushing the hardware graphics pipeline before the data can be
			 * sent back up to the app, which is slow).
		  	*/
			glTranslatef(0.0f, 0.0f, 0.0f);
			glRotatef(heading, 0.0f, 1.0f, 0.0f);

			/* draw a red arrow for the listener */
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

		/* draw a poly where the source is */
		glPushMatrix();

			/* (same note about xforms - could have done glMultMatrix() ) */
			glTranslatef(3.0f, 0.0f, -3.0f);
			glColor3f(0.0f, 0.0f, 1.0f);

			/* draw a blue square for the source */
			glBegin(GL_QUADS);
				glVertex3f(-0.5f, 0.0f,  0.5f);
				glVertex3f( 0.5f, 0.0f,  0.5f);
				glVertex3f( 0.5f, 0.0f, -0.5f);
				glVertex3f(-0.5f, 0.0f, -0.5f);
			glEnd();

		glPopMatrix();

	glPopMatrix();

	/* swap the graphics frame buffers to display what's just been drawn */
	auxSwapBuffers();


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
	
	if (a3dgeom)
		a3dgeom->Release();
	
	if (a3droot)
		a3droot->Release();

    /* free up COM (we could have done that at the end of audio_init() ) */
    CoUninitialize();

}

void CALLBACK
update_listener(void)
{
static float fPrevFrameTime = 0.0166f;
static float fPrevTime = 0.0f;
float fFrameTime;
float fTime;

	/* record time since last frame */
	fTime = (float)timeGetTime();

	fFrameTime = (fTime - fPrevTime) * 0.001f;

	/* check for fist frame and wrap-around */
	if (fFrameTime < 0.0f || fFrameTime > 5.0f)
		fFrameTime = fPrevFrameTime;

    fPrevFrameTime = fFrameTime;
    fPrevTime = fTime;


	/*
	 * this is the heading angle we use for the listener (it assumes we want
	 * to rotate at 30 degrees per second and that we're updating at 60Hz)
	*/
	heading += (30.0f * fFrameTime);

	/* render the new audio and graphics */
	update_scene();

}

void CALLBACK
start_listener(AUX_EVENTREC *event)
{

	/* can use the mouse buttons to start and stop the listener rotation */
	auxIdleFunc(update_listener);
}

void CALLBACK
stop_listener(AUX_EVENTREC *event)
{

	/*
	 * stop the listener rotation - take out the idle function which
	 * updated it
	*/
	auxIdleFunc(NULL);

}

/*
 * pure graphics stuff below here
*/

void
gfx_init(int argc, char **argv)
{

	/* crate a window in RGB color mode with double buffers and zbuffer */
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH);
	auxInitPosition(50, 400, 500, 280);
	auxInitWindow(argv[0]);

	hWin = GetForegroundWindow();

	/* clear to pale blue when glClear() is called */
	glClearColor(0.3f, 0.3f, 0.8f, 0.0f);

	/* gouraud shading in case we ever have vertex colors or normals */
	glShadeModel(GL_SMOOTH);

	/* switch on the zbuffer */
	glEnable(GL_DEPTH_TEST);

	/*
	 * set up functions to be called when window moves, mouse buttons are
	 * pressed, or just when nothing happens at all, but don't actually start
	 * to do anything yet.
 	*/
	auxReshapeFunc(gfx_reshape);
	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, start_listener);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, stop_listener);
	auxIdleFunc(update_listener);

}

void CALLBACK 
gfx_reshape(int w, int h)
{

	/*
	 * this is called when the window is initialize, or when it moves or
	 * gets resized. it sets up the graphics viewport within the window,
	 * sets up the projection matrix for a perspective view with 60 degress
	 * field of view vertically, an aspect ration to match the shape of the
	 * window and near and far clipping planes at 1 and 1000m.
 	*/
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)(w/h), 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

}

