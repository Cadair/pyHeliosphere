#ifndef VLI_H
#define VLI_H

/******************************************************************************
 *
 *  $Id: vli.h,v 1.76 2001/12/21 18:36:58 vesper Exp $
 *
 *  Purpose:
 *      This header file includes the version 3 vli3.h header file. It also
 *      specifies that the vli3 namespace where all of the vli objects and
 *      types are defined should be accessable without having to prefix them.
 *
 *  Copyright 2001 by Real Time Visualization, TeraRecon, Inc.
 *  All Rights Reserved.
 *
 *****************************************************************************/

// The following flags allow different compile-time behavior.
// The defaults are in this header file, but can be overridden by
// the application, setting them either in the program source (#define)
// or by compile switches (/D or -D)

// VLIAllowDeprecated

// This controls the functionality marked DEPRECATED. If this
// flag is set to 0, the deprecated interface is not available. This allows
// for application writers to quickly find out what deprecated interfaces
// are being used by their application.

// VLIAllowDeprecated is set to 1 by default

// VLIDummyCursor

// This flag controls the VolumePro (TM) 500 hardware cursor functionality.
// If it is set to 0, the VLICursor class is not available.
// Otherwise, the VLICursor class is available, but will have no affect
// on the volume rendering.

// VLIDummyCursor is set to 0 by default

// VLIAllowObsolete

// This controls the functionality marked OBSOLETE. If this
// flag is set to 0, the obsolete interface is not available. This allows
// for application writers to quickly find out what obsolete interfaces
// are being used by their application.

// VLIAllowObsolete is set to 0 by default


#include "vli3.h"

using namespace vli3;

#endif /* VLI_H */
