//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	T1: Tutorial One - Creating a 3D-Audio application.
//
//	Illustrates:	Creating an A3D render object.
//					Creating an A3D primary buffer.
//					Creating an A3D listener object.
//					Creating an A3D 3d-source buffer object.
//
//	Also contains:	2d-source buffer object creation.
//					Wavefile loading code.
//					Application-window creation code.
//
//	Created:		25th November, 1996, by Suneil Mishra.
//					Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


This example demonstrates the functionality listed above.
There follows a brief description of what each of these functions entails...


Creating an A3D render object
-----------------------------
In order to output any audio streams or files, a top-level render object must be created.
This can take the form of an A3D object using A3dCreate(). (It could also be a DirectSound3D
object using DirectSoundCreate().) This object must be the first created and last destroyed.
It is required to create buffers and indirectly the listener object.


Creating an A3D primary buffer object
-------------------------------------
The primary buffer is the actual final output buffer for the A3d/DS3d system. All other buffer
sources created are secondary buffers that get mixed through this primary buffer. This buffer
has special control flags set within it in order to be rendered through in 3d-mode. This is the
2d-equivalent to the listener object. This buffer is created by request to the renderer object.


Creating an A3D listener object
-------------------------------
The listener is the 3d-equivalent to the primary buffer. It is the global controller for 3d-audio
effects. The characteristics of the listener represent the virtual ears in the world through which
sources are heard. This is created by querying the primary buffer.


Creating an A3D 3d-source buffer object
---------------------------------------
Each audio file or stream to be rendered in 3d has an associated 3d-buffer and 2d-buffer.
The 2d-buffer contains the audio-data to be rendered and controls functionality such as starting
and stopping the source from playing. The 3d-audio functionality is handled by the 3d-buffer object.
This is used to characterize the source in terms of its position and orientation in the world. Other
attributes of the source including directionality are handled by the 3d-buffer also. The buffer can
be created and processed in hardware, software, or optimally based on currently available support.
(See Tutorial Three for more on forcing buffer-mixing.) The source 2d-buffer is created by request to
the renderer object. The 3d-buffer is created by querying the 2d-buffer. Note that the 2d-buffer
requires special flags to be set to enable 3d-audio processing. 



Further details on these and other audio-attributes can be found in the documentation for A3d and for DS3d.


DISCLAIMER: This tutorial code is meant simply to illustrate some of the audio-functionality available
			with A3D-Interactive. This code is provided as-is and is not intended to be used as the basis
			for an audio-rendering library in other applications. If you do so, do it at your own risk!


Suneil Mishra.
(December, 1996)


