/*
 * vliutils.h
 *
 * $Id: vliutils.h,v 1.11 2002/05/23 22:00:03 vesper Exp $
 *
 * Module to implement various utilities
 *
 *    Copyright 1998, 1999, 2000 by Mitsubishi Electric Information Technology 
 *    Center America, Real Time Visualization. All Rights Reserved
 *    Copyright 2001, 2002 by Real Time Visualization, TeraRecon, Inc.
 *    All Rights Reserved.
 */

#ifndef VLIUTILS_H
#define VLIUTILS_H


#include "vli3.h"
using namespace vli3;
// Compose and Decompose translate between two representations of
// nonsingular affine transformations, the standard 4x4 matrix
// and a decomposition into translation, rotation, shear and scale
// matricies as given by the following equation.
//
//           | 1 0 0 Translation[0] |
//           | 0 1 0 Translation[1] |
// Matrix == | 0 0 1 Translation[2] | * outRotation
//           | 0 0 0 1              |
//
//   | 1 Shear[0] Shear[1] 0 |   | Scale[0] 0        0        0 |
//   | 0 1        Shear[2] 0 |   | 0        Scale[1] 0        0 |
// * | 0 0        1        0 | * | 0        0        Scale[2] 0 |
//   | 0 0        0        1 |   | 0        0        0        1 |

VLIMatrix VLIUtCompose(VLIMatrix& inRotation,		// Rotation.
								 double inScale[3],			// X, Y, and Z Scales.
								 double inTranslation[3],	// X, Y, and Z Translations.
								 double inShear[3]);		// XY, XZ, and YZ shear.

void VLIUtDecompose (const VLIMatrix& inMatrix,	// Matrix to decompose
							   VLIMatrix& outRotation,		// Rotation.
							   double outScale[3],			// X, Y, and Z Scales.
							   double outTranslation[3],	// X, Y, and Z Translations.
							   double outShear[3]);			// XY, XZ, and YZ shear.

// VPNet: VLIStatus is an int within "nvr.h", so the following will not work
#ifndef kNVLI

// Returns a string describing the given VLIStatus.
const char *VLIUtGetErrorString(VLIStatus status);
#endif

// VPNet Coverage also uses the following
#if (kVLIMajorVersion >= 3) || defined (NVR_H)

#if qStdCppLibrary
	#include <iostream>
	using std::ostream;
	using std::endl;

#else

	#ifdef hpux
	#include <ostream>
	#else
	#include <ostream>
	#endif

#endif

//ostream & operator << ( ostream& os, const VLIMatrix & m);
//ostream & operator << ( ostream& os, const VLIVector2D & v); 
//ostream & operator << ( ostream& os, const VLIVector3D & v);
//ostream & operator << ( ostream& os, const VLIVector4D & v);
#endif

#endif  //VLIUTILS_H

