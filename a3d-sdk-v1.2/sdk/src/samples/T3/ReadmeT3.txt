//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	T3: Tutorial Three - Further 3D-Audio processes.
//
//	Illustrates:	Creating an A3D render object.				(T1)
//					Creating an A3D primary buffer.				(T1)
//					Creating an A3D listener object.			(T1)
//					Creating an A3D 3d-source buffer object.	(T1)
//
//					Positioning an A3D 3d-source object.		(T2)
//					Orienting an A3D 3d-source object.			(T2)
//					Positioning an A3D listener object.			(T2)
//					Orienting an A3D listener object.			(T2)
//					Setting a velocity for an A3D 3d-source.	(T2)
//					Setting a velocity for an A3D listener.		(T2)
//					Exaggerating Doppler and Rolloff effects.	(T2)
//
//					Forcing a hardware-source buffer.
//					Creating a directional source with cones.
//					Updating audio using deferred settings.
//					Minimum and maximum distances.
//					Head-relative positional settings.
//						
//
//	Created:		3rd December, 1996, by Suneil Mishra.
//					Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

This example adds to tutorials one and two. It demonstrates the functionality listed
above. There follows a brief description of what each of these functions entails...


Forcing a hardware-source buffer
--------------------------------
Buffers can be created in three ways. They can be software, hardware, or optimally processed.
A software-buffer is mixed in software and will have no 3D-characteristics.
A hardware-buffer is mixed in hardware. If there is no support for the hardware buffer, the
creation of such a buffer will fail. Your application should handle when this is the case. In
the tutorial, on failing to create a hardware buffer, we revert to an optimal buffer instead.
An optimal buffer attempts to utilize the hardware to the extent possible at the time. This is
usually the best method to create buffers with, as the application is freed of much responsibility,


Creating a directional source with cones
----------------------------------------
By default, sources are omnidirectional emitters. That is, they radiate sound equally in all
directions. In many cases, sources will actually be directional. This can be achieved using cones.
There are two cones associated with a directional sound source - the inner cone and the outer cone.
Within the inner cone, there is a constant maximal gain for the source. Outwith the outer cone, there
is a constant lower gain level. This outer level is specified by setting the ConeOutsideVolume.
Between the inner cone and the outer cone, the gain level transitions from the louder inside cone
volume to the quieter outer cone level. The cones are specified by inner and outer cone angles. These
sweep out the cone-volumes in the space around a source. If the cone angles are the same, or the
cone-outside-volume has no attenuation, then the source will again appear to be omnidirectional.
 

Updating audio using deferred settings
--------------------------------------
Setting updated values for all audio-attributes including positions or effects factors can be done 
in two ways: Immediate mode, and Deferred Mode.
In Immediate mode, the settings are immediately passed to the renderer and the audio will update for
these new values directly after a particular call is made.
In Deferred mode, the changes to values made in calls are stored but not applied immediately. Instead,
a call can be made at a later time to CommitDeferredSettings which then updates the rendering with
these new values. This can be particularly useful in frame-updated systems, where changes can be made
with a final Commit call at the end of each frame. This also improves on performance as repeated
access to the renderer is not made for each audio-call.


Minimum and maximum distances
-----------------------------
The minimum distance specifies the range for which sources within this distance do not have any
increase in gain. Thus, for a minimum distance of 1.0m, sources within this distance will appear no
louder no matter how much closer than 1.0m they are.
The maximum distance specifies the range for which sources beyond this distance do not have any
decrease in gain. Thus, for a maximum distance of 100.0m, sources beyond this distance will appear no
quieter no matter how much farther than 100.0m they are.


Head-relative positional settings
---------------------------------
The default coordinate system is for positions to be specified in world-space. Buffers can instead
use Head-Relative mode to use the listener position as their apparent origin position. This is useful
for sounds that follow the head, such as on a vehicle that the listener is an occupant in. Note that
this setting is on a source-by-source basis - it is not globally applied to all sources.


Further details on these and other audio-attributes can be found in the documentation for A3d and for DS3d.


DISCLAIMER: This tutorial code is meant simply to illustrate some of the audio-functionality available
			with A3D-Interactive. This code is provided as-is and is not intended to be used as the basis
			for an audio-rendering library in other applications. If you do so, do it at your own risk!


Suneil Mishra.
(December, 1996)


