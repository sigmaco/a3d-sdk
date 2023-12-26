//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	T2: Tutorial Two - Further 3D-Audio processes.
//
//	Illustrates:	Creating an A3D render object.				(T1)
//					Creating an A3D primary buffer.				(T1)
//					Creating an A3D listener object.			(T1)
//					Creating an A3D 3d-source buffer object.	(T1)
//
//					Positioning an A3D 3d-source object.
//					Orienting an A3D 3d-source object.
//					Positioning an A3D listener object.
//					Orienting an A3D listener object.
//					Setting a velocity for an A3D 3d-source.
//					Setting a velocity for an A3D listener.
//					Exaggerating Doppler and Rolloff effects.
//
//
//	Created:		30th November, 1996, by Suneil Mishra.
//					Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


This example adds to tutorial one. It demonstrates the functionality listed
above. There follows a brief description of what each of these functions entails...


Positioning an A3D 3d-source object
-----------------------------------
3d-Sources can be positioned in the world-space using (x, y, z) coordinates by default.
(For head-relative coordinates, see Tutorial Three.)
A3D uses a left-handed coordinate system. This means that the x-axis increases from left
to right, the y-axis increases from down-to-up, and the z-axis increases from back to front.
(IE: X increases from left-ear to right-ear, Y increases from chin to forehead, and 
     Z increases from back-of-the-head to front-of-the-head.)


Orienting an A3D 3d-source object
-----------------------------------
Orientation of a source is by means of a normalized direction-vector.
The coordinate system is as for positions.


Positioning an A3D listener object
----------------------------------
The listener can be positioned in the world-space using (x, y, z) coordinates by default.
A3D uses a left-handed coordinate system. This means that the x-axis increases from left
to right, the y-axis increases from down-to-up, and the z-axis increases from back to front.
(IE: X increases from left-ear to right-ear, Y increases from chin to forehead, and 
     Z increases from back-of-the-head to front-of-the-head.)


Orienting an A3D listener object
----------------------------------
Orientation of the listener is by means of two normalized direction-vectors.
The first is the FrontVector which is the direction a listener is facing (which way the nose points).
The second is the TopVector which is the direction in which the top of the head points.
The coordinate system is as for positions.


Setting a velocity for an A3D 3d-source
---------------------------------------
The velocity of a source is independent of its position. This means that setting a velocity does
not mean a corresponding change in position (unless calculated by the application). This allows for
special effects such as "jumping into hyperspace" and instantly reappearing in another part of the
world. The velocity of a source is used to calculate its Doppler effects with respect to the listener.
The coordinate system is as for positions. Velocities are specified as x, y, z-component-scalars.


Setting a velocity for an A3D listener
--------------------------------------
The velocity of the listener is independent of its position. This means that setting a velocity does
not mean a corresponding change in position (unless calculated by the application). This allows for
special effects such as "jumping into hyperspace" and instantly reappearing in another part of the
world. The velocity of the listener is used to calculate Doppler effects of sources in the world.
The coordinate system is as for positions. Velocities are specified as x, y, z-component-scalars.


Exaggerating Doppler and rolloff effects
----------------------------------------
Extra controls on effects such as attenuation due to distance, and velocity are available. "Real-world"
values are the default settings (1.0 values). These effects can be exaggerated or diminished by scaling
the factors from their unity defaults. Note that these values affect all sources in the world uniformly.



Further details on these and other audio-attributes can be found in the documentation for A3d and for DS3d.


DISCLAIMER: This tutorial code is meant simply to illustrate some of the audio-functionality available
			with A3D-Interactive. This code is provided as-is and is not intended to be used as the basis
			for an audio-rendering library in other applications. If you do so, do it at your own risk!


Suneil Mishra.
(December, 1996)


