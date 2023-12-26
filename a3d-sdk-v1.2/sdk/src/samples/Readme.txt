//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	Tutorials Readme.Txt
//
//	Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


Within this tutorial directory you will find three example applications that demonstrate
some of the basic Aureal3D sound functionality. Each tutorial can exist on its own, and
each builds on the previous one(s).


IMPORTANT
---------
The debug and release builds of each tutorial create executables in the Bin directory. Each
tutorial looks for wavefiles in the working directory which is this Bin folder by default.
Set the appropriate paths in your code or build settings if you wish to change this.
The executables are labelled by T<tutorial num><Debug | Release>.exe.

Each tutorial requires three files the paths of which you may have to set. These are:-
ia3d.lib, ia3d.h, and dsound.lib. In MSDev, you can set paths for these under Tools/Options in
the Directories tab. Otherwise, you can set the whole paths within your build settings.
If you get build errors, this will most likely be the cause...



The "Common" folder contains classes describing A3D objects. These classes wrap some of the
initialization functionality but are in no way meant to be comprehensive. There are three
files within this folder: a3d.cpp and a3d.h are the class definitions, and wave.h describes
some utility structures for loading wavefiles.


You may use this tutorial code freely, but must retain the copyright information as at the top
of this readme file as a minimum. Also, please read the disclaimer below...


DISCLAIMER: This tutorial code is meant simply to illustrate some of the audio-functionality
available with A3D-Interactive. This code is provided as-is and is not intended to be used as
the basis for an audio-rendering library in other applications.

...If you do so, do it at your own risk!

WATCOM BUILDS

Included in each tutorial subdirectory are watcom make files (Tn_WCOM.MK).
To build using the watcom compiler do the following:

1. Edit the file watcom.mk in this directory to point to Watcom and DirectX include and 
	Library directories.
2. To make tutorial n  the files from the command line do:
	CD Tn
	WMAKE /f Tn_WCOM.MK Tn.EXE

The make file will create an executable tn.exe in the project directory and also copy 
to the bin directory.

The Watcom make files only builds the release versions.  There is no make file for
Building the ia3d.lib.

Suneil Mishra.
Aureal Semiconductor.
(December, 1996)
