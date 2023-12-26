/*---------------------------------------------------------------------------
 *
 * $Workfile: macro3d.h $
 *
 *---------------------------------------------------------------------------
 *
 * $Author: Ndo $
 * $Revision: 6 $  
 *				
 *---------------------------------------------------------------------------
 *
 * Macros for 3D math functions.
 *
 *---------------------------------------------------------------------------
 * 
 * Copyright (c) 1998-1999 Aureal Semiconductor, Inc. - All rights reserved. 
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Aureal Semiconductor;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Aureal Semiconductor.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to restrictions
 * as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
 * and Computer Software clause at DFARS 252.227-7013, and/or in similar or
 * successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
 * rights reserved under the Copyright Laws of the United States.
 *
 *---------------------------------------------------------------------------
 *
 * Creator: Nam Do
 * Created:	5/27/98
 *
 *---------------------------------------------------------------------------
 *$NoKeywords: $
 */


#ifndef _MACRO3D_H_
#define _MACRO3D_H_

// Dot product
#define V_DOT3(_v1, _v2) \
(_v1[0]*_v2[0] + _v1[1]*_v2[1] + _v1[2]*_v2[2])

// Modulus length of a vector 
#define V_LENGTH(_v) \
((A3DVAL) sqrt(_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]))

#define V_LENGTH_FAST(_v) \
((A3DVAL) fmath::fsqrt(_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]))

// squared length of a vector 
#define V_LENGTH_SQUARED(_v) \
((A3DVAL) (_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]))

// Cross product
#define V_CROSS3(_r,_v1,_v2) \
{ _r[0] = (_v1[1] * _v2[2]) - (_v1[2] * _v2[1]);  \
  _r[1] = (_v1[2] * _v2[0]) - (_v1[0] * _v2[2]);  \
  _r[2] = (_v1[0] * _v2[1]) - (_v1[1] * _v2[0]);  }

// Scale a vector
#define V_SCALE3(_v,_s) \
{_v[0] *= _s; _v[1] *= _s; _v[2] *= _s;}

// Subtract to vectors
#define V_SUBTRACT3(_r, _to, _from) \
{_r[0] = _to[0] - _from[0];\
 _r[1] = _to[1] - _from[1];\
 _r[2] = _to[2] - _from[2];}

// Add two vectors
#define V_ADD3(_r, _to, _from) \
{_r[0] = _to[0] + _from[0];\
 _r[1] = _to[1] + _from[1];\
 _r[2] = _to[2] + _from[2];}

// Equate a=b
#define V_EQUATE3(a,b) {a[0]=b[0]; a[1]=b[1]; a[2]=b[2];}

// Zero a vector
#define V_ZERO_VECTOR(_v) \
{_v[0] = A3DVAL(0);\
 _v[1] = A3DVAL(0);\
 _v[2] = A3DVAL(0);\
 _v[3] = A3DVAL(0);}

#define V_ZERO_VERTEX(_v) \
{_v[0] = A3DVAL(0);\
 _v[1] = A3DVAL(0);\
 _v[2] = A3DVAL(0);\
 _v[3] = A3DVAL(1);}

// Unit vector X axis
#define V_AXIS_X(_v) \
{_v[0] = A3DVAL(1);\
 _v[1] = A3DVAL(0);\
 _v[2] = A3DVAL(0);\
 _v[3] = A3DVAL(0);}

// Unit vector Y axis
#define V_AXIS_Y(_v) \
{_v[0] = A3DVAL(0);\
 _v[1] = A3DVAL(1);\
 _v[2] = A3DVAL(0);\
 _v[3] = A3DVAL(0);}

// Unit vector Z axis
#define V_AXIS_Z(_v) \
{_v[0] = A3DVAL(0);\
 _v[1] = A3DVAL(0);\
 _v[2] = A3DVAL(1);\
 _v[3] = A3DVAL(0);}

// Identity matrix
#define M_IDENTITY(_m)  \
	memset(&_m, 0x00, sizeof(A3DMATRIX));\
	_m[0]  = A3DVAL(1);\
	_m[5]  = A3DVAL(1);\
	_m[10] = A3DVAL(1);\
	_m[15] = A3DVAL(1); 

#define A3D_PI      (3.14159265359f)
#define A3D_HALF_PI (1.570796326795f)
#define A3D_TWO_PI  (6.28318530718f)

#define A3D_RAD2DEG (57.29577951308f)
#define A3D_DEG2RAD (0.01745329251994f)    

// Heading angle in randians
// Ranges is from PI to -PI.
// Zero is in direction of nose.
// Increasing Positive angles go clockwise
// Decreasing Negative angles go counter clockwise
// Note: if fDir and Norm = 0, heading is meaningless

#define V_HEADING(_fH, _fDir, _fNorm) \
{\
	if (_fDir == 0.f)\
	{\
		if (_fNorm > 0)\
			_fH = HALF_PI;\
		else\
			_fH = -HALF_PI;\
	}\
	else\
	{\
		_fH = (A3DVAL) atan(_fNorm/_fDir);\
		if (_fDir < 0.f)\
		{\
			if (_fNorm > 0)\
				_fH += HALF_PI;\
			else\
				_fH -= HALF_PI;\
		}\
	}\
}

// Pitch angle in randians
// Range from HALF_PI to -HALF_PI
#define V_PITCH(_fP, _fDir, _fUp, _fNorm) \
{\
	if ((_fNorm == 0.f) && (_fDir == 0.f))\
	{\
		if (_fDir > 0)\
			_fP = HALF_PI;\
		else\
			_fP = -HALF_PI;\
	}\
	else\
	{\
		_fP = (A3DVAL)atan(_fUp/sqrt(_fDir*_fDir + _fNorm*_fNorm));\
	}\
}

#define M_MULTIPLY(r, a, b)		\
	r[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] +  a[12]*b[3];		\
	r[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] +  a[13]*b[3];		\
	r[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];		\
	r[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];		\
																\
	r[4] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] +  a[12]*b[7];		\
	r[5] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] +  a[13]*b[7];		\
	r[6] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];		\
	r[7] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];		\
																\
	r[8] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] +  a[12]*b[11];	\
	r[9] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] +  a[13]*b[11];	\
	r[10]= a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];	\
	r[11]= a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];	\
																\
	r[12] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] +  a[12]*b[15];\
	r[13] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] +  a[13]*b[15];\
	r[14] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];\
	r[15] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];


#define M_MULTIPLY_VECTOR(r, a, b)		\
	r[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] +  a[12]*b[3];		\
	r[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] +  a[13]*b[3];		\
	r[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];		\
	r[3] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];		


#define CLAMP_RANGE(fVal, fMin, fMax)		\
	if (fVal < fMin)		fVal = fMin;	\
	else if (fVal > fMax)	fVal = fMax;	
																
#endif
