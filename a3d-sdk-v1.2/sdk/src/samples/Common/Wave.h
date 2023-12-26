//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//
//	Wave.h			Utility structures for handling audio wavefiles.
//
//	Created:			25th November, 1996, by Suneil Mishra.
//						Copyright (C) 1996 Aureal Semiconductor. All Rights Reserved. 	
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __WAVE_H
#define __WAVE_H

typedef struct _WavCtrl
{
	DWORD	nSamplesPerSec;	// samplerate.
	WORD	wBitsPerSample;	// bitrate.
	WORD	nBlockAlign;		// 
	WORD	nChannels;			// mono or stereo file.
	WORD	wFormatTag;			// wavefile format.
	DWORD	nAvgBytesPerSec;	// 
	long	samples;				// total number of samples.
	char	path[256];			// wave file path name
	void	*data;				// pointer to wave file data in memory
} WavCtrl;

typedef struct _CHUNK
{
	FOURCC	id;				// chunk ID						
	DWORD		size;				// chunk size					
	FOURCC	Type;				// form type or list type	
} CHUNK;

typedef struct _TAG
{
	FOURCC	id;  				// chunk ID			
	DWORD		size;				// chunk size		
} TAG;


#endif // #ifndef __WAVE_H


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
