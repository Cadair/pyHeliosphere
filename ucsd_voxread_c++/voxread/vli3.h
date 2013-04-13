#ifndef VLI3_H
#define VLI3_H

/******************************************************************************
 *
 *  vli.h      $Revision: 1.25 $    $Date: 2003/02/05 18:28:05 $
 *
 *   Purpose:
 *       This is a single header file that includes all VLI objects.
 *
 *   VLI (Volume Library Interface) 3.0 provides a software interface
 *   to the VolumePro 1000 Hardware. See http://www.rtviz.com
 *   if the Online help has not been installed.
 *
 *    Copyright 1998, 1999, 2000 by Mitsubishi Electric Information Technology
 *    Center America, Real Time Visualization. All Rights Reserved.
 *
 *    Copyright 2001 by Real Time Visualization, TeraRecon, Inc.
 *    All Rights Reserved.
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

#if ! defined (VLIAllowDeprecated)
#define VLIAllowDeprecated 1
#endif

// VLIDummyCursor

// This flag controls the VolumePro (TM) 500 hardware cursor functionality.
// If it is set to 0, the VLICursor class is not available.
// Otherwise, the VLICursor class is available, but will have no affect
// on the volume rendering.

// VLIDummyCursor is set to 0 by default

#if ! defined (VLIDummyCursor)
#define VLIDummyCursor 0
#endif

// VLIAllowObsolete

// This controls the functionality marked OBSOLETE. If this
// flag is set to 0, the obsolete interface is not available. This allows
// for application writers to quickly find out what obsolete interfaces
// are being used by their application.

// VLIAllowObsolete is set to 0 by default

#if ! defined (VLIAllowObsolete)
#define VLIAllowObsolete 0
#endif

/****************************************************************************
 *
 *              This section defines types used throughout VLI.
 *
 ****************************************************************************/

#define kVLIMajorVersion        3
#define kVLIMinorVersion        1
#define kVLIMicroVersion        0

namespace vli3
{

typedef char                    VLIint8;
typedef unsigned char           VLIuint8;

typedef short                   VLIint16;
typedef unsigned short          VLIuint16;

#ifdef _M_IX86
typedef long                    VLIint32;
typedef unsigned long           VLIuint32;

typedef          __int64        VLIint64;
typedef unsigned __int64        VLIuint64;
#else
typedef int                     VLIint32;
typedef unsigned int            VLIuint32;

typedef          long long      VLIint64;
typedef unsigned long long      VLIuint64;
#endif

typedef VLIuint64               VLISerialNumber;

typedef float                   VLIfloat32;
typedef double                  VLIfloat64;

typedef int                     VLIbool;
static const VLIbool VLIfalse = 0;
static const VLIbool VLItrue = !VLIfalse;

/******************************************************************************
 *
 *  This section defines limits of the hardware
 *
 *****************************************************************************/

static const int kVLIMaxVoxelFields         = 4;
static const int kVLIMaxPixelFields         = 4;
static const int kVLIMaxPixelBits           = 64;
static const int kVLIMaxLookupTables        = 4;
static const int kVLIMaxLookupTableFields   = 4;
static const int kVLIMaxOutputChannels      = 4;
static const int kVLICombinationUnits       = 3;
static const VLIuint32 kVLIMaxDepthValue    = 0x00ffffff;

// Different voxel formats for single field volumes.
// Holdover from VLI 1.0.

const VLIuint32 kVLIVoxelFormatUINT8    = 1;
const VLIuint32 kVLIVoxelFormatUINT12L  = 2;
const VLIuint32 kVLIVoxelFormatUINT12U  = 3;
const VLIuint32 kVLIVoxelFormatOther    = 4;            // Not one of the V1.0 types

enum VLIFieldFormat
{
    kVLIUnsignedFraction             = 0x00,        // represents 0 to 1; denominator is 2^n - 1;
                                                    // widen by repeating initial bits,
                                                    // narrow by rounding
    kVLISignedFraction               = 0x01,        // sign bit, plus a fraction 0 to 1 as above.
    kVLIUnsignedBitfield             = 0x02,        // NOT SUPPORTED: represents 0 to almost 1, denominator is 2^n;
                                                    // widen by filling with zeros on the right,
                                                    // narrow by truncating bits on the right
    kVLIUnsignedInteger              = 0x03,        // 0 to 2^n - 1;
                                                    // widen by filling with zeros on the left,
                                                    // narrow by truncating bits on the left
    kVLISignedInteger                = 0x10,        // NOT SUPPORTED: -2^(n-1) to 2^(n-1) - 1
                                                    // widen by filling with sign bits on the left,
                                                    // narrow by truncating bits on the left --
                                                    // in other words, straight 2's complement binary
    kVLIFloat                        = 0x11,        // NOT SUPPORTED: floating point
    kVLIUnknownFormat                = 0xff
};

class VLIFieldDescriptor
{
public:
    VLIFieldDescriptor (int inPos, int inSize, VLIFieldFormat inFormat = kVLIUnsignedFraction)
            : m_position (inPos), m_size (inSize), m_format (inFormat) {};

    VLIFieldDescriptor (void)
            : m_position (0), m_size (0), m_format (kVLIUnknownFormat) {};

    int                             m_position;     // in bits
    int                             m_size;         // in bits
    VLIFieldFormat                  m_format;
};

// Constants
const int kVLIMaxRenderBuffers      = 16;           // VLI 1.0 max # render buffers

typedef VLIuint32       VLIPixel;                   // Old definition


// Enum to define all the errors in VLI
enum VLIStatus
{
    // Successful
    kVLIOK                          = 0,    // 0 Successful
    kVLIMultiPass,                          // 1 OBSOLETE Multiple passes needed
    kVLITimeout,                            // 2 Timer went off before an event was done
    kVLIMustReload,                         // 3 Successfully allocated, but contents undefined
    kVLIInProgress,                         // 4 Successfully started, not yet complete

    // Not successful
    kVLIErrAlloc                    = 1001, // 1001 Could not allocate a needed resource
    kVLIErrVolume,                          // 1002 Volume is incorrect; generally means volume has no data storage
    kVLIErrVolumeInvalid = kVLIErrVolume,   // 1002 Old name for kVLIVolume
    kVLIErrMath,                            // 1003 Math error such as divide by zero
    kVLIErrArgument,                        // 1004 Argument value is incorrect
    kVLIErrUnimplemented,                   // 1005 Reserved for future
    kVLIErrTooMany,                         // 1006 Attempting to add too many objects to a context (e.g. CutPlanes)
    kVLIErrTooLarge,                        // 1007 Volume is too large to map or lock
    kVLIErrDuplicate,                       // 1008 Attempting to add a duplicate object to a context
    kVLIErrNoHardware,                      // 1009 No Volume Graphics hardware is present
    kVLIErrReadOnly,                        // 1010 Cannot satisfy 'write' request because volume is read-only
    kVLIErrAccess,                          // 1011 Cannot currently satisfy access request because of current access usage
    kVLIErrAborted,                         // 1012 Rendering process was aborted
    kVLIErrBasePlaneAllocation,             // 1013 Out of pixel buffers
    kVLIErrBasePlaneTooLarge,               // 1014 BasePlane is too large for graphics board
    kVLIErrHardware,                        // 1015 Undetermined hardware error
    kVLIErrInternal,                        // 1016 Internal error
    kVLIErrIDRollover,                      // 1017 Internal IDs have maxxed out. Exit all VLI apps, then try again.
    kVLIErrVersion,                         // 1018 Application was built against another version of the library
    kVLIErrMaxAngleDeviation,               // 1019 view angle workaround failed
    kVLIErrFormat,                          // 1020 bad format specifier
    kVLIErrCantResize,                      // 1021 Attempt to resize a volume that cannot be resized.
    kVLIErrCantSubsample,                   // 1022 Volume too large for image (or, image too small for volume)
    kVLIErrClassifier,                      // 1023 Classifier is not valid
    kVLIErrTransform,                       // 1024 Transformation state is not valid
    kVLIErrPermission,                      // 1025 Permission denied for an operation on a shared buffer
    kVLIErrVoxFile,                         // 1026 Vox file formatted incorrectly (header problems, etc) 
    kVLIErrRead,                            // 1027 Error reading, possibly lack of data 
    kVLIErrWrite,                           // 1028 Error writing, possibly lack of disk space 
    kVLIErrBuffer,                          // 1029 (Render) Image or depth buffer needed but missing or invalid 
    kVLIErrEmptyRender,                     // 1030 (Render) Viewport does not intersect output buffer range 
    kVLIErrCutplaneOverflow                 // 1031 (Render) Cut plane parameters overflow hardware range
};

inline VLIbool VLIFailed(VLIStatus inStatus)    { return inStatus >= kVLIErrAlloc; }
inline VLIbool VLISucceeded(VLIStatus inStatus) { return inStatus <  kVLIErrAlloc; }

// Pixel Formats supported in VLI

enum VLIPixelFormat
{
    kVLIPixelFormatRGBA8,           // 1 byte per component
    kVLIPixelFormatBGRA8,           // 1 byte per component
    kVLIPixelFormatARGB8,           // 1 byte per component
    kVLIPixelFormatABGR8,           // 1 byte per component
    kVLIPixelFormatRGBA4,           // 2 components per byte
    kVLIPixelFormatBGRA4,           // 2 components per byte
    kVLIPixelFormatARGB4,           // 2 components per byte
    kVLIPixelFormatABGR4,           // 2 components per byte
    kVLIPixelFormatRGBA12,          // 12 bits per component in a 64-bit unit; everything right justified: 0000RRRGGGBBBAAA
    kVLIPixelFormatBGRA12,          // 12 bits per component
    kVLIPixelFormatARGB12,          // 12 bits per component
    kVLIPixelFormatABGR12,          // 12 bits per component
    kVLIPixelFormatLA8,             // Luminance Alpha; 1 byte per component
    kVLIPixelFormatLA12,            // Luminance Alpha; 12 bits per component in a 32-bit unit; right justfied: 00LLLAAA
                                    // This could be: LLL0AAA0 instead.
    kVLIPixelFormatRGBA     = kVLIPixelFormatRGBA8,
    kVLIFirstPixelFormat    = kVLIPixelFormatRGBA8,
    kVLILastPixelFormat     = kVLIPixelFormatLA12
};

enum VLIBlendMode
{
    kVLIBlendNone                   = 0,            // Accumulation color is not changed
    kVLIBlendReplace                = 1,            // Accumulation color is replaced with sample
    kVLIBlendBTFNotWeighted         = 2,            // back to front with non-weighted colors
    kVLIBlendBTFPreWeighted         = 3,            // back to front with opacity weighted colors
    kVLIBlendFTBNotWeighted         = 4,            // front to back with non-weighted colors
    kVLIBlendFTBPreWeighted         = 5,            // front to back with opacity weighted colors
    // Modes 6 to 15 are not supported for BlendLast, only BlendAccum
    kVLIBlendSumAndCount            = 6,            // Report the sum of alpha values and the count of samples
    kVLIBlendEmptyBits              = 7,            // NOT SUPPORTED
    // The BlendMinXxx modes replace the entire accumulated color with sample color if
    // the sample Xxx field is less than the accumulated Xxx field
    kVLIBlendMinRed                 = 8,            // replace accumulated color if sample red is less than accum red
    kVLIBlendMinGreen               = 9,            // replace accumulated color if sample green is less than accum green
    kVLIBlendMinBlue                = 10,           // replace accumulated color if sample blue is less than accum blue
    kVLIBlendMinAlpha               = 11,           // replace accumulated color if sample alpha is less than accum alpha
    // The BlendMaxXxx modes replace the entire accumulated color with sample color if
    // the sample Xxx field is greater than the accumulated Xxx field
    kVLIBlendMaxRed                 = 12,           // replace accumulated color if sample red is greater than accum red
    kVLIBlendMaxGreen               = 13,           // replace accumulated color if sample green is greater than accum green
    kVLIBlendMaxBlue                = 14,           // replace accumulated color if sample blue is greater than accum blue
    kVLIBlendMaxAlpha               = 15,           // replace accumulated color if sample alpha is greater than accum alpha
    // Old names and aliases
    kVLIBlendMIP                    = kVLIBlendMaxAlpha,            // Maximum Intensity Projection
    kVLIBlendMINIP                  = kVLIBlendMinAlpha,            // MINimum Intensity Projection
    kVLIBlendBTF                    = kVLIBlendBTFNotWeighted,
    kVLIBlendFTB                    = kVLIBlendFTBNotWeighted       // Front to back, standard blending method
};


enum VLICoordinateSpace
{
    kVLIVoxelSpace,                                 // Voxel space; one unit equals one voxel
    kVLIObjectSpace,                                // Object/Volume space; corrected for anisotropy and shear
    kVLIWorldSpace,                                 // World Space
    kVLICameraSpace,                                // Camera/Eye space
    kVLIBasePlaneSpace,                             // Baseplane space; used only for shear-warp rendering
    kVLIImageSpace                                  // Image/Window space; XY values are in pixels,
                                                    // Z is normalized to the range 0.0 to 1.0
};

enum VLIAccessType
{
    kVLIAccessNoControl,                            // No readers or writers
    kVLIAccessReadExclusive,                        // One reader, no writers
    kVLIAccessReadShared,                           // Multiple readers, no writers

    kVLIAccessWrite,                                // Internal use only
    kVLIAccessWriteExclusive,                       // No readers, one writer
    kVLIAccessWriteShared,                          // Multiple readers, multiple writers  DANGEROUS!!

    kVLIAccessWriteOnlyExclusive                    // Access for write only; no reading permitted
};

enum VLILocation
{
    kVLIBoard0              = 0,                    // Specific board
    kVLIBoard1              = 1,
    kVLIBoard2              = 2,
    kVLIBoard3              = 3,
    kVLIBoard4              = 4,
    kVLIBoard5              = 5,
    kVLIBoard6              = 6,
    kVLIBoard7              = 7,
    kVLIAnyBoard            = 1000,                 // Can create on any board
    kVLISystem              = 2000,                 // System memory (kernel allocated, shareable by many processes)
    kVLIMultiBoard          = 3000,                 // Multiple boards
    kVLINoBufferAttached    = -1                    // There is no buffer attached to this volume
};

enum VLITarget
{
    kVLISource              = 1,                    // Update/Map to the source of the volume
    kVLIBuffer              = 2                     // Update/Map to the buffer of the volume
};

enum VLISource
{
    kVLIUseTarget           = 0,                    // Copy from the current target
    kVLIUseSource           = 1,                    // Copy from the source
    kVLIUseBuffer           = 2                     // Copy from the buffer
};

enum VLIPermission
{
    kVLIPermissionNotShared     = 0,                // No permissions
    kVLIPermissionRead          = 1,                // Other processes can read buffer
    kVLIPermissionWrite         = 2,                // Other processes can write to buffer
    kVLIPermissionReadWrite     = kVLIPermissionRead | kVLIPermissionWrite,
    kVLIPermissionResize        = 4,                // Other processes can resize buffer
    kVLIPermissionSetPermission = 8,                // Other processes can change permissions
    ALLPERMISSIONBITS           = 0xf
};

inline VLIPermission operator | (VLIPermission a, VLIPermission b) { return VLIPermission ( (unsigned int) (a) | (unsigned int) (b) ); }
inline VLIPermission operator & (VLIPermission a, VLIPermission b) { return VLIPermission ( (unsigned int) (a) & (unsigned int) (b) ); }
inline VLIPermission operator ~ (VLIPermission a) { return ALLPERMISSIONBITS & VLIPermission ( ~ (unsigned int) (a) ); }
inline VLIPermission & operator |= (VLIPermission &a, VLIPermission b) { a = a | b; return a; }
inline VLIPermission & operator &= (VLIPermission &a, VLIPermission b) { a = a & b; return a; }

enum VLIChannel
{
    kVLIChannelRed,
    kVLIChannelGreen,
    kVLIChannelBlue,
    kVLIChannelAlpha
};

enum VLIFieldNumber
{
    kVLIField0,
    kVLIField1,
    kVLIField2,
    kVLIField3
};

enum VLITableNumber
{
    kVLITable0,
    kVLITable1,
    kVLITable2,
    kVLITable3
};

typedef VLIuint64       VLIBufferID;
static const VLIBufferID kVLINoBuffer (0);
static const VLIBufferID kVLIMultipleBuffers (~VLIBufferID(1));

enum VLIAddressMode
{
    kVLIOffsetAddress,
    kVLIWrapAddress
};

enum VLIInterpolationMode       // How do we interpolate samples between voxels?
{
    kVLINearestNeighbor,        // Use the value of the closest voxel
    kVLITrilinear               // Interpolate linearly from the 8 voxels surrounding the sample
};

typedef VLIuint32 VLIRGBAPacked;

inline VLIRGBAPacked VLIPackRGBA(VLIuint32 r, VLIuint32 g, VLIuint32 b, VLIuint32 a)
{
    return (r<<24) | (g<<16) | (b<<8) | a ;
}

inline VLIuint32 VLIGetRed(VLIRGBAPacked packed)
{
    return (packed >> 24) & 0xff;
}
inline VLIuint32 VLIGetGreen(VLIRGBAPacked packed)
{
    return (packed >> 16) & 0xff;
}
inline VLIuint32 VLIGetBlue(VLIRGBAPacked packed)
{
    return (packed >> 8) & 0xff;
}
inline VLIuint32 VLIGetAlpha(VLIRGBAPacked packed)
{
    return (packed >> 0) & 0xff;
}

// The following is needed to generate WIN32 dll

#if defined(WIN32)
#  ifdef VLI_MAKE_DLL
#     define VLIEXPORT __declspec( dllexport )
#  else
#     define VLIEXPORT __declspec( dllimport )
#  endif
#else
#  undef  VLIEXPORT
#  define VLIEXPORT
#endif

// Macros for deprecated and obsolete interface
// support

#if VLIAllowDeprecated
#define VLI_DEPRECATED(x_)  x_
#else
#define VLI_DEPRECATED(x_)  x_ ## Deprecated
#endif

#if VLIAllowObsolete
#define VLI_OBSOLETE(x_)    x_
#else
#define VLI_OBSOLETE(x_)    x_ ## Obsolete
#endif

/***************************************************************************
 *
 *       Global functions that handle system initialization
 *       and configuration.
 *
 **************************************************************************/

VLIEXPORT VLIStatus         VLIOpenInternal (           // Open VLI for use
                                int majorVersion,       // Compile time major version number
                                int minorVersion        // Compile time minor version number
                            );

inline VLIStatus            VLIOpen(void)               // Open VLI for use
{
    return VLIOpenInternal(kVLIMajorVersion, kVLIMinorVersion);
}

VLIEXPORT void              VLIClose(void               // Close VLI for all usage
                            );

VLIEXPORT VLIStatus         VLISetEpsilon (             // Set the epsilon value used for testing against zero
                                double inEpsilon        // new epsilon value (should be on the order of 1e-7 or so)
                            );

VLIEXPORT double            VLIGetEpsilon (void         // Return the epsilon value used for testing against zero
                            );

VLIEXPORT void              VLISetParameter (           // Set a global parameter
                                const char *inName,     // parameter name
                                VLIuint64 inValue       // parameter value
                            );

VLIEXPORT VLIuint64         VLIGetParameter (           // Return a global parameter
                                const char *inName      // parameter name
                            );

// Shareable objects need reference counts and
// serial numbers

class VLIEXPORT VLIShareable
{

protected:
    // The constructors are protected; derived classes should have protected or private constructors.

    // NOTE: Whoever does a new of a VLIShareable-derived object *must* call
    // AddRef() when the new returns. This must be done by all "Create" routines,
    // as they are expected to return an object with refcount 1.

                            VLIShareable (void);        // Protected Constructor

                            VLIShareable(               // Protected Copy constructor
                                const VLIShareable & copyFrom
                            );

    virtual                 ~VLIShareable (void         // Protected Destructor
                            ) = 0;

    VLIShareable &          operator = (                // assignment operator
                                const VLIShareable & copyFrom
                            );

public:

    int                     RefCount (void) const;      // Return reference count (number of references to this object)

    int                     AddRef (void);              // Increment the reference count

    int                     Release(void);              // Decrement the reference count; if it goes to zero, then destruct

    void                    IncSerial(void);            // Increment the serial number (change count)

    VLISerialNumber         GetSerial (void) const;     // Return the serial number (change count)

    VLISerialNumber         GetObjectID (void) const;   // Return the object number

public:
    // A public data member for application usage. Except for initializing it to null,
    // VLI does not change or reference it.
    void                    *m_appData;                 // Application data


private:                    // Data
    int                     m_refCount;
    VLISerialNumber         m_serial;
    const VLISerialNumber   m_object;

};


/****************************************************************************
*
*       This section describes the VLISyncEvent class.
*
****************************************************************************/

static const long kVLIWaitForever = -1;
static const int kVLIMaxEventCount = 8; // Maximum number of events for WaitForAny

class VLIEXPORT VLISyncEvent
{
public:
    // Constructors
                            VLISyncEvent (              // Convert from status to sync event
                                VLIStatus inStatus = kVLIErrArgument
                            );

                            VLISyncEvent (              // Construct a VLISyncEvent from an existing one
                                const VLISyncEvent & inEvent
                            );

    VLISyncEvent &          operator = (                // Copy a VLISyncEvent
                                const VLISyncEvent & inEvent
                            );

    VLIStatus               Status (void                // Return the status of the initiating routine
                            );

    VLIbool                 IsDone (void                // Return VLItrue if completely done
                            );

    VLIStatus               Wait (                      // Wait until this event is complete and return the status of the operation
                                long inTimeoutInMS = kVLIWaitForever    // timeout
                            );

    static VLIStatus        WaitForAny (                // Wait for any of the events in the array
                                int inNumEvents,                // number events in the array
                                VLISyncEvent inEventArray[],    // array event
                                long inTimeoutInMS = kVLIWaitForever            // timeout
                            );

    VLIStatus               Abort (void                 // Abort the operation denoted by this event object
                            );

    VLIbool                 operator == (               // Return VLItrue if the events are identical
                                const VLISyncEvent & inEvent
                            ) const;

    VLIbool                 operator != (               // Return VLItrue if the events are not identical
                                const VLISyncEvent & inEvent
                            ) const;

protected:
    VLIuint64               m_private;

};

/****************************************************************************
*
*       This section describes the VLIVector2D class/object.
*       This is subset of the vector3 class from the graphics gems
*       vector math library.
*
****************************************************************************/

class VLIEXPORT VLIVector2D
{
public:

    // Constructors

                            VLIVector2D (void           // Default constructor; sets vector to (0,0)
                            );

                            VLIVector2D (               // Create a VLIVector2D from another one
                                const VLIVector2D& v    // input vector
                            );

                            VLIVector2D (               // Constructor with two double values
                                double x,               // x value
                                double y                // y value
                            );

                            VLIVector2D (               // Constructor with double array
                                const double [2]
                            );

                            VLIVector2D (               // Constructor with float array
                                const float  [2]
                            );

    // Methods to assign new values to a VLIVector2D

    VLIVector2D&            Assign (                    // Assign x and y from separate arguments
                                double x,               // x value
                                double y                // y value
                            );

    VLIVector2D&            Assign (                    // Assign x and y from double array
                                const double [2]
                            );

    VLIVector2D&            Assign (                    // Assign x and y from float array
                                const float [2]
                            );

    VLIVector2D&            operator = (                // Assign x and y from VLIVector2D
                                const VLIVector2D& v
                            );

    // Array access, for reading or writing

    double&                 X(void);                    // X component
    double&                 Y(void);                    // Y component
    double                  X(void) const;              // X component
    double                  Y(void) const;              // Y component

    double&                 operator [](int);           // Direct access to components
    double                  operator [](int) const;     // Direct access to components

    // Reading out the values

    void                    CopyTo (                    // Copy components to float arguments
                                float *x,               // RETURNED x component
                                float *y                // RETURNED y component
                            ) const;

    void                    CopyTo (                    // Copy components to double arguments
                                double *x,              // RETURNED x component
                                double *y               // RETURNED y component
                            ) const;

    void                    CopyTo (                    // Copy components to float array
                                float [2]               // RETURNED x and y components
                            ) const;

    void                    CopyTo (                    // Copy components to double array
                                double [2]              // RETURNED x and y components
                            ) const;

    // Math operators

    VLIVector2D&            operator +=(const VLIVector2D&);
    VLIVector2D&            operator -=(const VLIVector2D&);
    VLIVector2D&            operator *=(double);
    VLIVector2D&            operator /=(double);

    VLIVector2D             operator +(const VLIVector2D&) const;
    VLIVector2D             operator -(const VLIVector2D&) const;
    VLIVector2D             operator -(void) const;
    VLIVector2D             operator *(double) const;
    VLIVector2D             operator /(double) const;

    VLIbool                 operator ==(const VLIVector2D&) const;
    VLIbool                 operator !=(const VLIVector2D&) const;

    double                  Length (void) const;        // SQRT(x*x+y*y)
    double                  LengthSquared (void) const; // x*x + y*y

    VLIVector2D&            Normalize(                  // Make length equal to argument
                                double to=1.0           // desired vector length
                            );

    double                  Dot (                       // Return dot product of this and given vector
                                const VLIVector2D&      // given vector
                            ) const;

protected:
    double                  m_v[2];
};

VLIEXPORT VLIVector2D operator *(
                            double,
                            const VLIVector2D&
                        );

VLIEXPORT double VLIDot(                        // Return dot product of two vectors
                            const VLIVector2D&,
                            const VLIVector2D&
                        );


VLIEXPORT double VLIDistance (              // Return distance between two points
                            const VLIVector2D&,
                            const VLIVector2D&
                        );

VLIEXPORT double VLIDistanceSquared (       // Return square of distance between two points
                            const VLIVector2D&,
                            const VLIVector2D&
                        );

/****************************************************************************
*
*        This section describes VLIVector3D objects. It is mostly a subset
*        of the vector3 class from the graphics gems vector math library.
*
***************************************************************************/


class VLIEXPORT VLIVector3D
{
public:

    // Constructors

                            VLIVector3D (void           // Default constructor; sets vector to (0,0,0)
                            );

                            VLIVector3D (               // Create a VLIVector3D from another one
                                const VLIVector3D& v    // input vector
                            );

                            VLIVector3D (               // Constructor with three double values
                                double x,               // x value
                                double y,               // y value
                                double z                // z value
                            );

                            VLIVector3D (               // Constructor with double array
                                const double [3]
                            );

                            VLIVector3D (               // Constructor with float array
                                const float  [3]
                            );

    // Methods to assign new values to a VLIVector3D

    VLIVector3D&            Assign (                    // Assign x, y and z from separate arguments
                                double x,               // x value
                                double y,               // y value
                                double z                // z value
                            );

    VLIVector3D&            Assign (                    // Assign x, y and z from double array
                                const double [3]
                            );

    VLIVector3D&            Assign (                    // Assign x, y and z from float array
                                const float [3]
                            );

    VLIVector3D&            operator = (                // Assign x, y and z from VLIVector3D
                                const VLIVector3D& v
                            );

    // Array access, for reading or writing

    double&                 X(void);                    // X component
    double&                 Y(void);                    // Y component
    double&                 Z(void);                    // Z component
    double                  X(void) const;              // X component
    double                  Y(void) const;              // Y component
    double                  Z(void) const;              // Z component

    double&                 operator [](int);           // Direct access to components
    double                  operator [](int) const;     // Direct access to components

    // Reading out the values

    void                    CopyTo (                    // Copy components to float arguments
                                float *x,               // RETURNED x component
                                float *y,               // RETURNED y component
                                float *z                // RETURNED z component
                            ) const;

    void                    CopyTo (                    // Copy components to double arguments
                                double *x,              // RETURNED x component
                                double *y,              // RETURNED y component
                                double *z               // RETURNED z component
                            ) const;

    void                    CopyTo (                    // Copy components to float array
                                float [3]               // RETURNED components
                            ) const;

    void                    CopyTo (                    // Copy components to double array
                                double [3]              // RETURNED components
                            ) const;

    // Math operators

    VLIVector3D&            operator +=(const VLIVector3D&);
    VLIVector3D&            operator -=(const VLIVector3D&);
    VLIVector3D&            operator *=(double);
    VLIVector3D&            operator /=(double);

    VLIVector3D             operator +(const VLIVector3D&) const;
    VLIVector3D             operator -(const VLIVector3D&) const;
    VLIVector3D             operator -(void) const;
    VLIVector3D             operator *(double) const;
    VLIVector3D             operator /(double) const;

    VLIbool                 operator ==(const VLIVector3D&) const;
    VLIbool                 operator !=(const VLIVector3D&) const;

    double                  Length (void) const;        // SQRT(x*x + y*y + z*z)
    double                  LengthSquared (void) const; // x*x + y*y + z*z

    VLIVector3D&            Normalize(                  // Make length equal to argument
                                double to=1.0           // desired vector length
                            );

    double                  Dot (                       // Return dot product of this and given vector
                                const VLIVector3D&      // given vector
                            ) const;

protected:
    double                  m_v[3];
};

VLIEXPORT VLIVector3D operator *(double, const VLIVector3D&);

VLIEXPORT double VLIDot(                        // Return dot product of two vectors
                            const VLIVector3D&,
                            const VLIVector3D&
                        );

VLIEXPORT VLIVector3D VLICross (                // Return cross product of two vectors
                            const VLIVector3D&,
                            const VLIVector3D&
                        );

VLIEXPORT double VLIDistance (              // Return distance between two points
                            const VLIVector3D&,
                            const VLIVector3D&
                        );

VLIEXPORT double VLIDistanceSquared (       // Return square of distance between two points
                            const VLIVector3D&,
                            const VLIVector3D&
                        );


/****************************************************************************
*
*        This section describes VLIVector4D objects. It is mostly a subset
*        of the vector4 class from the graphics gems vector math library.
*
***************************************************************************/


class VLIEXPORT VLIVector4D
{
public:

    // Constructors

                            VLIVector4D (void           // Default constructor; sets vector to (0,0,0,0)
                            );

                            VLIVector4D (               // Create a VLIVector4D from another one
                                const VLIVector4D& v    // input vector
                            );

                            VLIVector4D (               // Constructor with three double values
                                double x,               // x value
                                double y,               // y value
                                double z,               // z value
                                double w                // w value
                            );

                            VLIVector4D (               // Constructor with double array
                                const double [4]
                            );

                            VLIVector4D (               // Constructor with float array
                                const float  [4]
                            );

    // Methods to assign new values to a VLIVector3D

    VLIVector4D&            Assign (                    // Assign x, y and z from separate arguments
                                double x,               // x value
                                double y,               // y value
                                double z,               // z value
                                double w                // w value
                            );

    VLIVector4D&            Assign (                    // Assign x, y and z from double array
                                const double [4]
                            );

    VLIVector4D&            Assign (                    // Assign x, y and z from float array
                                const float [4]
                            );

    VLIVector4D&            operator = (                // Assign x, y and z from VLIVector4D
                                const VLIVector4D& v
                            );

    // Array access, for reading or writing

    double&                 X(void);                    // X component
    double&                 Y(void);                    // Y component
    double&                 Z(void);                    // Z component
    double&                 W(void);                    // W component
    double                  X(void) const;              // X component
    double                  Y(void) const;              // Y component
    double                  Z(void) const;              // Z component
    double                  W(void) const;              // W component

    double&                 operator [](int);           // Direct access to components
    double                  operator [](int) const;     // Direct access to components

    // Reading out the values

    void                    CopyTo (                    // Copy components to float arguments
                                float *x,               // RETURNED x component
                                float *y,               // RETURNED y component
                                float *z,               // RETURNED z component
                                float *w                // RETURNED w component
                            ) const;

    void                    CopyTo (                    // Copy components to double arguments
                                double *x,              // RETURNED x component
                                double *y,              // RETURNED y component
                                double *z,              // RETURNED z component
                                double *w               // RETURNED w component
                            ) const;

    void                    CopyTo (                    // Copy components to float array
                                float [4]               // RETURNED components
                            ) const;

    void                    CopyTo (                    // Copy components to double array
                                double [4]              // RETURNED components
                            ) const;

    // Math operators

    VLIVector4D&            operator +=(const VLIVector4D&);
    VLIVector4D&            operator -=(const VLIVector4D&);
    VLIVector4D&            operator *=(double);
    VLIVector4D&            operator /=(double);

    VLIVector4D             operator +(const VLIVector4D&) const;
    VLIVector4D             operator -(const VLIVector4D&) const;
    VLIVector4D             operator -(void) const;
    VLIVector4D             operator *(double) const;
    VLIVector4D             operator /(double) const;

    VLIbool                 operator ==(const VLIVector4D&) const;
    VLIbool                 operator !=(const VLIVector4D&) const;

    double                  Length (void) const;        // SQRT(x*x + y*y + z*z + w*w)
    double                  LengthSquared (void) const; // x*x + y*y + z*z + w*w

    VLIVector4D&            Normalize(                  // Make length equal to argument
                                double to=1.0           // desired vector length
                            );

    double                  Dot (                       // Return dot product of this and given vector
                                const VLIVector4D&      // given vector
                            ) const;

protected:
    double                  m_v[4];
};

VLIEXPORT VLIVector4D operator *(double, const VLIVector4D&);

VLIEXPORT double VLIDot(                        // Return dot product of two vectors
                            const VLIVector4D&,
                            const VLIVector4D&
                        );

VLIEXPORT VLIVector4D VLICross (                // Return cross product of two vectors
                            const VLIVector4D&,
                            const VLIVector4D&
                        );

VLIEXPORT double VLIDistance (              // Return distance between two points
                            const VLIVector4D&,
                            const VLIVector4D&
                        );

VLIEXPORT double VLIDistanceSquared (       // Return square of distance between two points
                            const VLIVector4D&,
                            const VLIVector4D&
                        );

/***************************************************************************
 *
 *    This section defines a 4x4 matrix class.
 *
 ***************************************************************************/

class VLIEXPORT VLIMatrix
{
public:

    // Constructors and object factories

                            VLIMatrix (void             // Default constructor: creates an identity matrix
                            );

                            VLIMatrix (                 // Copy constructor: create a matrix from the given matrix
                                const VLIMatrix&
                            );

    // The array constructors take in column major form,
    // the same as in OpenGL:
    // 0, 4,  8, 12
    // 1, 5,  9, 13
    // 2, 6, 10, 14
    // 3, 7, 11, 15

                            VLIMatrix (                 // Construct a matrix from a double array
                                const double inArray[16]
                            );

                            VLIMatrix (                 // Construct a matrix from a float array
                                const float inArray[16]
                            );

    // Constructor to specify by individual elements.
    // Elements are passed in row by row.
                            VLIMatrix (                 // Construct a matrix from 16 double arguments
                                double in00, double in01, double in02, double in03,
                                double in10, double in11, double in12, double in13,
                                double in20, double in21, double in22, double in23,
                                double in30, double in31, double in32, double in33
                            );

    static VLIMatrix        Identity  (void             // Explicit identity constructor
                            );

    static VLIMatrix        Rotate (                    // Construct a rotation matrix
                                double inDegrees,           // rotate this many degrees (positive uses right hand rule)
                                const VLIVector3D& inAxis   // axis about which to rotate
                            );

    static VLIMatrix        Scale (                     // Construct a scale matrix
                                double inSx,            // scale factor in X
                                double inSy,            // scale factor in Y
                                double inSz             // scale factor in Z
                            );

    static VLIMatrix        Translate (                 // Construct a translation matrix
                                double inTx,            // X translation value
                                double inTy,            // Y translation value
                                double inTz             // Z translation value
                            );

    static VLIMatrix        LookAt (                    // Construct a view matrix based on the camera paradigm
                                const VLIVector3D& inEye,   // position of the eye in world space
                                const VLIVector3D& inAim,   // position of the point of interest in world space
                                const VLIVector3D& inUp     // world space direction of the camera's up vector
                            );

    static VLIMatrix        Ortho (                     // Construct an orthographic (parallel) projection matrix
                                double inLeft,          // minimum X value in camera space
                                double inRight,         // maximum X value in camera space
                                double inBottom,        // minimum Y value in camera space
                                double inTop,           // maximum Y value in camera space
                                double inNegativeNear,  // negative of minimum Z value in camera space
                                double inNegativeFar    // negative of maximum Z value in camera space
                            );

    static VLIMatrix        Frustum (                   // Construct a perspective projection matrix
                                double inLeft,          // minimum X value in camera space
                                double inRight,         // maximum X value in camera space
                                double inBottom,        // minimum Y value in camera space
                                double inTop,           // maximum Y value in camera space
                                double inNegativeNear,  // negative of minimum Z value in camera space
                                double inNegativeFar    // negative of maximum Z value in camera space
                            );

    static VLIMatrix        Perspective (               // Another way to construct a perspective projection matrix
                                double inFieldOfViewY,  // field of view in Y (in degrees)
                                double inAspectRatio,   // ration of width to height
                                double inNegativeNear,  // negative of minimum Z value in camera space
                                double inNegativeFar    // negative of maximum Z value in camera space
                            );

    // Methods to assign new values to a VLIMatrix

    VLIMatrix&              Assign (                    // Assign new values from a double array
                                const double inArray[16]
                            );

    VLIMatrix&              Assign (                    // Assign new values from a float array
                                const float  inArray[16]
                            );


    // Assignment from individual elements.
    // Elements are passed in row by row.

    VLIMatrix&              Assign (                    // Assign new values from 16 double arguments
                                double in00, double in01, double in02, double in03,
                                double in10, double in11, double in12, double in13,
                                double in20, double in21, double in22, double in23,
                                double in30, double in31, double in32, double in33
                            );

    // Array access, for reading or writing

    double *                operator [](int);           // Direct access to components
    const double *          operator [](int) const;     // Direct access to components

    // Reading out the values

    void                    CopyTo (                    // Copy matrix to double array
                                double outArray[16]
                            ) const;

    void                    CopyTo (                    // Copy matrix to float array
                                float   outArray[16]
                            ) const;

    // Math operators

    VLIMatrix&              operator *=(const VLIMatrix&);
    VLIMatrix&              operator *=(double);

    VLIMatrix               operator *(double) const;
    VLIMatrix               operator *(const VLIMatrix&) const;

    VLIMatrix&              operator =(const VLIMatrix&);

    VLIVector4D             operator *(const VLIVector4D& inVec4D) const;

    VLIbool                 operator ==(const VLIMatrix&) const;
    VLIbool                 operator !=(const VLIMatrix&) const;

    VLIMatrix               Transpose(void) const;      // Return the transpose of the 4x4 Matrix

    VLIMatrix               Inverse (void) const;       // Return the inverse of the 4x4 matrix

    double                  Determinant(void) const;    // Return the determinant of the 4x4 matrix

    VLIMatrix               Adjoint(void) const;        // Return the adjoint matrix of the 4x4 matrix

    VLIbool                 IsSingular (void) const;    // Return VLItrue if the matrix is singular

    // Transform points and vectors

    void                    TransformPoint (            // Transform a VLIVector3D as a point (include translation)
                                const VLIVector3D& in,
                                VLIVector3D& out
                            ) const;

    void                    TransformVector (           // Transform a VLIVector3D as a vector (exclude translation)
                                const VLIVector3D& in,
                                VLIVector3D& out
                            ) const;

    void                    Transform (                 // Transform a VLIVector4D as a homogeneous point
                                const VLIVector4D& in,
                                VLIVector4D& out
                            ) const;

protected:

    double                  m_matrix[4][4];
};

VLIEXPORT VLIMatrix operator *(double inC, const VLIMatrix& inA);


/****************************************************************************
 *
 *       This section describes the VLICamera class that manages
 *       the view transformation.
 *
 ***************************************************************************/

class VLIEXPORT VLICamera
{

public:

    // The default constructor creates a camera at (0,0,1) with
    // aim (0,0,0) and up vector (0,1,0).

                            VLICamera (void);           // Default constructor

                            VLICamera (                 // Constructor with given view matrix
                                const VLIMatrix& inViewMatrix   // view matrix to use
                            );

                            VLICamera (                             // Constructor with complete data
                                const VLIMatrix& inViewMatrix,      // view matrix
                                const VLIMatrix &inProjectionMatrix,// projection matrix
                                int inWidth,                        // viewport width (pixels)
                                int inHeight,                       // viewport height (pixels)
                                int inOriginX = 0,                  // viewport origin (lower left corner) in X (pixels)
                                int inOriginY = 0,                  // viewport origin (lower left corner) in Y (pixels)
                                double inNear = 0.0,                // near depth value (0.0 to 1.0)
                                double  inFar = 1.0                 // far depth value (0.0 to 1.0)
                            );

    virtual                 ~VLICamera(void);               // Destructor

    VLIMatrix               GetViewMatrix (void             // Return view matrix
                            ) const;

    VLIStatus               SetViewMatrix (                 // Set view matrix
                                const VLIMatrix& inMatrix   // new view matrix
                            );

    VLIMatrix               GetProjectionMatrix (void       // Return projection matrix
                            ) const;

    VLIStatus               SetProjectionMatrix (           // Set projection matrix
                                const VLIMatrix& inMatrix   // new projection matrix
                            );

    void                    GetViewport (                   // Return viewport
                                int &outOriginX,            // RETURNED viewport origin (lower left) in X (pixels)
                                int &outOriginY,            // RETURNED viewport origin (lower left) in Y (pixels)
                                int &outWidth,              // RETURNED viewport size in X (pixels)
                                int &outHeight              // RETURNED viewport size in Y (pixels)
                            ) const;

    int                     GetViewportOriginX (void        // Return viewport Origin X (pixels)
                            ) const;

    int                     GetViewportOriginY (void        // Return viewport Origin Y (pixels)
                            ) const;

    int                     GetViewportWidth (void          // Return viewport Width (pixels)
                            ) const;

    int                     GetViewportHeight (void         // Return viewport Height (pixels)
                            ) const;

    VLIStatus               SetViewport (                   // Set viewport
                                int inOriginX,              // viewport origin (lower left) in X (pixels)
                                int inOriginY,              // viewport origin (lower left) in Y (pixels)
                                int inWidth,                // viewport size in X (pixels)
                                int inHeight                // viewport size in Y (pixels)
                            );

    void                    GetDepthRange (                 // Return depth range
                                double &outNear,            // RETURNED near depth value (0.0 to 1.0)
                                double &outFar              // RETURNED far depth value (0.0 to 1.0)
                            ) const;

    double                  GetDepthRangeNear (void         // Return near depth value (0.0 to 1.0)
                            ) const;

    double                  GetDepthRangeFar (void          // Return far depth value (0.0 to 1.0)
                            ) const;

    VLIStatus               SetDepthRange (                 // Set depth range
                                double inNear,              // near depth value (0.0 to 1.0)
                                double inFar                // far depth value (0.0 to 1.0)
                            );

    VLIMatrix               GetModelMatrix (void            // Return model matrix
                            ) const;

    VLIStatus               SetModelMatrix (                // Set model matrix
                                const VLIMatrix& inMatrix   // new model matrix
                            );

    // The old viewport was intended to be used to bound the volume being rendered.
    // It was never implemented.
    // Note that this interface is obsolete and should not be used.

    void                    VLI_OBSOLETE (GetViewport) (    // OBSOLETE Return old viewport size
                                double& outWidth,           // RETURNED old viewport width
                                double& outHeight           // RETURNED old viewport height
                            ) const;

    VLIStatus               VLI_OBSOLETE (SetViewport) (    // OBSOLETE Set old viewport size
                                double inWidth,             // old viewport width
                                double inHeight             // old viewport height
                            );

private:
    VLIMatrix               m_viewMatrix;
    VLIMatrix               m_projectionMatrix;
    VLIMatrix               m_modelMatrix;
    int                     m_vpX;
    int                     m_vpY;
    int                     m_vpWidth;
    int                     m_vpHeight;
    double                  m_vpNear;
    double                  m_vpFar;

    double                  m_obsWidth;
    double                  m_obsHeight;

};

/**************************************************************************
 *
 *       This section describes the VLIConfiguration class that encapsulates
 *       the hardware and software configuration.
 *
 ***************************************************************************/

class VLIEXPORT VLIConfiguration
{
public:
                            VLIConfiguration (void);        // Default constructor

    virtual                 ~VLIConfiguration (void);       // Destructor

    static int              GetBoardMajorVersion (void      // Return major version number of board 0
                            );

    // New for 3.1
    static int              GetBoardMajorVersion (        // Return major version number of board i
                                int inIthBoard              // board number (0 to n-1)
                            );

    static int              GetBoardMinorVersion (void      // Return minor version number of board 0
                            );

    // New for 3.1
    static int              GetBoardMinorVersion (        // Return minor version number of board i
                                int inIthBoard              // board number (0 to n-1)
                            );

    static int              GetNumberOfBoards (void         // Return the number of boards installed
                            );

    static VLIuint32        GetAvailableMemory (            // Return the available memory on a board
                                int inIthBoard              // board number (0 to n-1)
                            );

    static int              GetMaxCutPlanes (void           // Return the maximum number of cut planes (4)
                            );

    static int              GetMaxVolumeFields(void         // Return the maximum number of fields supported for a volume (16)
                            );

    static int              GetMaxTables (void              // Return the number of tables supported (4)
                            );

    static int              GetMaxChannels (void            // Return the maximum number of color channels supported after the lookup tables (4)
                            );

    static int              GetMinVoxelSize (void           // Return the minimum size (in bits) of a voxel (8)
                            );

    static int              GetMaxVoxelSize (void           // Return the maximum size (in bits) of a voxel (32)
                            );

    static int              GetMinFieldSize (void           // Return the minimum size (in bits) of a field (4)
                            );

    static int              GetMaxFieldSize (void           // Return the maximum size (in bits) of a field (16)
                            );

    static int              GetVLIMajorVersion(void         // Return library's major version number
                            );

    static int              GetVLIMinorVersion(void         // Return library's minor version number
                            );

    static int              GetGradientTableLength(void     // Return the number of entries in a gradient table (8 for Roadrunner)
                            );

    static void             GetMaxLockedSize (              // Return the maximum size of a volume that can be locked
                                unsigned int inVoxelSize,   // voxel size or format
                                unsigned int &outNx,        // RETURNED maximum size in X
                                unsigned int &outNy,        // RETURNED maximum size in Y
                                unsigned int &outNz         // RETURNED maximum size in Z
                            );

    static void             GetMaxMappedSize (              // Return the maximum size of a volume that can be mapped
                                unsigned int inVoxelSize,   // voxel size or format
                                unsigned int &outNx,        // RETURNED maximum size in X
                                unsigned int &outNy,        // RETURNED maximum size in Y
                                unsigned int &outNz         // RETURNED maximum size in Z
                            );

    static VLIStatus        GetMemoryUsage (                // Return current memory usage values
                                VLILocation inLocation,     // board number
                                VLIuint64 & outTotal,       // RETURNED total size of board memory, in bytes
                                VLIuint64 & outLocked,      // RETURNED number of bytes currently locked down
                                VLIuint64 & outUnlocked,    // RETURNED number of bytes currently loaded but not locked
                                VLIuint64 & outFree         // RETURNED number of bytes currently free.
                            );

    enum VersionType
    {
        kVLIVersion,
        kDriverWanted,
        kDriverFound
    };

    static const char       * GetVersionString (            // Return description of build
                                VersionType inVersion = kVLIVersion
                            );

};

/***************************************************************************
 *
 *       This section describes the VLICrop class/object.
 *
 ***************************************************************************/

class VLIEXPORT VLICrop
{
public:

    enum Flags
    {
        kDisable                = 0,
        kEnableX0               = 0x1,
        kEnableY0               = 0x2,
        kEnableZ0               = 0x4,
        kInvert0                = 0x8,

        kEnableX1               = 0x10,
        kEnableY1               = 0x20,
        kEnableZ1               = 0x40,
        kInvert1                = 0x80,

        kEnableX2               = 0x100,
        kEnableY2               = 0x200,
        kEnableZ2               = 0x400,
        kInvert2                = 0x800,

        kOrSelect               = 0x1000,
        kInvertOutput   = 0x2000,

        kSubVolume              = kEnableX0|kEnableY0|kEnableZ0,
        k3DCross                = kEnableX0|kEnableY0|kEnableY1|kEnableZ1|kEnableX2|kEnableZ2|kOrSelect,
        k3DCrossInvert  = k3DCross|kInvertOutput,
        k3DFence                = kEnableX0|kEnableY1|kEnableZ2|kOrSelect,
        k3DFenceInvert  = k3DFence|kInvertOutput
    };

                            VLICrop (void);                 // Default constructor: create a disabled crop object

                            VLICrop (                       // Constructor with complete data
                                double inMinX,              // minimum X value
                                double inMaxX,              // maximum X value
                                double inMinY,              // minimum Y value
                                double inMaxY,              // maximum Z value
                                double inMinZ,              // minimum Z value
                                double inMaxZ,              // maximum Z value
                                int inFlags = kSubVolume    // crop flags (see enum Flags)
                            );

                            ~VLICrop (void);                // Destructor

    void                    GetSlabs (                      // Return minimum and maximum values for all slabs
                                double& outMinX,            // RETURNED minumum X value
                                double& outMaxX,            // RETURNED maxumum X value
                                double& outMinY,            // RETURNED minumum Y value
                                double& outMaxY,            // RETURNED maxumum Y value
                                double& outMinZ,            // RETURNED minumum Z value
                                double& outMaxZ             // RETURNED maxumum Z value
                            ) const;


    double                  GetSlabsMinX (void              // Return minimum X value
                            ) const;

    double                  GetSlabsMaxX (void              // Return maximum X value
                            ) const;

    double                  GetSlabsMinY (void              // Return minimum Y value
                            ) const;

    double                  GetSlabsMaxY (void              // Return maximum Y value
                            ) const;

    double                  GetSlabsMinZ (void              // Return minimum Z value
                            ) const;

    double                  GetSlabsMaxZ (void              // Return maximum Z value
                            ) const;

    VLIStatus               SetSlabs (                      // Set minimum and maximum values for all slabs
                                double inMinX,              // minimum X value
                                double inMaxX,              // maximum X value
                                double inMinY,              // minimum Y value
                                double inMaxY,              // maximum Y value
                                double inMinZ,              // minimum Z value
                                double inMaxZ               // maximum Z value
                            );

    VLIStatus               SetXSlab (                      // Set minimum and maximum values for the X slab
                                double inMinX,              // minimum X value
                                double inMaxX               // maximum X value
                            );

    VLIStatus               SetYSlab (                      // Set minimum and maximum values for the Y slab
                                double inMinY,              // minimum Y value
                                double inMaxY               // maximum Y value
                            );

    VLIStatus               SetZSlab (                      // Set minimum and maximum values for the Z slab
                                double inMinZ,              // minimum Z value
                                double inMaxZ               // maximum Z value
                            );

    int                     GetFlags (void                  // Return crop flags
                            ) const;

    VLIStatus               SetFlags (                      // Set crop flags (see enum Flags)
                                int inFlags                 // new flags
                            );

private:
    double                  m_minX;                         // X position of minimum YZ crop plane
    double                  m_minY;                         // Y position of minimum XZ crop plane
    double                  m_minZ;                         // Z position of minimum XY crop plane

    double                  m_maxX;                         // X position of maximum YZ crop plane
    double                  m_maxY;                         // Y position of maximum XZ crop plane
    double                  m_maxZ;                         // Z position of maximum XY crop plane

    unsigned int            m_configuration;                // how planes are combined to form crop volume

};

/***************************************************************************
 *
 *       This section describes the VLICutPlane class/object.
 *
 ****************************************************************************/


class VLIEXPORT VLICutPlane : public VLIShareable
{

public:

    enum  Flags
    {
        kInside =  0,                   // Render inside the thick cut plane
        kOutside = 1                    // Render outside the thick cut plane
    };

    static VLICutPlane      *Create (                       // Create a cutplane object
                                double inA,                 // A value (world space)
                                double inB,                 // B value (world space)
                                double inC,                 // C value (world space)
                                double inD,                 // D value (world space)
                                double inThickness,         // thickness value (world space)
                                double inFallOff = 0.0,     // falloff value (voxels)
                                Flags inFlags = kInside     // flags
                            );

    virtual void            GetPlane (                      // Return plane parameters
                                double& outA,               // RETURNED A value (world space)
                                double& outB,               // RETURNED B value (world space)
                                double& outC,               // RETURNED C value (world space)
                                double& outD                // RETURNED D value (world space)
                            ) const = 0;

    virtual double          GetPlaneA (void                 // Return plane A value (world space)
                            ) const = 0;

    virtual double          GetPlaneB (void                 // Return plane B value (world space)
                            ) const = 0;

    virtual double          GetPlaneC (void                 // Return plane C value (world space)
                            ) const = 0;

    virtual double          GetPlaneD (void                 // Return plane D value (world space)
                            ) const = 0;

    virtual VLIStatus       SetPlane (                      // Set plane parameters
                                double inA,                 // A value (world space)
                                double inB,                 // B value (world space)
                                double inC,                 // C value (world space)
                                double inD                  // D value (world space)
                            ) = 0;


    virtual double          GetThickness (void              // Return plane thickness (world space)
                            ) const = 0;

    virtual VLIStatus       SetThickness (                  // Set plane thickness (world space)
                                double inThickness          // thickness (world space)
                            ) = 0;


    // Falloff region in which opacity ramps down to zero
    virtual double          GetFallOff (void                // Return falloff (voxels)
                            ) const = 0;

    virtual VLIStatus       SetFallOff (                    // Set falloff (voxels)
                                double inFallOff            // falloff (voxels)
                            ) = 0;


    // Get and set flags
    virtual Flags           GetFlags (void                  // Return inside/outside flag
                            ) const = 0;

    virtual VLIStatus       SetFlags (                      // Set inside/outside flag
                                Flags inFlags               // kInside or kOutside
                            ) = 0;

    virtual VLIStatus Reserved00(void);
    virtual VLIStatus Reserved01(void);
    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);

protected:
                            VLICutPlane (void) {}
    virtual                 ~VLICutPlane (void) {}


};

/****************************************************************************
 *
 *       This section describes the VLIGraphicsContext abstract class/object.
 *
 ****************************************************************************/

// VLIGraphicsContext is deprecated and should not be used for new programs

class VLIContext;

#if ! VLIAllowDeprecated
class VLIGraphicsContextDeprecated { };
#else   // VLIAllowDeprecated
class VLIEXPORT VLIGraphicsContext              // DEPRECATED
{
public:
    enum InfoType
    {
        kView     = 1,
        kLight    = 2,
        kCutPlane = 4,
        kMaterial = 8,
        kAll      = (kView|kLight|kCutPlane|kMaterial)
    };
    virtual int             ShouldLeaveOnBoard (void        // DEPRECATED
                            ) = 0;

    virtual int             TransferBasePlane (             // DEPRECATED
                                int inBuffer,
                                unsigned int inNx,
                                unsigned int inNy,
                                unsigned int inIX,
                                unsigned int inIY,
                                VLIPixel *inBasePlane
                            ) = 0;

    virtual int             ImportAttributesToVLIContext (  // DEPRECATED
                                VLIContext& inContext,
                                    InfoType inFlags
                                ) = 0;


    virtual int             GetTextureMemoryAddress (       // DEPRECATED
                                int inBuffer,
                                unsigned int inNX,
                                unsigned int inNY,
                                unsigned int & outStride,
                                unsigned int & outFormat,
                                void *& outAddress,
                                unsigned int outBusAddress[2]
                            ) = 0;

    virtual int             RenderHexagon (                 // DEPRECATED
                                int inBuffer,
                                VLIVector3D inGeomHex[6],
                                VLIVector2D inTexHex[6]
                            ) = 0;


    virtual VLIPixelFormat  GetPixelFormat (void            // DEPRECATED
                            ) const;

    virtual int             SetPixelFormat (                // DEPRECATED
                                VLIPixelFormat inFormat
                            );

protected:                  // Member functions

                            VLIGraphicsContext (void);      // DEPRECATED

protected:                  // Data

    VLIPixelFormat          m_pixelFormat;

};
#endif  // VLIAllowDeprecated

/****************************************************************************
 *
 *       This section describes the VLILight class/object.
 *
 ****************************************************************************/

class VLIEXPORT VLILight : public VLIShareable
{
public:

    static VLILight*        CreateDirectional (void         // Create directional VLILight object
                                            );
    static VLILight*        CreateDirectional (             // Create directional VLILight object
                                const VLIVector3D& inDirection, // direction vector
                                double inIntensity = 1.0    // intensity of light (0.0 to 1.0)
                            );

    // Accessor functions for direction, and intensity.
    virtual const VLIVector3D&      GetDirection  (void     // Return the direction vector
                            ) const = 0;

    virtual double          GetIntensity  (void             // Return the light intensity
                            ) const = 0;

    virtual VLIStatus       SetDirection  (                 // Set the direction vector
                                const VLIVector3D &inDirection // new direction vector
                            ) = 0;

    virtual VLIStatus       SetIntensity  (                 // Set the light intensity
                                double inIntensity          // new light intensity
                            ) = 0;
    virtual VLIStatus Reserved00(void);
    virtual VLIStatus Reserved01(void);
    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);

protected:
                            VLILight (void) {}
    virtual                 ~VLILight (void) {}

};


/***************************************************************************
 *
 *       This section describes the VLICursor class/object that manages
 *       the hardware 3D cursor
 *
 ****************************************************************************/

// The Volume Pro 500 hardware cursor is not supported on the Volume Pro 1000
// If the application writer defines VLIDummyCursor before including
// this header file, the class is defined, but no cursor will actually show
// up in the volume rendering.

#if VLIDummyCursor

class VLIEXPORT VLICursor   // NOT SUPPORTED
{
public:

    enum Types
    {
        kCrossHair          = 0x00000000,
        kPlane              = 0x00000010
    };

    enum Flags
    {
        kDisable            = 0x00000000,
        kEnableX            = 0x00000004,
        kEnableY            = 0x00000002,
        kEnableZ            = 0x00000001,
        kEnableAll          = (kEnableX|kEnableY|kEnableZ),

        kExcludeOverlap     = 0x00000080,
        kDisableCrop        = 0x00040000
    };

    enum Axes
    {
        kXAxis              = 4,
        kYAxis              = 2,
        kZAxis              = 1
    };

    // The default (and only) constructor creates an enabled cross-hair cursor
    // and with blend mode. The cursor is located at origin of the volume.
                            VLICursor (void);               // Default constructor

    virtual                 ~VLICursor (void);              // Destructor

    VLIStatus               SetType (                   // Set the type of cursor
                                int inType              // kCrossHair or kPlane
                            );

    int                     GetType (void               // Return cursor type (kCrossHair or kPlane)
                            ) const;

    VLIStatus               SetWidth (                  // Set cursor width (thickness) in voxels
                                double inWidth          // width (1.0, 3.0, 5.0 or 7.0)
                            );

    double                  GetWidth (void              // Return cursor width (thickness) in voxels (1.0, 3.0, 5.0 or 7.0)
                            );

    VLIStatus               SetLength (                 // Set cursor length (voxels)
                                double inLength         // length (voxels); 0 means infinite length
                            );

    double                  GetLength (void             // Return cursor length (voxels)
                            );

    VLIStatus               SetPosition (               // Set cursor location (object space)
                                double inX,             // X coordinate (object space)
                                double inY,             // Y coordinate (object space)
                                double inZ              // Z coordinate (object space)
                            );

    void                    GetPosition (               // Return cursor location (object space)
                                double& outX,           // RETURNED X coordinate (object space)
                                double& outY,           // RETURNED Y coordinate (object space)
                                double& outZ            // RETURNED Z coordinate (object space)
                            );

    VLIStatus               SetAttributes (             // Set cursor attributes
                                int inModes             // attributes: bitwise OR of Flags
                            );

    int                     GetAttributes(void          // Return attributes: bitwise OR of Flags
                            ) const;

    VLIStatus               SetColor (                  // Set color(s) for cursor
                                int inAxes,             // axis or axes: bitwise OR of Axes enumerants
                                double inRed,           // red component (0.0 to 1.0)
                                double inGreen,         // green component (0.0 to 1.0)
                                double inBlue           // blue component (0.0 to 1.0)
                            );

    VLIStatus               GetColor (                  // Return color for one axis of cursor
                                int inAxis,             // axis: one of the Axes enumerants
                                double& outRed,         // RETURNED red component (0.0 to 1.0)
                                double& outGreen,       // RETURNED green component (0.0 to 1.0)
                                double& outBlue         // RETURNED blue component (0.0 to 1.0)
                            );

    VLIStatus               SetOpacity (                // Set opacity(ies) for cursor
                                int inAxes,             // axis or axes: bitwise OR of Axes enumerants
                                double inAlpha          // opacity (0.0 to 1.0)
                            );

    double                  GetOpacity (                // Return opacity for one axis of cursor (0.0 to 1.0)
                                int inAxis              // axis: one of the Axes enumerants
                            );

    VLIStatus               SetColorAndOpacity (        // Set color and opacity for one or more axes of cursor
                                int inAxes,             // axis or axes: bitwise OR of Axes enumerants
                                VLIRGBAPacked inRGBA    // color and opacity, packed format (0 to 255)
                            );

    VLIRGBAPacked           GetColorAndOpacity (        // Return color and opacity for one axis of cursor (packed)
                                int inAxis              // axis: one of the Axes enumerants
                            );

private:
    double                  m_Position[3];
    double                  m_width;
    double                  m_length;
    unsigned int            m_configuration;

    double                  m_Red  [3];
    double                  m_Green[3];
    double                  m_Blue [3];
    double                  m_Alpha[3];

};

#else // VLIDummyCursor is not set

// Declare a cursor class with nothing in it.
class VLICursorDeprecated
{
};

#endif

/******************************************************************************
 *
 *  Name and purpose:
 *      VLIVolumeRange -- maintain a range of voxel addresses for a volume
 *
 *****************************************************************************/

class VLIEXPORT VLIVolumeRange
{
public:
                            VLIVolumeRange (void        // Construct a null volume range
                            );

                            VLIVolumeRange (            // Construct a volume range from paramaters
                                int inSizeX,            // size in X dimension
                                int inSizeY,            // size in Y dimension
                                int inSizeZ,            // size in Z dimension
                                int inOriginX = 0,      // origin in X dimension
                                int inOriginY = 0,      // origin in Y dimension
                                int inOriginZ = 0       // origin in Z dimension
                            );

    inline int &            XOrigin ( void              // Return reference to origin in X dimension
                            )  { return m_origin[0]; }

    inline int &            YOrigin ( void              // Return reference to origin in Y dimension
                            )  { return m_origin[1]; }

    inline int &            ZOrigin ( void              // Return reference to origin in Z dimension
                            )  { return m_origin[2]; }

    inline int &            XSize ( void                // Return reference to size in X dimension
                            )   { return m_size  [0]; }

    inline int &            YSize ( void                // Return reference to size in Y dimension
                            )   { int *i = &(m_size  [1]); return *i ; }

    inline int &            ZSize ( void                // Return reference to size in Z dimension
                            )   { int *i = &(m_size  [2]); return *i; }

    inline int              XOrigin ( void              // Return origin in X dimension
                            ) const { return m_origin[0]; }

    inline int              YOrigin ( void              // Return origin in Y dimension
                            ) const { return m_origin[1]; }

    inline int              ZOrigin ( void              // Return origin in Z dimension
                            ) const { return m_origin[2]; }

    inline int              XSize ( void                // Return origin in X dimension
                            ) const { return m_size  [0]; }

    inline int              YSize ( void                // Return origin in Y dimension
                            ) const { return m_size  [1]; }

    inline int              ZSize ( void                // Return origin in Z dimension
                            ) const { return m_size  [2]; }

    void                    Reset ( void                // Set size and origin to zero
                            );

    VLIbool                 IsReset ( void              // Return VLItrue if sizes are all zero
                            ) const;

    VLIbool                 IsEmpty ( void              // Return VLItrue if one or more sizes are zero
                            ) const;

    VLIVolumeRange &        Union (                     // Compute union of this and inOther in this; return this
                                const VLIVolumeRange &inOther
                            );

    VLIVolumeRange &        Intersect (                 // Compute intersection of this and inOther in this; return this
                                const VLIVolumeRange & inOther
                            );

    VLIbool                 Contains (                  // Return VLItrue if this contains all of inOther
                                const VLIVolumeRange & inOther
                            ) const;

    VLIbool                 operator == (               // Return VLItrue if this objects size and origin match inOther
                                const VLIVolumeRange & inOther
                            ) const;

    VLIbool                 operator != (               // Return VLItrue if this objects size and origin do not match inOther
                                const VLIVolumeRange & inOther
                            ) const;

    // Alternative constructors
    static VLIVolumeRange   Union (                     // Return union of two input ranges
                                const VLIVolumeRange & inA,
                                const VLIVolumeRange & inB
                            );

    static VLIVolumeRange   Intersect (                 // Return intersection of two input ranges
                                const VLIVolumeRange & inA,
                                const VLIVolumeRange & inB
                            );

private:
    int                     m_origin[3];
    int                     m_size[3];

};

class VLIEXPORT VLIImageRange
{
public:
                            VLIImageRange (void         // Construct a null image range
                            );

                            VLIImageRange (             // Construct an image range from paramaters
                                int inSizeX,            // size in X dimension
                                int inSizeY,            // size in Y dimension
                                int inOriginX = 0,      // origin in X dimension
                                int inOriginY = 0       // origin in Y dimension
                            );

    inline int &            XOrigin ( void              // Return reference to origin in X dimension
                            )  { return m_origin[0]; }

    inline int &            YOrigin ( void              // Return reference to origin in Y dimension
                            )  { return m_origin[1]; }

    inline int &            XSize ( void                // Return reference to size in X dimension
                            )   { return m_size  [0]; }

    inline int &            YSize ( void                // Return reference to size in Y dimension
                            )   { return m_size  [1]; }

    inline int              XOrigin ( void              // Return origin in X dimension
                            ) const { return m_origin[0]; }

    inline int              YOrigin ( void              // Return origin in Y dimension
                            ) const { return m_origin[1]; }

    inline int              XSize ( void                // Return origin in X dimension
                            ) const { return m_size  [0]; }

    inline int              YSize ( void                // Return origin in Y dimension
                            ) const { return m_size  [1]; }

    void                    Reset ( void                // Set size and origin to zero
                            );

    VLIbool                 IsReset ( void              // Return VLItrue if all sizes are zero
                            ) const;

    VLIbool                 IsEmpty ( void              // Return VLItrue if any sizes are zero
                            ) const;

    VLIbool                 operator == (               // Return VLItrue if this objects size and origin match inOther
                                const VLIImageRange & inOther
                            ) const;

    VLIbool                 operator != (               // Return VLItrue if this objects size and origin do not match inOther
                                const VLIImageRange & inOther
                            ) const;

private:
    int                     m_origin[2];
    int                     m_size[2];

};


/******************************************************************************
 *
 *  Name and purpose:
 *      VLIImageBuffer - Manage the state of hardware image buffers
 *
 *****************************************************************************/

class VLIEXPORT VLIImageBuffer : public VLIShareable
{
public:

    // Object factory

    static VLIImageBuffer*  Create (                            // Create a VLIImageBuffer object
                                VLILocation inLocation,         // location -- cannot be kVLIAnyBoard
                                unsigned int inWidth,           // width in pixels
                                unsigned int inHeight,          // height in pixels
                                unsigned int inPixelSize,       // size of pixel in bits (16, 32, or 64)
                                int inNumberOfFields = 0,       // number of fields in field array
                                const VLIFieldDescriptor inFieldArray[] = 0 // pointer to field array
                            );

    // Location and size access

    virtual VLILocation     GetLocation (void                   // Return location of buffer
                            ) const = 0;

    virtual void            GetSize (                           // Return the size of the image in pixels
                                unsigned int& outWidth,         // RETURNED width in pixels
                                unsigned int& outHeight         // RETURNED height in pixels
                            ) const = 0;

    virtual unsigned int    GetWidth (void                      // Return the width of the image in pixels
                            ) const = 0;

    virtual unsigned int    GetHeight (void                     // Return the height of the image in pixels
                            ) const = 0;

    virtual unsigned int    GetPixelSize (void                  // Return the size of a pixel in bits
                            ) const = 0;

    // Field descriptor access

    virtual VLIFieldDescriptor  GetFieldDescriptor (            // Return a field descriptor
                                VLIChannel inFieldNumber        // which field to return
                            ) const = 0;

    virtual VLIStatus       SetFieldDescriptor (                // Set a field descriptor
                                VLIChannel inFieldNumber,       // which field to set
                                const VLIFieldDescriptor &inDescriptor // field descriptor
                            ) = 0;

    // Updating the data in the image

    virtual VLIStatus       Update (                            // Update a color image with new values
                                const void * inData,            // location of input image data
                                const VLIImageRange & inRange,  // range of image to update
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) = 0;

    virtual VLIStatus       UpdateField (                       // Update one color image field with new values
                                const void * inData,            // location of input image data
                                const VLIFieldDescriptor & inDescriptor, // description of output
                                const VLIImageRange & inRange,  // range of image to update
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) = 0;

    virtual VLISyncEvent    StartUpdate (                       // ASYNCHRONOUS Start to update a color image with new values
                                const void * inData,            // location of input image data
                                const VLIImageRange & inRange,  // range of image to update
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) = 0;

    virtual VLISyncEvent    StartUpdateField (                  // ASYNCHRONOUS Start to update a color image field with new values
                                const void * inData,            // location of input image data
                                const VLIFieldDescriptor & inDescriptor, // description of output
                                const VLIImageRange & inRange,  // range of image to update
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) = 0;

    virtual VLIStatus       Clear (                             // Clear a range of this buffer
                                const VLIImageRange & inRange,  // range to clear
                                VLIuint64 inValue               // clear value
                            ) = 0;

    // Reading the data in the image

    virtual VLIStatus       Unload (                            // Unload a color image to application storage
                                void * outData,                 // RETURNED pixel data
                                const VLIImageRange & inRange,  // range of image to return
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) const = 0;

    virtual VLIStatus       UnloadField (                       // Unload a color image field to application storage
                                void * outData,                 // RETURNED image data
                                const VLIFieldDescriptor & inDescriptor, // description of output
                                const VLIImageRange & inRange,  // range of image to return
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) const = 0;

    virtual VLISyncEvent    StartUnload (                       // ASYNCHRONOUS Start to unload a color image to application storage
                                void * outData,                 // RETURNED pixel data
                                const VLIImageRange & inRange,  // range of image to return
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) const = 0;

    virtual VLISyncEvent    StartUnloadField (                  // ASYNCHRONOUS Start to unload a color image field to application storage
                                void * outData,                 // RETURNED image data
                                const VLIFieldDescriptor & inDescriptor, // description of output
                                const VLIImageRange & inRange,  // range of image to return
                                unsigned int inWidth = 0        // width of application buffer (0 means use range)
                            ) const = 0;

    // Accessing the data in the image

    virtual VLIStatus       Map (                               // Map a 16-bit image into application memory space
                                VLIuint16 *& outData,           // RETURNED pointer to image data
                                const VLIImageRange & inRange,  // range of image to map in
                                unsigned int & outWidth         // RETURNED width of application buffer
                            ) = 0;

    virtual VLIStatus       Map (                               // Map a 32-bit image into application memory space
                                VLIuint32 *& outData,           // RETURNED pointer to image data
                                const VLIImageRange & inRange,  // range of image to map in
                                unsigned int & outWidth         // RETURNED width of application buffer
                            ) = 0;

    virtual VLIStatus       Map (                               // Map a 64-bit image into application memory space
                                VLIuint64 *& outData,           // RETURNED pointer to image data
                                const VLIImageRange & inRange,  // range of image to map in
                                unsigned int & outWidth         // RETURNED width of application buffer
                            ) = 0;

    virtual VLIStatus       Map (                               // Map an image into application memory space
                                void *& outData,                // RETURNED pointer to image data
                                const VLIImageRange & inRange,  // range of image to map in
                                unsigned int & outWidth         // RETURNED width of application buffer
                            ) = 0;

    virtual VLIStatus       Unmap (                             // Release an image buffer mapping
                                void * inData                   // mapped image pointer to be released
                            ) = 0;

    // Addressing modes

    virtual VLIStatus       SetOffset (                         // Set addressing mode to kVLIOffsetAddress and set the offset values
                                int inOffsetX,                  // Offset for X (must be even)
                                int inOffsetY                   // Offset for Y (must be even)
                            ) = 0;

    virtual VLIStatus       SetWrap (                           // Set addressing mode to kVLIWrapAddress and set the modulus values
                                int inWrapX,                    // Wrap modulus for X (must be a power of 2)
                                int inWrapY                     // Wrap modulus for Y (must be a power of 2)
                            ) = 0;

    virtual VLIStatus       SetAddressParameters (              // Set the addressing parameters
                                VLIAddressMode inMode,          // kVLIOffsetAddress or kVLIWrapAddress
                                int inAddressParameterX,        // Offset or wrap modulus for X
                                int inAddressParameterY         // Offset or wrap modulus for Y
                            ) = 0;

    virtual void            GetAddressParameters (              // Return the addressing parameters
                                VLIAddressMode & outMode,       // RETURNED kVLIOffsetAddress or kVLIWrapAddress
                                int& outAddressParameterX,      // RETURNED Offset or wrap modulus for X
                                int& outAddressParameterY       // RETURNED Offset or wrap modulus for Y
                            ) const = 0;

    virtual int             GetAddressParameterX (void) const = 0;  // Return buffer offset or wrap X value

    virtual int             GetAddressParameterY (void) const = 0;  // Return buffer offset or wrap Y value

    virtual VLIAddressMode  GetAddressMode (void) const = 0;    // Return kVLIOffsetAddress or kVLIWrapAddress

    // Ranges of interest, input and output

    virtual VLIImageRange   GetInputLimits (void                // Return the input limits (initial range)
                            ) const = 0;

    virtual VLIStatus       SetInputLimits (                    // Set the input limits (initial range)
                                const VLIImageRange & inLimits  // input limits
                            ) = 0;

    virtual VLIImageRange   GetOutputLimits (void               // Return the output limits (view frustum?)
                            ) const = 0;

    virtual VLIStatus       SetOutputLimits (                   // Set the output limits (view frustum?)
                                const VLIImageRange & inLimits  // new output limits
                            ) = 0;

    // The border value is used outside the input limits

    virtual void            GetBorderValue (                    // Return the border value (double)
                                double & outRed,                // red component, 0.0 to 1.0
                                double & outGreen,              // green component, 0.0 to 1.0
                                double & outBlue,               // blue component, 0.0 to 1.0
                                double & outAlpha               // alpha component, 0.0 to 1.0
                            ) const = 0;

    virtual double          GetBorderRed (void) const = 0;      // Return the red component of the border value

    virtual double          GetBorderGreen (void) const = 0;    // Return the Green component of the border value

    virtual double          GetBorderBlue (void) const = 0;     // Return the Blue component of the border value

    virtual double          GetBorderAlpha (void) const = 0;    // Return the Alpha component of the border value

    virtual VLIStatus       SetBorderValue (                    // Set the border value (double)
                                double inRed,                   // red component, 0.0 to 1.0
                                double inGreen,                 // green component, 0.0 to 1.0
                                double inBlue,                  // blue component, 0.0 to 1.0
                                double inAlpha                  // alpha component, 0.0 to 1.0
                            ) = 0;

    // New API for 3.1

    virtual VLIStatus       Resample (                                  // Resample the image to a different size
                                VLIImageBuffer * outDestination,            // destination image buffer
                                const VLIImageRange & inDestinationRange,   // detination range
                                const VLIImageRange & inSourceRange         // source range
                            );

    virtual VLISyncEvent    StartResample (                             // ASYNCHRONOUS Start resampling the image
                                VLIImageBuffer * outDestination,            // destination image buffer 
                                const VLIImageRange & inDestinationRange,   // destination range
                                const VLIImageRange & inSourceRange         // source range
                            );

    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);

protected:
                            VLIImageBuffer (void) {}
    virtual                 ~VLIImageBuffer (void) {}
};

/******************************************************************************
 *
 *  Name and purpose:
 *      VLIDepthBuffer - Manage the state of hardware depth buffers
 *
 *****************************************************************************/

class VLIEXPORT VLIDepthBuffer : public VLIShareable
{
public:

    enum DepthFormat
    {
        kDepth24Low,        // 0x00000000 to 0x00ffffff (1.0)
        kDepth24High,       // 0x00000000 to 0xffffff00 (1.0) NOT SUPPORTED
        kDepthFloat         // NOT SUPPORTED
    };

    // Object factory

    static VLIDepthBuffer*  Create (                            // Create a VLIDepthBuffer object
                                VLILocation inLocation,         // location -- cannot be kVLIAnyBoard
                                unsigned int inWidth,           // width in depth elements (32 bits each)
                                unsigned int inHeight           // height in depth elements
                            );

    // Location and size access

    virtual VLILocation     GetLocation (void                   // Return location of buffer
                            ) const = 0;

    virtual void            GetSize (                           // Return the size of the depth buffer
                                unsigned int& outWidth,         // RETURNED width in depth elements
                                unsigned int& outHeight         // RETURNED height in depth elements
                            ) const = 0;

    virtual unsigned int    GetWidth (void                      // Return the width of the depth buffer
                            ) const = 0;

    virtual unsigned int    GetHeight (void                     // Return the height of the depth buffer
                            ) const = 0;

    // Updating the data in the depth buffer

    virtual VLIStatus       Update (                            // Update a depth buffer with new values
                                const void * inData,            // location of input data
                                const VLIImageRange & inRange,  // range of depth buffer to update
                                unsigned int inWidth = 0,       // width of application buffer (0 means use range)
                                DepthFormat inFormat = kDepth24Low  // format of this data
                            ) = 0;

    virtual VLISyncEvent    StartUpdate (                       // ASYNCHRONOUS Start to update a depth buffer with new values
                                const void * inData,            // location of input data
                                const VLIImageRange & inRange,  // range of depth buffer to update
                                unsigned int inWidth = 0,       // width of application buffer (0 means use range)
                                DepthFormat inFormat = kDepth24Low  // format of this data
                            ) = 0;

    virtual VLIStatus       Clear (                             // Clear a range of this buffer
                                const VLIImageRange & inRange,  // range to clear
                                VLIuint32 inValue               // clear value
                            ) = 0;

    // Reading the data in the depth buffer

    virtual VLIStatus       Unload (                            // Unload a depth buffer to application storage
                                void * outData,                 // RETURNED depth data
                                const VLIImageRange & inRange,  // range of depth buffer to return
                                unsigned int inWidth = 0,       // width of application buffer (0 means use range)
                                DepthFormat inFormat = kDepth24Low  // format to be used for this data
                            ) const = 0;

    virtual VLISyncEvent    StartUnload (                       // ASYNCHRONOUS Start to unload a depth buffer to application storage
                                void * outData,                 // RETURNED depth data
                                const VLIImageRange & inRange,  // range of depth buffer to return
                                unsigned int inWidth = 0,       // width of application buffer (0 means use range)
                                DepthFormat inFormat = kDepth24Low  // format to be used for this data
                            ) const = 0;

    // Accessing the data in the image

    virtual VLIStatus       Map (                               // Map a 32-bit depth buffer into application memory space
                                VLIuint32 *& outData,           // RETURNED pointer to depth buffer data
                                const VLIImageRange & inRange,  // range of depth buffer to map in
                                unsigned int & outWidth,        // RETURNED width of application buffer
                                DepthFormat inFormat = kDepth24Low  // format to be used for this data
                            ) = 0;

    virtual VLIStatus       Map (                               // Map a depth buffer into application memory space
                                void *& outData,                // RETURNED pointer to depth buffer data
                                const VLIImageRange & inRange,  // range of depth buffer to map in
                                unsigned int & outWidth,        // RETURNED width of application buffer
                                DepthFormat inFormat = kDepth24Low  // format to be used for this data
                            ) = 0;

    virtual VLIStatus       Unmap (                             // Release a depth buffer mapping
                                void * inData                   // mapped depth pointer to be released
                            ) = 0;

    // Addressing modes

    virtual VLIStatus       SetOffset (                         // Set addressing mode to kVLIOffsetAddress and set the offset values
                                int inOffsetX,                  // Offset for X (must be even)
                                int inOffsetY                   // Offset for Y (must be even)
                            ) = 0;

    virtual VLIStatus       SetWrap (                           // Set addressing mode to kVLIWrapAddress and set the modulus values
                                int inWrapX,                    // Wrap modulus for X (must be a power of 2)
                                int inWrapY                     // Wrap modulus for Y (must be a power of 2)
                            ) = 0;

    virtual VLIStatus       SetAddressParameters (              // Set the addressing parameters
                                VLIAddressMode inMode,          // kVLIOffsetAddress or kVLIWrapAddress
                                int inAddressParameterX,        // Offset or wrap modulus for X
                                int inAddressParameterY         // Offset or wrap modulus for Y
                            ) = 0;

    virtual void            GetAddressParameters (              // Return the addressing parameters
                                VLIAddressMode &outMode,        // RETURNED kVLIOffsetAddress or kVLIWrapAddress
                                int& outAddressParameterX,      // RETURNED Offset or wrap modulus for X
                                int& outAddressParameterY       // RETURNED Offset or wrap modulus for Y
                            ) const = 0;

    virtual int             GetAddressParameterX (void) const = 0;  // Return buffer offset or wrap X value

    virtual int             GetAddressParameterY (void) const = 0;  // Return buffer offset or wrap Y value

    virtual VLIAddressMode  GetAddressMode (void) const = 0;    // Return kVLIOffsetAddress or kVLIWrapAddress

    // Ranges of interest, input and output

    virtual VLIImageRange   GetInputLimits (void                // Return the input limits (initial range)
                            ) const = 0;

    virtual VLIStatus       SetInputLimits (                    // Set the input limits (initial range)
                                const VLIImageRange & inLimits  // input limits
                            ) = 0;

    virtual VLIImageRange   GetOutputLimits (void               // Return the output limits (view frustum?)
                            ) const = 0;

    virtual VLIStatus       SetOutputLimits (                   // Set the output limits (view frustum?)
                                const VLIImageRange & inLimits  // output limits
                            ) = 0;

    // The border value is used outside the input limits

    virtual VLIuint32       GetBorderValue (void                // Return the border value (VLIuint32)
                            ) const = 0;

    virtual VLIStatus       SetBorderValue (                    // Set the border value (VLIuint32)
                                VLIuint32 inBorderValue         // border value (kVLIDepth24Low)
                            ) = 0;

    virtual VLIStatus Reserved00(void);
    virtual VLIStatus Reserved01(void);
    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);

protected:
                            VLIDepthBuffer (void) {}
    virtual                 ~VLIDepthBuffer (void) {}
};


/****************************************************************************
*
*        This section describes the VLILookupTable class/object that manages
*        both color and opacity lookups.
*
****************************************************************************/

class VLILookupTable;

struct VLIRGBColor
{
public:
            VLIuint8 m_red, m_green, m_blue;
private:
            VLIuint8 m_pad;
};

struct VLIRGBAFloat
{
public:
    VLIRGBAFloat (float inRed, float inGreen, float inBlue, float inAlpha)
        : m_red (inRed), m_green (inGreen), m_blue (inBlue), m_alpha (inAlpha) {}

    VLIRGBAFloat (void)
        : m_red (0), m_green (0), m_blue (0), m_alpha (0) {}

    float m_red, m_green, m_blue, m_alpha;
};

class VLIEXPORT VLILookupTable : public VLIShareable
{
public:

    enum Size
    {
        kSize4096           = 4096,
#if VLIAllowDeprecated
        kSize2048           = 2048,         // DEPRECATED
        kSize1024           = 1024,         // DEPRECATED
        kSize512            = 512,          // DEPRECATED
#endif
        kSize256            = 256,
        kSize0              = 0
    };

    // Object factories

    // Creates a gray ramp for color lookup table and
    // a linear ramp for alpha table
    static VLILookupTable * Create (                            // Create VLILookupTable with linear ramps
                                Size inTableSize = kSize4096    // table size desired
                            );

    static VLILookupTable * Create (                            // Create VLILookupTable with given data
                                Size inTableSize,               // table size desired
                                const VLIuint8 inColor[][3],    // color data
                                const VLIuint16* inAlphas       // alpha data
                            );

    static VLILookupTable * Create (                            // Create VLILookupTable with given data
                                Size inTableSize,               // table size desired
                                const VLIRGBAPacked inEntries[] // color and alpha data
                            );


    static VLILookupTable * Create (                            // Create VLILookupTable with given data
                                Size inTableSize,               // table size desired
                                const VLIRGBAFloat inEntries[]  // color and alpha data
                            );

    // Size query

    virtual Size            GetSize (void                       // Return number of entries in table
                            ) const = 0;

    // Field descriptor access

    virtual VLIFieldDescriptor  GetFieldDescriptor (            // Return a field descriptor
                                VLIChannel inFieldNumber        // which field to return
                            ) const = 0;

    virtual VLIStatus       SetFieldDescriptor (                // Set a field descriptor
                                VLIChannel inFieldNumber,       // which field to set
                                const VLIFieldDescriptor &inDescriptor  // field descriptor
                            ) = 0;

    // Routines to change one or more color entries

    virtual VLIStatus       SetColorEntry (                     // Set color for one entry (int)
                                unsigned int inIndex,           // index (0 based)
                                unsigned int inRed,             // red value, 0 to 255
                                unsigned int inGreen,           // green value, 0 to 255
                                unsigned int inBlue             // blue  value, 0 to 255
                            ) = 0;

    virtual VLIStatus       SetColorEntry (                     // Set color for one entry (double)
                                unsigned int inIndex,           // index (0 based)
                                double inRed,                   // red value, 0.0 to 1.0
                                double inGreen,                 // green value, 0.0 to 1.0
                                double inBlue                   // blue value, 0.0 to 1.0
                            ) = 0;

    virtual VLIStatus       SetColorEntries (                   // Set color for multiple entries (VLIuint8)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const VLIuint8 inRGB[][3]       // color data
                            ) = 0;

    virtual VLIStatus       SetColorEntries (                   // Set color for multiple entries (VLIRGBColor)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const VLIRGBColor* inEntries    // color data
                            ) = 0;

    // Routines to retrieve color entries

    virtual VLIStatus       GetColorEntries (                   // Return color for multiple entries (int)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                VLIuint8 outRGB[][3]            // RETURNED color data (0 to 255)
                            ) const = 0;

    // Red, green and blue in the range 0.0 to 1.0
    virtual VLIStatus       GetColorEntries (                   // Return color for multiple entries (VLIRGBColor)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                VLIRGBColor* outEntries         // RETURNED color data (0.0 to 1.0)
                            ) const = 0;

    // Routines to change one or more alpha entries

    // Alpha in the range 0 to 4095.
    virtual VLIStatus       SetAlphaEntry (                     // Set alpha for one entry (int)
                                unsigned int inIndex,           // index (0 based)
                                unsigned int inAlpha            // alpha data (0 to 4095)
                            ) = 0;

    // Alpha in the range 0.0 to 1.0.
    virtual VLIStatus       SetAlphaEntry (                     // Set alpha for one entry (double)
                                unsigned int inIndex,           // index (0 based)
                                double inAlpha                  // alpha data (0.0 to 1.0)
                            ) = 0;

    // Alpha in the range 0 to 4095.
    virtual VLIStatus       SetAlphaEntries (                   // Set alpha for multiple entries (VLIuint16)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const VLIuint16* inAlpha        // alpha data (0 to 4095)
                            ) = 0;

    // Alpha in the range 0.0 to 1.0.
    virtual VLIStatus       SetAlphaEntries (                   // Set alpha for multiple entries (float)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const float * inAlpha           // alpha data (0.0 to 1.0)
                            ) = 0;

    // Routines to retrieve alpha entries

    // Alpha in the range 0 to 4095.
    virtual VLIStatus       GetAlphaEntries (                   // Return alpha for multiple entries (VLIuint16)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                VLIuint16* outAlpha             // RETURNED alpha data (0 to 4095)
                            ) const = 0;

    // Alpha in the range 0.0 to 1.0.
    virtual VLIStatus       GetAlphaEntries (                   // Return alpha for multiple entries (float)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                float * outAlpha                // RETURNED alpha data (0.0 to 1.0)
                            ) const = 0;

    // Routines to change both color and alpha of one or more entries

    // Red, green and blue in the range 0 to 255, alpha in the range 0 to 4095.
    virtual VLIStatus       SetRGBAEntry (                      // Set color and alpha data for one entry (int)
                                unsigned int inIndex,           // index (0 based)
                                unsigned int inRed,             // red value, 0 to 255
                                unsigned int inGreen,           // green value, 0 to 255
                                unsigned int inBlue,            // blue value, 0 to 255
                                unsigned int inAlpha            // alpha value, 0 to 4095
                            ) = 0;

    // Red, green, blue and alpha in the range 0.0 to 1.0.
    virtual VLIStatus       SetRGBAEntry (                      // Set color and alpha data for one entry (double)
                                unsigned int inIndex,           // index (0 based)
                                double inRed,                   // red value, 0.0 to 1.0
                                double inGreen,                 // green value, 0.0 to 1.0
                                double inBlue,                  // blue value, 0.0 to 1.0
                                double inAlpha                  // alpha value, 0.0 to 1.0
                            ) = 0;

    virtual VLIStatus       SetRGBAEntry (                      // Set color and alpha data for one entry (VLIRGBAPacked)
                                unsigned int inIndex,           // index (0 based)
                                VLIRGBAPacked inPacked          // color and alpha data in packed form (0 to 255)
                            ) = 0;

    virtual VLIStatus       SetRGBAEntries (                    // Set color and alpha for multiple entries (VLIRGBAPacked)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const VLIRGBAPacked* inRGBA     // color and alpha data in packed form (0 to 255)
                            ) = 0;

    virtual VLIStatus       SetRGBAEntries (                    // Set color and alpha for multiple entries (VLIRGAFloat)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to set
                                const VLIRGBAFloat* inRGBA      // color and alpha data, 0.0 to 1.0
                            ) = 0;

    // Routines to retrieve color and alpha entries

    virtual VLIStatus       GetRGBAEntries (                    // Return color and alpha for multiple entries (VLIRGBAPacked)
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                VLIRGBAPacked* outRGBA          // RETURNED color and alpha data in packed form (0 to 255)
                            ) const = 0;

    virtual VLIStatus       GetRGBAEntries (                    // Return color and alpha for multiple entries (VLIRGBAFloat)
                                unsigned int index,             // starting index (0 based)
                                unsigned int length,            // number of entries to return
                                VLIRGBAFloat* outRGBA           // RETURNED color and alpha data, 0.0 to 1.0
                            ) const = 0;


    // General routines to manage arbitrary fields

    virtual VLIStatus       UpdateField (                       // Update one lookup table field with new values
                                const void * inData,            // location of input data
                                unsigned int inIndex,                   // starting index (0 based)
                                unsigned int inLength,                  // number of entries to set
                                const VLIFieldDescriptor & inTableField // description of output
                            ) = 0;

    virtual VLIStatus       UnloadField (                       // Unload one lookup table field to application storage
                                void * outData,                 // RETURNED field data
                                unsigned int inIndex,           // starting index (0 based)
                                unsigned int inLength,          // number of entries to return
                                const VLIFieldDescriptor & inTableField // description of output
                            ) const = 0;

    virtual VLIStatus Reserved00(void);
    virtual VLIStatus Reserved01(void);
    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);

protected:
                            VLILookupTable (void) {}
    virtual                 ~VLILookupTable (void) {}
};


/***************************************************************************
 *
 *       This section describes the VLIClassifier class and helper classes
 *       (VLIOpcode and VLIOperation)
 *
 ***************************************************************************/

enum VLIOpcode          // Operations possible for the combination units
{                       // A is the left operand
                        // B is the right operand

    // The first operations are supported for all three combination units

    kVLI_Zero                           = 0,    // all 0's
    kVLI_A_And_B                        = 1,    // A AND B
    kVLI_A_And_NotB                     = 2,    // A AND (NOT B)
    kVLI_A                              = 3,    // A
    kVLI_NotA_And_B                     = 4,    // (NOT A) AND B
    kVLI_B                              = 5,    // B
    kVLI_A_Xor_B                        = 6,    // A XOR B    == (A AND (NOT B)) OR ((NOT A) AND B)
    kVLI_A_Or_B                         = 7,    // A OR B
    kVLI_A_Nor_B                        = 8,    // A NOR B    == NOT (A OR B)
    kVLI_A_Equiv_B                      = 9,    // A EQUIV B  == (A AND B) OR ((NOT A) AND (NOT B))
    kVLI_NotB                           = 10,   // NOT B
    kVLI_A_Or_NotB                      = 11,   // A OR (NOT B)
    kVLI_NotA                           = 12,   // NOT A
    kVLI_NotA_Or_B                      = 13,   // (NOT A) OR B
    kVLI_A_Nand_B                       = 14,   // A NAND B   == NOT (A AND B)
    kVLI_One                            = 15,   // all 1's

    // Note that all results are clamped to the range 0 to 1

    // Addition and Subtraction

    kVLI_A_Plus_B                       = 16,   // Clamp (A + B)
    kVLI_A_Plus_OneMinusB               = 17,   // Clamp (A + 1 - B)
    kVLI_OneMinusA_Plus_B               = 18,   // Clamp (1 - A + B)
    kVLI_OneMinusA_Plus_OneMinusB       = 19,   // Clamp (1 - A + 1 - B) == Clamp (2 - A - B)
    kVLI_OneMinusA_Minus_B              = 20,   // Clamp (1 - A - B)
    kVLI_B_Minus_A                      = 21,   // Clamp (B - A)
    kVLI_A_Minus_B                      = 22,   // Clamp (A - B)
    kVLI_A_Plus_B_Minus_One             = 23,   // Clamp (A + B - 1)

    // Minimums and Maximums

    kVLI_A_Min_B                        = 24,   // Min (A, B)
    kVLI_A_Min_OneMinusB                = 25,   // Min (A, 1 - B)
    kVLI_OneMinusA_Min_B                = 26,   // Min (1 - A, B)
    kVLI_OneMinusA_Min_OneMinusB        = 27,   // Min (1 - A, 1 - B)
    kVLI_OneMinusA_Max_OneMinusB        = 28,   // Max (1 - A, 1 - B)
    kVLI_OneMinusA_Max_B                = 29,   // Max (1 - A, B)
    kVLI_A_Max_OneMinusB                = 30,   // Max (A, 1 - B)
    kVLI_A_Max_B                        = 31,   // Max (A, B)

    // The following operations are supported only for kCombineFinal
    // Multiplications

    kVLI_A_Times_B                      = 32,   // A * B
    kVLI_A_Times_OneMinusB              = 33,   // A * (1 - B)
    kVLI_OneMinusA_Times_B              = 34,   // (1 - A) * B
    kVLI_OneMinusA_Times_OneMinusB      = 35,   // (1 - A) * (1 - B)
    kVLI_2X_A_Times_B                   = 36,   // Clamp (2 * A * B)
    kVLI_2X_A_Times_OneMinusB           = 37,   // Clamp (2 * A * (1 - B))
    kVLI_2X_OneMinusA_Times_B           = 38,   // Clamp (2 * (1 - A) * B)
    kVLI_2X_OneMinusA_Times_OneMinusB   = 39,   // Clamp (2 * (1 - A) * (1 - B))
    kVLI_4X_A_Times_B                   = 40,   // Clamp (4 * A * B)
    kVLI_4X_A_Times_OneMinusB           = 41,   // Clamp (4 * A * (1 - B))
    kVLI_4X_OneMinusA_Times_B           = 42,   // Clamp (4 * (1 - A) * B)
    kVLI_4X_OneMinusA_Times_OneMinusB   = 43,   // Clamp (4 * (1 - A) * (1 - B))
    kVLI_8X_A_Times_B                   = 44,   // Clamp (8 * A * B)
    kVLI_8X_A_Times_OneMinusB           = 45,   // Clamp (8 * A * (1 - B))
    kVLI_8X_OneMinusA_Times_B           = 46,   // Clamp (8 * (1 - A) * B)
    kVLI_8X_OneMinusA_Times_OneMinusB   = 47,   // Clamp (8 * (1 - A) * (1 - B))

    // Multiplications with final inversion

    kVLI_One_Minus_A_Times_B                    = 48,   // 1 - (A * B)
    kVLI_One_Minus_A_Times_OneMinusB            = 49,   // 1 - (A * (1 - B))
    kVLI_One_Minus_OneMinusA_Times_B            = 50,   // 1 - ((1 - A) * B)
    kVLI_One_Minus_OneMinusA_Times_OneMinusB    = 51,   // 1 - ((1 - A) * (1 - B))
    kVLI_One_Minus_2X_A_Times_B                 = 52,   // 1 - Clamp (2 * A * B)
    kVLI_One_Minus_2X_A_Times_OneMinusB         = 53,   // 1 - Clamp (2 * A * (1 - B))
    kVLI_One_Minus_2X_OneMinusA_Times_B         = 54,   // 1 - Clamp (2 * (1 - A) * B)
    kVLI_One_Minus_2X_OneMinusA_Times_OneMinusB = 55,   // 1 - Clamp (2 * (1 - A) * (1 - B))
    kVLI_One_Minus_4X_A_Times_B                 = 56,   // 1 - Clamp (4 * A * B)
    kVLI_One_Minus_4X_A_Times_OneMinusB         = 57,   // 1 - Clamp (4 * A * (1 - B))
    kVLI_One_Minus_4X_OneMinusA_Times_B         = 58,   // 1 - Clamp (4 * (1 - A) * B)
    kVLI_One_Minus_4X_OneMinusA_Times_OneMinusB = 59,   // 1 - Clamp (4 * (1 - A) * (1 - B))
    kVLI_One_Minus_8X_A_Times_B                 = 60,   // 1 - Clamp (8 * A * B)
    kVLI_One_Minus_8X_A_Times_OneMinusB         = 61,   // 1 - Clamp (8 * A * (1 - B))
    kVLI_One_Minus_8X_OneMinusA_Times_B         = 62,   // 1 - Clamp (8 * (1 - A) * B)
    kVLI_One_Minus_8X_OneMinusA_Times_OneMinusB = 63    // 1 - Clamp (8 * (1 - A) * (1 - B))

};

static const int kVLIOperationRedShift   = 0;
static const int kVLIOperationGreenShift = 8;
static const int kVLIOperationBlueShift  = 16;
static const int kVLIOperationAlphaShift = 24;
static const VLIuint32 kVLIOperationMask = 0xff;

class VLIEXPORT VLIOperation
{
public:


                            VLIOperation (void)                     // Default constructor, needed for arrays
    : m_opcodes (     (kVLI_Zero << kVLIOperationRedShift)          // Note -- not very useful
                    | (kVLI_Zero << kVLIOperationGreenShift)
                    | (kVLI_Zero << kVLIOperationBlueShift)
                    | (kVLI_Zero << kVLIOperationAlphaShift) )
    {
    }
                            VLIOperation (                      // Create a VLIOperation with four different opcodes
                                VLIOpcode inRed,                // Red channel opcode
                                VLIOpcode inGreen,              // Green channel opcode
                                VLIOpcode inBlue,               // Blue channel opcode
                                VLIOpcode inAlpha               // ALpha channel opcode
                            )

    : m_opcodes (     ((inRed   & kVLIOperationMask) << kVLIOperationRedShift)
                    | ((inGreen & kVLIOperationMask) << kVLIOperationGreenShift)
                    | ((inBlue  & kVLIOperationMask) << kVLIOperationBlueShift)
                    | ((inAlpha & kVLIOperationMask) << kVLIOperationAlphaShift) )
    {
    }

                            VLIOperation (                      // Create a VLIOperation with one opcode for all four channels
                                VLIOpcode inCommon              // Common opcode
                            )
    : m_opcodes (     ((inCommon & kVLIOperationMask) << kVLIOperationRedShift)
                    | ((inCommon & kVLIOperationMask) << kVLIOperationGreenShift)
                    | ((inCommon & kVLIOperationMask) << kVLIOperationBlueShift)
                    | ((inCommon & kVLIOperationMask) << kVLIOperationAlphaShift) )
    {
    }

    VLIOpcode               GetRed (void) const                 // Return red operation code
    {
        return static_cast<VLIOpcode>((m_opcodes >> kVLIOperationRedShift) & kVLIOperationMask);
    }

    VLIOpcode               GetGreen (void) const               // Return green operation code
    {
        return static_cast<VLIOpcode>((m_opcodes >> kVLIOperationGreenShift) & kVLIOperationMask);
    }

    VLIOpcode               GetBlue (void) const                // Return blue operation code
    {
        return static_cast<VLIOpcode>((m_opcodes >> kVLIOperationBlueShift) & kVLIOperationMask);
    }

    VLIOpcode               GetAlpha (void) const               // Return alpha operation code
    {
        return static_cast<VLIOpcode>((m_opcodes >> kVLIOperationAlphaShift) & kVLIOperationMask);
    }

    VLIbool operator == (VLIOperation inOther) const            // Compare two VLIOperation values
    {
        return VLIbool (m_opcodes == inOther.m_opcodes);
    }

    VLIbool operator != (VLIOperation inOther) const            // Compare two VLIOperation values
    {
        return VLIbool (m_opcodes != inOther.m_opcodes);
    }

protected:
    VLIuint32               m_opcodes;
};

class VLIEXPORT VLIClassifier
{
public:
                            VLIClassifier (void                 // an invalid classifier.
                            );

                            VLIClassifier (                     // copy constructor
                                const VLIClassifier & inSource  // Classifier to copy
                            );

    virtual                 ~VLIClassifier (void                // virtual destructor
                            );

    VLIClassifier &         operator = (                        // assignment operator
                                    const VLIClassifier & inSource // Classifier to copy
                            );

    // The following functions create new classifier objects

    // The source numbers select a field from the VLIVolume object
    // when the classifier is used while rendering a volume.

    // Single-field classifiers, both direct and mapped through a lookup table.

    static VLIClassifier    Direct (                            // Create a classifier for one voxel field, unmapped
                                VLIFieldNumber inSource,        // volume field number
                                VLIChannel inChannel = kVLIChannelRed // output channel number
                            );

    static VLIClassifier    Mapped (                            // Create a classifier for one voxel field, mapped
                                VLIFieldNumber inSource,        // volume field number to be looked up in the given LUT
                                VLILookupTable *inLUT           // lookup table
                            );

    // Two-field classifiers, both direct and mapped through two lookup
    // tables. Three channels are used from the luminance LUT (red, green and blue)
    // and the alpha channel is used from the alpha LUT

    static VLIClassifier    DirectLA (                          // Create a classifier for two voxel fields, unmapped
                                VLIFieldNumber inLuminanceSource,   // volume field number for kVLIChannelRed through Blue
                                VLIFieldNumber inAlphaSource        // volume field number for kVLIChannelAlpha
                            );

    static VLIClassifier    MappedLA (                          // Create a classifier for two voxel fields, each mapped
                                VLIFieldNumber inLuminanceSource,   // volume field number to be looked up in the luminance LUT
                                VLIFieldNumber inAlphaSource,       // volume field number to be looked up in the alpha lut
                                VLILookupTable *inLuminanceLUT,     // lookup table for luminance, generating kVLIChannelRed through Blue
                                VLILookupTable *inAlphaLUT          // lookup table for alpha, generating kVLIChannelAlpha
                            );

    // Four-field classifiers, both direct and mapped through four lookup
    // tables. One channel is used from each LUT.

    static VLIClassifier    DirectRGBA (                        // Create a classifier for four voxel fields, unmapped
                                VLIFieldNumber inRedSource,     // volume field number for kVLIChannelRed
                                VLIFieldNumber inGreenSource,   // volume field number for kVLIChannelGreen
                                VLIFieldNumber inBlueSource,    // volume field number for kVLIChannelBlue
                                VLIFieldNumber inAlphaSource    // volume field number for kVLIChannelAlpha
                            );

    static VLIClassifier    MappedRGBA (                                                // Create a classifier for four voxel fields, each mapped
                                VLIFieldNumber inRedSource,     // volume field number to be looked up in the Red LUT
                                VLIFieldNumber inGreenSource,   // volume field number to be looked up in the Green LUT
                                VLIFieldNumber inBlueSource,    // volume field number to be looked up in the Blue LUT
                                VLIFieldNumber inAlphaSource,   // volume field number to be looked up in the Alpha LUT
                                VLILookupTable *inRedLUT,       // lookup table for kVLIChannelRed
                                VLILookupTable *inGreenLUT,     // lookup table for kVLIChannelGreen
                                VLILookupTable *inBlueLUT,      // lookup table for kVLIChannelBlue
                                VLILookupTable *inAlphaLUT      // lookup table for kVLIChannelAlpha
                            );

    // Classifier objects can be used in an expression tree also.

    // For Condor, there can be from one to four leaf nodes and only
    // two levels of operation. Only one leaf node can use a 4096-entry LUT.
    // Only some operations are available for the intermediate opcodes,
    // shown below as opA and opB.

    //              opC
    //          /\
    //         /  \
    //        /    \
    //         opA      opB
    //      /\       /\
    //     /  \     /  \
    //  src0 src1 src2 src3

    // The following function creates one node of a tree, if it is able to do
    // so. The resulting classifier tree may not be realizable on Condor; the
    // application needs to call IsValid() to see if it can be used.
    // The source classifiers are copied into the new classifier, so any future
    // changes to them do not affect this classifier.

    // Generally speaking, the input classifiers would be created as single
    // field classifiers, with lookup tables with either one or four output fields.

    static VLIClassifier    Combine (                                   // Create a classifier that combines two other classifiers
                                const VLIClassifier &inClassifierA,     // left classifier
                                VLIOperation inOperation,               // operations to use to combine the output channels
                                const VLIClassifier &inClassifierB      // right classifier
                            );

    VLIbool                 IsValid (void                           // Is this classifier valid?
                            ) const;

    // We may want specific creation routines to do things like having a 4-bit
    // tag field specify a color (RGB) which is modulated by an 8- or 12-bit
    // value field mapped through an alpha table. (The modulation is done by
    // the opacity weighing multiplier later in the pipeline.)

    // This could be created by:
    //              VLIClassifier color = VLIClassifier::Mapped (1, colorLUT);
    //              VLIClassifier alpha = VLIClassifier::Mapped (0, alphaLUT);
    //              VLIOperation opCode = VLIOperation (kA, kA, kA, kB);
    //              VLIClassifier final = VLIClassifier::Combine (color, opCode, alpha);


    // The following routines allow access to the Condor-specific classifier
    // tree opcodes and sources. It is possible that these will change in future hardware;
    // upward compatiblity is not guaranteed.

    // Set and Get source numbers
    VLIStatus               SetSource (                         // Set the volume field number for a lookup table index
                                VLITableNumber inTableNumber,   // lookup table number
                                VLIFieldNumber inSource         // volume field number
                            );

    VLIFieldNumber          GetSource (                         // Return the volume field number for a lookup table index
                                VLITableNumber inTableNumber    // lookup table number
                            ) const;

    // Lookup table access and combination operations

    VLIStatus               SetLookupTable (                    // Set the lookup table
                                VLITableNumber inTableNumber,   // lookup table number
                                VLILookupTable * inLUT          // pointer to the lookup table (may be 0)
                            );

    VLILookupTable *        GetLookupTable (                    // Return a pointer to the lookup table
                                VLITableNumber inTableNumber    // lookup table number
                            ) const;

    enum CombinationUnit    // Lookup table combination arithmetic/logic units
    {
        kCombine0And1,      // Combine lookup tables 0 and 1
        kCombine2And3,      // Combine lookup tables 2 and 3
        kCombineFinal       // Combine the results of the first two combination units
    };

    // Set and Get opcodes

    VLIStatus               SetOperation (                      // Set the operation codes for a combination unit
                                CombinationUnit inUnit,         // Unit number
                                VLIOperation inOperation        // Operation codes
                            );

    VLIOperation            GetOperation (                      // Return the operation codes for a combination unit
                                CombinationUnit inUnit          // Unit number
                            ) const;


    enum InterpolationOrder     // Which of Interpolation or Classification should occur first?
    {
        kInterpolateFirst,      // Interpolation then classification
        kClassifyFirst          // Classification then interpolation
    };

    VLIStatus               SetInterpolationOrder (             // Set the interpolation order
                                InterpolationOrder inOrder      // interpolation order
                            );

    InterpolationOrder      GetInterpolationOrder (void         // Return the interpolation order
                            ) const;

    VLIStatus               SetInterpolationMode (              // Set the interpolation mode
                                int inFieldNumber,              // kVLIField0 through 3 OR kVLIChannelRed through Alpha
                                VLIInterpolationMode inMode     // interpolation mode
                            );

    VLIInterpolationMode    GetInterpolationMode (              // Return the interpolation order
                                int inFieldNumber               // kVLIField0 through 3 OR kVLIChannelRed through Alpha
                            ) const;

    VLIbool                 GetOpacityWeighting (void           // Return opacity weighting flag
                            ) const;

    VLIStatus               SetOpacityWeighting (               // Set opacity weighting flag
                                VLIbool inOn                    // opacity weighting desired
                            );

protected:
    InterpolationOrder      m_renderingOrder;
    VLIbool                 m_opacityWeighting;
    VLIInterpolationMode    m_interpolationMode[kVLIMaxVoxelFields];
    VLIFieldNumber          m_source[kVLIMaxLookupTables];
    VLILookupTable *        m_lut[kVLIMaxLookupTables];
    VLIOperation            m_operation[kVLICombinationUnits];
    mutable int             m_isValid;
    mutable int             m_redirect[kVLIMaxLookupTables];
    mutable VLIOperation    m_HWoperation[kVLICombinationUnits];
};

/***************************************************************************
 *
 *       This section describes the VLIVolumeSource abstract class
 *
 ***************************************************************************/

//  The VLIVolumeSource class is an abstract class that defines
//  a plug-in architecture that would ultimately allow the application
//  (and VLI) to handle volumes with data from any arbitrary source.
//  There is no assumption made about how the volume is stored.
//  The volume could even be a stream from a pipe or a socket.
//
//  Each volume (not necessarily a file) is viewed as a collection of
//  voxels. The voxel is not interpreted by VLI at all, so its only
//  attribute is that it has a certain size.
//
//  This class describes the interface used by VLI to manipulate the
//  volume source---the concrete class that implements this needs
//  to define other routines (such as one to open a specific file)
//  to be useful to an application. The comments below on when the
//  various member functions are called refer to VLI's usage only.
//
//  The term 'implementation' is used below to mean the derived
//  class that implements this interface.
//
//  The UpdateVoxelField and UnloadVoxelField methods are implemented
//  by VLIVolumeSource, using other methods of VLIVolumeSource.
//  (UpdateVoxels, UnloadVoxels, MapVoxels and ReleaseVoxels)
//  This simplifies the task of writing derived classes.

class VLIEXPORT VLIVolumeSource : public VLIShareable
{
public:

    // The following routines are called once, when the application
    // passes a pointer to a VLIVolumeSource-derived class object
    // to the VLIVolume constructor. VLIVolume assumes that the
    // values indicated do not change during the lifetime of the
    // VLIVolume object.

    virtual VLIVolumeRange  GetVolumeSize (void         // Return volume range: size and origin in voxels
                            ) const = 0;

    // Is this volume read-only or writeable (either CopyOnWrite or ReadWrite)
    virtual VLIbool         IsReadOnly (void            // Return VLItrue if volume should be read-only
                            ) const = 0;

    virtual VLIbool         HasData (void               // Return VLItrue if we have voxel data
                            ) const = 0;

    virtual VLIbool         CanResize (void             // Can this source accept a resize request?
                            ) const = 0;

    // initial ModelView matrix in the file
    virtual VLIMatrix       GetInitialCorrectionMatrix (void    // Return correction matrix
                            ) const = 0;

    virtual int             GetVoxelSize (void          // Return voxel size in bits (8, 16 or 32)
                            ) const = 0;                // in bits

    virtual VLIFieldDescriptor  GetFieldDescriptor (        // Return field descriptor for a field
                                VLIFieldNumber inFieldNumber    // field number to return
                            ) const = 0;

    // The following routines may be called multiple times

    // Copy a volume range into caller's buffer. The strides are the same as
    // the volume range size. The caller is in total charge of the memory,
    // and must make sure that it has enough space for the volume range.

    virtual VLIStatus       UnloadVoxels (              // Copy a portion of the volume data to caller's storage
                                void * outVoxels,                   // address to which to copy data
                                const VLIVolumeRange &inVolumeRange // range of volume data requested
                            ) = 0;

    virtual VLIStatus       UnloadVoxelField (          // Copy one field of the volume data to caller's storage
                                void * outVoxels,       // address to which to copy data
                                const VLIVolumeRange & inVolumeRange,           // range of volume to copy
                                const VLIFieldDescriptor & inVolumeField    // description of input -- size & position must be multiples of 8
                            );

    // Get a volume range, with strides determined by the implementation.
    // The pointer returned will be valid until the pointer is used
    // in a call to ReleaseVoxels. An implementation can provide one
    // or more volume range windows into the volume.

    // The stride in X must be equal to the voxel size, and so is not
    // returned by this routine. outYStride is the distance, in bytes,
    // from voxel (x,y,z) to voxel (x,y+1,z). outZStride is the distance,
    // in bytes, from voxel (x,y,z) to voxel (x,y,z+1).

    // If the 'inMayWrite' flag is set to VLIfalse, the caller must
    // not modify the data using the given pointer.
    // If 'inMayWrite' is set to VLItrue, the caller may modify
    // the data using the given pointer. The implementation must save
    // the modified data until the implementation object is deleted.

    // VLI will call MapVoxels when MapVolume and MapSubVolume are
    // called (when the target is kVLISource), and possibly when rendering.

    virtual VLIStatus       MapVoxels (                 // Return a pointer to volume data
                                const VLIVolumeRange &inVolumeRange,    // range of volume data requested
                                VLIbool inMayWrite,                     // if VLItrue, caller may write new values
                                void *& outVoxels,                      // RETURNED pointer to volume data
                                unsigned int & outXSize,                // RETURNED # voxels in X allocated
                                unsigned int & outYSize                 // RETURNED # voxels in Y allocated
                            ) = 0;

    // VLI will call ReleaseVoxels when UnmapVolume is called, and
    // if MapVoxels was called during rendering, when the rendering
    // is far enough along that it no longer needs the data pointer.

    virtual VLIStatus       ReleaseVoxels (             // Release a mapping into volume data
                                void *inVoxels          // mapping to release
                            ) = 0;

    // VLI will call UpdateVoxels when UpdateVolume is called and the target is kVLISource

    // VLI will check the input arguments so that, for example,
    // the size of the input voxels is equal to the size of
    // the voxels in the volume.

    virtual VLIStatus       UpdateVoxels (                          // Update volume source with new data
                                const VLIVolumeRange &inVolumeRange,// volume range to update
                                const void *inVoxels                // new data
                            ) = 0;

    virtual VLIStatus       UpdateVoxelField (                      // Update volume source with new data
                                const void * inVoxels,              // location of input data
                                const VLIVolumeRange & inVolumeRange,       // range of volume to update
                                const VLIFieldDescriptor & inVolumeField    // description of output -- size & position must be multiples of 8
                            );

    virtual VLIStatus       Resize (                                // Resize the volume source
                                const VLIVolumeRange & inNewRange   // New range for volume
                            ) = 0;

    virtual VLIVolumeRange  ChangedRange (                          // Return the portion of the volume range that has changed
                                const VLIVolumeRange & inVolumeRange    // What range is being queried
                            ) const = 0;

    virtual void            ResetChangedRange (void                 // Reset the modified range
                            ) = 0;

    virtual VLIStatus Reserved00(void);
    virtual VLIStatus Reserved01(void);
    virtual VLIStatus Reserved02(void);
    virtual VLIStatus Reserved03(void);
    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);
    virtual VLIStatus Reserved06(void);
    virtual VLIStatus Reserved07(void);
    virtual VLIStatus Reserved08(void);
    virtual VLIStatus Reserved09(void);


protected:
                            VLIVolumeSource (void) {}
    virtual                 ~VLIVolumeSource (void) {}

};

/***************************************************************************
*
*        This section describes the VLIVolume class/object that manages
*        voxel data.
*
****************************************************************************/

class VLIEXPORT VLIVolume : public VLIShareable
{

public:

    enum VLI_DEPRECATED(Layout)
    {
        kSliced,            // data is a sequence of XY slices
        kBlocked            // data is arranged as multiple XYZ blocks; NOT SUPPORTED
    };

    enum ReadMode           // Can volumes created from sources be modified?
    {
        kReadOnly,          // no, sources and volumes are read-only
        kCopyOnWrite,       // sources are read-only, volumes can be modified
        kReadWrite          // modifying the volume will cause the source to change
    };

    // Object factories: creating new VLIVolumes

#if VLIAllowDeprecated
    static VLIVolume        * Create (                          // DEPRECATED Create VLI volume from main memory data (or sourceless)
                                VLIuint32 inFormat,             // voxel format
                                void *inVoxels,                 // pointer to voxel data in slices
                                unsigned int inSizeX,           // size of the volume in X in voxels
                                unsigned int inSizeY,           // size of the volume in Y in voxels
                                unsigned int inSizeZ,           // size of the volume in Z in voxels
                                VLI_DEPRECATED(Layout) inLayout = kSliced       // layout: must be kSliced
                            );
#endif // VLIAllowDeprecated

    static VLIVolume        * CreateFromFile (                  // Create VLI volume from volume data in a file
                                const char *inFilename,         // name of file to be loaded
                                int inFileType = 0,             // type of file, 0 = decipher
                                ReadMode inReadMode = kReadOnly,// read mode
                                int inVolumeNumber = 0          // which volume do we want from the file?
                            );

    static VLIVolume        * CreateFromSource (                // Create VLI volume from application defined source
                                VLIVolumeSource * inVolumeSource// application defined source
                            );

    // Note: pointer to voxels moved to end of argument list to increase the
    // difference from old Create routine.
    static VLIVolume        * Create (                          // Create VLI volume from main memory data (or sourceless)
                                int inVoxelSize,                // size of voxels, in bits (must be 8, 16 or 32)
                                unsigned int inSizeX,           // size of the volume in X in voxels
                                unsigned int inSizeY,           // size of the volume in Y in voxels
                                unsigned int inSizeZ,           // size of the volume in Z in voxels
                                int inNumberOfFields = 0,       // number of fields in field array
                                const VLIFieldDescriptor * inFieldArray = 0,    // field array
                                void * inVoxels = 0             // pointer to voxel data in slices
                            );

    static VLIVolume        * Create (                          // Create VLI volume from main memory data (or sourceless)
                                int inVoxelSize,                // size of voxels, in bits (must be 8, 16 or 32)
                                const VLIVolumeRange & inRange, // size and origin of the volume in voxels
                                int inNumberOfFields = 0,       // number of fields in field array
                                const VLIFieldDescriptor * inFieldArray = 0,    // field array
                                void * inVoxels = 0             // pointer to voxel data in slices
                            );

    // Queries for whole volume state

    virtual VLIbool         IsReadOnly (void                    // Was this volume created in read-only mode?
                            ) const = 0;

    virtual VLIuint32       VLI_DEPRECATED(GetFormat) (void // DEPRECATED Return format of the volume
                            ) const = 0;

    virtual VLI_DEPRECATED(Layout)  VLI_DEPRECATED(GetLayout) (void // DEPRECATED Return layout of the volume(slice/block)
                            ) const = 0;

    virtual VLIVolumeRange  GetRange (void                      // Return the range (size and origin) of the volume in voxels
                            ) const = 0;

    virtual void            GetSize (                           // Return the size of the volume in voxels
                                unsigned int& outSizeX,         // RETURNED size of volume in X in voxels
                                unsigned int& outSizeY,         // RETURNED size of volume in Y in voxels
                                unsigned int& outSizeZ          // RETURNED size of volume in Z in voxels
                            ) const = 0;

    virtual unsigned int    GetSizeX (void                      // Return the X size of the volume in voxels
                            ) const = 0;

    virtual unsigned int    GetSizeY (void                      // Return the Y size of the volume in voxels
                            ) const = 0;

    virtual unsigned int    GetSizeZ (void                      // Return the Z size of the volume in voxels
                            ) const = 0;

    virtual int             GetVoxelSize (void                  // Return the size of voxel in bits (8, 16 or 32)
                            ) const = 0;

    // Field descriptor access

    virtual VLIFieldDescriptor  GetFieldDescriptor (                // Return a field descriptor
                                VLIFieldNumber inFieldNumber    // which field to return
                            ) const = 0;

    virtual VLIStatus       SetFieldDescriptor (                        // Set a field descriptor
                                VLIFieldNumber inFieldNumber,           // which field to set
                                const VLIFieldDescriptor &inDescriptor  // field descriptor
                            ) = 0;

    // Locking and unlocking

    virtual VLIStatus       LockVolume (                        // Lock volume onto a board
                                VLILocation inLocation = kVLIAnyBoard   // location desired
                            ) = 0;

    virtual VLIbool         IsLocked (void                      // Return VLItrue if volume is locked (Has a buffer)
                            ) const = 0;

    virtual VLIStatus       UnlockVolume (                      // Unlock the volume from a board
                                VLIbool inUnloadToSource = VLIfalse     // should VLI unload modified data back to the source?
                            ) = 0;

    // Access to Correction matrix

    virtual VLIStatus       SetCorrectionMatrix (               // Set the correction matrix
                                const VLIMatrix& inCorrection   // new correction matrix to set in volume
                            ) = 0;

    virtual VLIMatrix       GetCorrectionMatrix (void           // Return the correction matrix
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetModelMatrix) (    // DEPRECATED Set the correction matrix from the given one
                                const VLIMatrix& inModel        // new correction matrix to set in volume
                            ) = 0;

    virtual VLIMatrix       VLI_DEPRECATED(GetModelMatrix) (void    // DEPRECATED Return the correction matrix
                            ) const = 0;

    // Active SubVolume: that portion of the volume to be used for rendering

    virtual VLIStatus       SetActiveSubVolume (                // Set the active subvolume
                                const VLIVolumeRange &inVolumeRange     // range for the active subvolume
                            ) = 0;

    virtual VLIVolumeRange  GetActiveSubVolume (                // Return the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeSizeX (void                   // Return the X size of the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeSizeY (void                   // Return the Y size of the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeSizeZ (void                   // Return the Z size of the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeOriginX (void                 // Return the X origin of the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeOriginY (void                 // Return the Y origin of the active subvolume
                            ) const = 0;

    virtual int GetActiveSubVolumeOriginZ (void                 // Return the Z origin of the active subvolume
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED (SetActiveSubVolumeSize) (   // DEPRECATED Set the active subvolume size
                                unsigned int inSizeX,           // size of active subvolume in X in voxels
                                unsigned int inSizeY,           // size of active subvolume in Y in voxels
                                unsigned int inSizeZ            // size of active subvolume in Z in voxels
                            ) = 0;

    virtual void            VLI_DEPRECATED (GetActiveSubVolumeSize) (   // DEPRECATED Return the active subvolume size
                                unsigned int& outSizeX,         // RETURNED size of active subvolume in X in voxels
                                unsigned int& outSizeY,         // RETURNED size of active subvolume in Y in voxels
                                unsigned int& outSizeZ          // RETURNED size of active subvolume in Z in voxels
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED (SetActiveSubVolumeOrigin) ( // DEPRECATED Set the active subvolume origin
                                unsigned int inOriginX,         // origin of active subvolume in X in voxels
                                unsigned int inOriginY,         // origin of active subvolume in Y in voxels
                                unsigned int inOriginZ          // origin of active subvolume in Z in voxels
                            ) = 0;

    virtual void            VLI_DEPRECATED (GetActiveSubVolumeOrigin) ( // DEPRECATED Return the active subvolume origin
                                unsigned int& outOriginX,       // RETURNED origin of active subvolume in X in voxels
                                unsigned int& outOriginY,       // RETURNED origin of active subvolume in Y in voxels
                                unsigned int& outOriginZ        // RETURNED origin of active subvolume in Z in voxels
                            ) const = 0;

    virtual VLIStatus       SetBorderValue (                    // Set the border value (VLIuint32)
                                VLIuint32 inBorderValue         // border value (voxel value)
                            ) = 0;

    virtual VLIuint32       GetBorderValue (void                // Return the border value
                            ) const = 0;

    enum ExtendMode
    {
        kExtendWithBorder   = 0,
        kExtendWithEdge     = 1
    };

    virtual VLIStatus       SetExtendMode (                     // Set extend modes for all dimensions
                                ExtendMode inCommonMode         // extend mode to use for all dimension
                            ) = 0;

    virtual VLIStatus       SetExtendModes (                    // Set extend modes
                                ExtendMode inXMode,             // extend mode to use for the X dimension
                                ExtendMode inYMode,             // extend mode to use for the Y dimension
                                ExtendMode inZMode              // extend mode to use for the Z dimension
                            ) = 0;

    virtual void            GetExtendModes (                    // Return extend modes
                                ExtendMode &outXMode,           // RETURNED extend mode to use for the X dimension
                                ExtendMode &outYMode,           // RETURNED extend mode to use for the Y dimension
                                ExtendMode &outZMode            // RETURNED extend mode to use for the Z dimension
                            ) const = 0;

    virtual ExtendMode      GetExtendModeX (void                // Return extend mode to use for the X dimension
                            ) const = 0;

    virtual ExtendMode      GetExtendModeY (void                // Return extend mode to use for the Y dimension
                            ) const = 0;

    virtual ExtendMode      GetExtendModeZ (void                // Return extend mode to use for the Z dimension
                            ) const = 0;

    // Updating the volume with new data

    virtual VLIStatus       Update (                            // Update a range of a volume with new data
                                const void *inVoxels,           // data
                                const VLIVolumeRange &inDestinationRange        // range of data to update
                            ) = 0;

    virtual VLIStatus       Update (                            // Update a range of a volume from another volume
                                const VLIVolume * inVolume,     // volume supplying source
                                const VLIVolumeRange &inDestinationRange,       // range of data to update
                                int inSourceX = 0,              // X Origin of source range
                                int inSourceY = 0,              // Y Origin of source range
                                int inSourceZ = 0,              // Z Origin of source range
                                VLISource inSource = kVLIUseTarget // Do we want the source or the buffer of inVolume?
                                                                // (default: current target)
                            ) = 0;

    virtual VLIStatus       UpdateField (                       // Update a field of a range of a volume with new data
                                const void * inVoxels,          // location of input data
                                const VLIVolumeRange     & inVolumeRange,       // range of volume to update
                                const VLIFieldDescriptor & inDestinationField   // description of output -- size & position must be multiples of 8
                            ) = 0;

    virtual VLISyncEvent    StartUpdate (                       // ASYNCHRONOUS Start to Update a range of a volume with new data
                                const void *inVoxels,           // data
                                const VLIVolumeRange &inDestinationRange        // range of data to update
                            ) = 0;

    virtual VLISyncEvent    StartUpdate (                       // ASYNCHRONOUS Start to update a range of a volume from another volume
                                const VLIVolume * inVolume,     // volume supplying source
                                const VLIVolumeRange &inDestinationRange,       // range of data to update
                                int inSourceX = 0,              // X Origin of source range
                                int inSourceY = 0,              // Y Origin of source range
                                int inSourceZ = 0,              // Z Origin of source range
                                VLISource inSource = kVLIUseTarget // Do we want the source or the buffer of inVolume?
                                                                // (default: current target)
                            ) = 0;

    virtual VLISyncEvent    StartUpdateField (                  // ASYNCHRONOUS Start to update a field of a range of a volume with new data
                                const void               * inVoxels,    // location of input data
                                const VLIVolumeRange     & inVolumeRange,       // range of volume to update
                                const VLIFieldDescriptor & inDestinationField   // description of output -- size & position must be multiples of 8
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED (UpdateVolume) (     // DEPRECATED Update a range of a volume with new data
                                VLIuint32 inFormat,             // voxel format
                                const void *inVoxels,           // new data
                                unsigned int inTargetXVox,      // where to change the volume in X
                                unsigned int inTargetYVox,      // where to change the volume in Y
                                unsigned int inTargetZVox,      // where to change the volume in Z
                                unsigned int inNXVox,           // size of the volume range in X
                                unsigned int inNYVox,           // size of the volume range in Y
                                unsigned int inNZVox,           // size of the volume range in Z
                                VLI_DEPRECATED(Layout) inLayout = kSliced       // layout: must be kSliced
                            ) = 0;

    // Mapping the volume for application access

    virtual VLIStatus       MapVolume (                         // Return a pointer to 8-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                VLIuint8 *&outBaseAddress,      // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapVolume (                         // Return a pointer to 16-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                VLIuint16 *&outBaseAddress,     // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapVolume (                         // Return a pointer to 32-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                VLIuint32 *&outBaseAddress,     // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapVolume (                         // Return a pointer to voxel data
                                VLIAccessType inAccess,         // access type requested
                                void *&outBaseAddress,          // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapSubVolume (                      // Return a pointer to a portion of 8-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                const VLIVolumeRange & inVolumeRange,   // range of data to map
                                VLIuint8 *&outBaseAddress,      // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapSubVolume (                      // Return a pointer to a portion of 16-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                const VLIVolumeRange & inVolumeRange,   // range of data to map
                                VLIuint16 *&outBaseAddress,     // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapSubVolume (                      // Return a pointer to a portion of 32-bit voxel data
                                VLIAccessType inAccess,         // access type requested
                                const VLIVolumeRange & inVolumeRange,   // range of data to map
                                VLIuint32 *&outBaseAddress,     // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       MapSubVolume (                      // Return a pointer to a portion of voxel data
                                VLIAccessType inAccess,         // access type requested
                                const VLIVolumeRange & inVolumeRange,   // range of data to map
                                void *&outBaseAddress,          // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    virtual VLIStatus       UnmapVolume (void                   // Unmap volume or subvolume
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED (MapSubVolume) (     // DEPRECATED Return a pointer to a portion of voxel data
                                VLIAccessType inAccess,         // access type requested
                                unsigned int inAtX,             // position of volume range in X
                                unsigned int inAtY,             // position of volume range in Y
                                unsigned int inAtZ,             // position of volume range in Z
                                unsigned int inNx,              // size of volume range in X
                                unsigned int inNy,              // size of volume range in Y
                                unsigned int inNz,              // size of volume range in Z
                                void *&outBaseAddress,          // RETURNED address of data
                                unsigned int &outSx,            // RETURNED size of mapping in X, in voxels
                                unsigned int &outSy,            // RETURNED size of mapping in Y, in voxels
                                VLISource inSource = kVLIUseTarget // Do we want map to the source or the buffer?
                            ) = 0;

    // Unloading volume data to application memory

    virtual VLIStatus       Unload (                                // Unload volume data to application memory
                                void * outVoxels,                   // where to place the data
                                const VLIVolumeRange & inVolumeRange, // range of data to unload
                                VLISource inSource = kVLIUseTarget // Do we want unload from the source or the buffer?
                            ) const = 0;

    virtual VLIStatus       UnloadField (                           // Unload a field of volume data to application memory
                                void * outVoxels,                   // where to place the data
                                const VLIVolumeRange & inVolumeRange,   // range of data to unload
                                const VLIFieldDescriptor & inSourceField, // field to unload -- size & position must be multiples of 8
                                VLISource inSource = kVLIUseTarget // Do we want unload from the source or the buffer?
                            ) const = 0;

    virtual VLISyncEvent    StartUnload (                           // ASYNCHRONOUS Start to unload volume data to application memory
                                void * outVoxels,                   // where to place the data
                                const VLIVolumeRange & inVolumeRange, // range of data to unload
                                VLISource inSource = kVLIUseTarget // Do we want unload from the source or the buffer?
                            ) const = 0;

    virtual VLISyncEvent    StartUnloadField (                      // ASYNCHRONOUS Start to unload a field of volume data to application memory
                                void * outVoxels,                   // where to place the data
                                const VLIVolumeRange & inVolumeRange,     // range of data to unload
                                const VLIFieldDescriptor & inSourceField, // field to unload -- size & position must be multiples of 8
                                VLISource inSource = kVLIUseTarget // Do we want unload from the source or the buffer?
                            ) const = 0;

    // Other queries

    virtual VLIbool         IsLoaded (void                      // Return VLItrue if volume is loaded on a board
                            ) const = 0;

    virtual VLIAccessType   GetAccessControl (void              // Return current access control
                            ) const = 0;

    virtual VLIStatus       Resize (                            // Resize a volume
                                unsigned int inNewSizeX,        // new size for X
                                unsigned int inNewSizeY,        // new size for Y
                                unsigned int inNewSizeZ,        // new size for Z
                                VLIbool inResizeBuffer = VLItrue    // resize buffer also?
                            ) = 0;

    virtual VLIStatus       Resize (                            // Resize a volume
                                const VLIVolumeRange & inNewRange,  // new addressing range for this volume
                                VLIbool inResizeBuffer = VLItrue    // resize buffer also?
                            ) = 0;

    // Buffer routines

    // All the buffer creation routines initialize the buffer in offset mode

    virtual VLIStatus       CreateBuffer (                      // Create a buffer with the same range as the volume
                                VLILocation inLocation          // Where to create buffer
                            ) = 0;

    virtual VLIStatus       CreateBuffer (                      // Create a buffer with the given size; offsets come from the volume range
                                VLILocation inLocation,         // Where to create buffer
                                unsigned int inSizeX,           // X size of buffer
                                unsigned int inSizeY,           // Y size of buffer
                                unsigned int inSizeZ            // Z size of buffer
                            ) = 0;

    virtual VLIStatus       CreateBuffer (                      // Create a buffer with the given range
                                VLILocation inLocation,         // Where to create buffer
                                const VLIVolumeRange & inRange  // Size and offset for buffer
                            ) = 0;

    virtual VLIBufferID     GetBufferID (void                   // Return buffer ID or kVLINoBuffer
                            ) const = 0;

    virtual void            GetBufferSize (                     // Return buffer size; all 0 if no buffer attached
                                unsigned int & outSizeX,        // RETURNED size in X
                                unsigned int & outSizeY,        // RETURNED size in Y
                                unsigned int & outSizeZ         // RETURNED size in Z
                            ) const = 0;

    virtual unsigned int    GetBufferSizeX (void                // Return buffer size in X; 0 if no buffer attached
                            ) const = 0;

    virtual unsigned int    GetBufferSizeY (void                // Return buffer size in Y; 0 if no buffer attached
                            ) const = 0;

    virtual unsigned int    GetBufferSizeZ (void                // Return buffer size in Z; 0 if no buffer attached
                            ) const = 0;

    virtual VLIVolumeRange  GetBufferRange (void                // Return buffer addressing range; all 0 if no buffer attached
                            ) const = 0;

    virtual VLIStatus       AttachBuffer (                      // Attach this volume to the specified buffer
                                VLIBufferID inBufferID          // Buffer ID
                            ) = 0;

    virtual VLIStatus       ReleaseBuffer (void                 // Release any buffer that may be attached
                            ) = 0;

    virtual VLIStatus       SetBufferPermission (               // Set permission for other processes
                                VLIPermission inBufferPermission
                            ) = 0;

    virtual VLIPermission   GetBufferPermission (void           // Return current permission
                            ) const = 0;

    virtual VLILocation     GetBufferLocation (void             // Return location of buffer
                            ) const = 0;                        // or kVLINoBufferAttached

    virtual VLIStatus       ResizeBuffer (                      // Change the size of the buffer
                                unsigned int inNewSizeX,        // new size for X
                                unsigned int inNewSizeY,        // new size for Y
                                unsigned int inNewSizeZ         // new size for Z
                            ) = 0;

    virtual VLIStatus       ResizeBuffer (                      // Change the buffer addressing range (offset mode)
                                const VLIVolumeRange & inNewRange       // new addressing range
                            ) = 0;

    // Controlling the mapping from object coordinates to data indices

    virtual VLIStatus       SetBufferWrap (                     // Set addressing mode to kVLIWrapAddress and set the modulus values
                                int inModulusX,                 // modulus for X (must be a power of 2)
                                int inModulusY,                 // modulus for Y (must be a power of 2)
                                int inModulusZ                  // modulus for Z (must be a power of 2)
                            ) = 0;

    virtual VLIStatus       SetBufferOffset (                   // Set addressing mode to kVLIOffsetAddress and set the offset values
                                int inOffsetX,                  // offset for X (must be even)
                                int inOffsetY,                  // offset for Y (must be even)
                                int inOffsetZ                   // offset for Z (must be even)
                            ) = 0;

    virtual void            GetBufferAddressParameters (        // Return the addressing parameters
                                VLIAddressMode & outMode,       // RETURNED kVLIOffsetAddress or kVLIWrapAddress
                                int &outAddressParameterX,      // RETURNED offset or modulus for X
                                int &outAddressParameterY,      // RETURNED offset or modulus for Y
                                int &outAddressParameterZ       // RETURNED offset or modulus for Z
                            ) const = 0;

    virtual VLIStatus       SetBufferAddressParameters (        // Set the addressing parameters
                                VLIAddressMode inMode,          // kVLIOffsetAddress or kVLIWrapAddress
                                int inAddressParameterX,        // offset or modulus for X
                                int inAddressParameterY,        // offset or modulus for Y
                                int inAddressParameterZ         // offset or modulus for Z
                            ) = 0;

    virtual VLIAddressMode  GetBufferAddressMode (void          // Return kVLIOffsetAddress or kVLIWrapAddress
                            ) const = 0;

    virtual int             GetBufferAddressParameterX (void    // Return the offset/modulus X field
                            ) const = 0;

    virtual int             GetBufferAddressParameterY (void    // Return the offset/modulus Y field
                            ) const = 0;

    virtual int             GetBufferAddressParameterZ (void    // Return the offset/modulus Z field
                            ) const = 0;

    // Do we want the source or the buffer?

    virtual VLIStatus       SetTarget (                         // Select source or buffer as target for updates
                                VLITarget inNewTarget           // Do we want the source or the buffer?
                            ) = 0;

    virtual VLITarget       GetTarget (void                     // Return current target
                            ) const = 0;

    // Rendering routines

    // Note that which buffers are input and which are output is determined by the state
    // contained in inContext. Normally, outColor0 represents the output image; this is
    // set by the context, using SetImageOutputBuffer().

    virtual VLIStatus       Render (                                // ASYNCHRONOUS Start to peform a volume render
                                const VLIContext * inContext,       // Context (state) to use (INPUT)
                                VLIImageBuffer * inOutColor0,       // color image 0 INPUT (FRONT) and/or OUTPUT
                                VLIDepthBuffer * inOutDepth0 = 0,   // depth image 0 INPUT (NEAR)  and/or OUTPUT
                                VLIImageBuffer * inOutColor1 = 0,   // color image 1 INPUT (BACK)  and/or OUTPUT
                                VLIDepthBuffer * inOutDepth1 = 0    // depth image 1 INPUT (FAR)   and/or OUTPUT
                            ) = 0;

    virtual VLISyncEvent    StartRender (                           // ASYNCHRONOUS Start to peform a volume render
                                const VLIContext * inContext,       // Context (state) to use (INPUT)
                                VLIImageBuffer * inOutColor0,       // color image 0 INPUT (FRONT) and/or OUTPUT
                                VLIDepthBuffer * inOutDepth0 = 0,   // depth image 0 INPUT (NEAR)  and/or OUTPUT
                                VLIImageBuffer * inOutColor1 = 0,   // color image 1 INPUT (BACK)  and/or OUTPUT
                                VLIDepthBuffer * inOutDepth1 = 0    // depth image 1 INPUT (FAR)   and/or OUTPUT
                            ) = 0;

    // Added for 3.1 Note -- these methods are not pure (no "= 0" notation).

    // Mipmap support

    virtual VLIStatus       SetMipmapAutoGenerate (                 // Enable or disable auto mipmap level generation
                                VLIbool inEnabled                   // VLItrue for enabled, VLIfalse for disabled
                            );
    
    virtual VLIbool         GetMipmapAutoGenerate ( void            // Return VLItrue if auto generate enabled
                            ) const;

    virtual VLIStatus       SetMipmapMinDimension (                 // Set the minimum size automatic mode will create
                                int inMinDimension
                            );

    virtual int             GetMipmapMinDimension ( void            // Return the minimum size
                            ) const;

    virtual VLIStatus       SetMipmapShrinkFactors (                // Set shrink factors
                                double inShrinkX,                   // X dimension shrink factor: 0.5 to 0.90
                                double inShrinkY,                   // Y dimension shrink factor: 0.5 to 0.90
                                double inShrinkZ                    // Z dimension shrink factor: 0.5 to 0.90
                            );

    virtual void            GetMipmapShrinkFactors (                // Return shrink factors
                                double & outShrinkX,                // RETURNED X dimension shrink factor
                                double & outShrinkY,                // RETURNED Y dimension shrink factor
                                double & outShrinkZ                 // RETURNED Z dimension shrink factor
                            ) const;
    // Note -- normally we would also add GetMipmapShrinkFactorX(void), etc, but
    // we also want to limit the number of methods added.

    virtual VLIStatus       SetMipmapInterpolationMode (            // Set interpolation mode for a field
                                VLIFieldNumber inField,             // Field number: kVLIField0 through 3
                                VLIInterpolationMode inMode         // mode: kVLINearestNeighbor or kVLITrilinear
                            );

    virtual VLIInterpolationMode  GetMipmapInterpolationMode (      // Return interpolation mode for a field
                                VLIFieldNumber inField              // Field number: kVLIField0 through 3
                            ) const;

    virtual VLIStatus       GenerateMipmapLevel (                   // Explicitly generate a mipmap level
                                int inLevelToGenerate,              // Level to generate, 1 to N - 1
                                int inSourceLevel = -1,             // Level to use as source: 0 to inLevelGenerate - 1;
                                                                    // -1 means use inLevelToGenerate - 1.
                                double inShrinkX = 0.0,             // X dimension Shrink factor: 0.5 to 0.90
                                double inShrinkY = 0.0,             // Y dimension Shrink factor: 0.5 to 0.90
                                double inShrinkZ = 0.0              // Z dimension Shrink factor: 0.5 to 0.90
                            );

    virtual VLISyncEvent    StartGenerateMipmapLevel (              // ASYNCHRONOUS Explicitly generate a mipmap level
                                int inLevelToGenerate,              // Level to generate, 1 to N - 1
                                int inSourceLevel = -1,             // Level to use as source: 0 to inLevelGenerate - 1;
                                                                    // -1 means use inLevelToGenerate - 1.
                                double inShrinkX = 0.0,             // X dimension Shrink factor: 0.5 to 0.90
                                double inShrinkY = 0.0,             // Y dimension Shrink factor: 0.5 to 0.90
                                double inShrinkZ = 0.0              // Z dimension Shrink factor: 0.5 to 0.90
                            );

    virtual VLIStatus       SetMipmapRange (                        // Set range of mipmap levels to use when rendering
                                int inMinLevel,                     // first mipmap level to use: 0 to N - 1
                                int inMaxLevel                      // last mipmap level to use: inMinLevel to N - 1
                            );
    
    virtual void            GetMipmapRange (                        // Return mipmap level range
                                int & outMinLevel,                  // RETURNED first mipmap level to use
                                int & outMaxLevel                   // RETURNED last mipmap level to use
                            ) const;

    // The single argument form of ChooseMipmapLevel uses the current MipmapRange
    virtual int             ChooseMipmapLevel (                     // Return the first level that will render,
                                                                    // or -1 to indicate none will render
                                const VLIContext * inContext        // Using this context (and camera)
                            ) const;

    virtual int             ChooseMipmapLevel (                     // Return the first level that will render,
                                                                    // or -1 to indicate none will render
                                const VLIContext * inContext,       // Using this context (and camera)
                                int inMinLevel,                     // first mipmap level to test
                                int inMaxLevel = 9999               // last mipmap level to test
                            ) const;

    // Volume resample support

    virtual VLIStatus       ResampleBuffer (                        // Resample this volume's buffer to another volume
                                VLIVolume * outDestination,                  // destination volume (buffer)
                                const VLIVolumeRange & inSourceRange,       // source range
                                const VLIVolumeRange & inDestinationRange,  // destination range
                                const VLIClassifier &inClassifier           // classifier to use
                            );

    virtual VLISyncEvent    StartResampleBuffer (                   // ASYNCHRONOUS Start to resample this volume to another volume
                                VLIVolume * outDestination,                   // destination volume (buffer)
                                const VLIVolumeRange & inSourceRange,       // source range
                                const VLIVolumeRange & inDestinationRange,  // destination range
                                const VLIClassifier &inClassifier           // classifier to use
                            );


    virtual VLIStatus Reserved16(void);
    virtual VLIStatus Reserved17(void);
    virtual VLIStatus Reserved18(void);
    virtual VLIStatus Reserved19(void);
    virtual VLIStatus Reserved20(void);
    virtual VLIStatus Reserved21(void);
    virtual VLIStatus Reserved22(void);
    virtual VLIStatus Reserved23(void);
    virtual VLIStatus Reserved24(void);
    virtual VLIStatus Reserved25(void);
    virtual VLIStatus Reserved26(void);
    virtual VLIStatus Reserved27(void);
    virtual VLIStatus Reserved28(void);
    virtual VLIStatus Reserved29(void);



protected:
                            VLIVolume (void) {}
    virtual                 ~VLIVolume (void) {}

};


/***************************************************************************
*
*        Error handler functions that handle system initialization
*        and configuration.
*
**************************************************************************/

class VLIEXPORT VLIErrorHandler
{
public:

    VLIErrorHandler (void);

    virtual ~VLIErrorHandler (void);

    virtual void            ErrorNotify (                       // Called when an error occurs
                                int inErrNum,                   // error number
                                const char *inFile,             // file in which error was detected
                                int inLineNum                   // line in file where error was detected
                            );
};

VLIEXPORT VLIErrorHandler* VLISetErrorHandler(VLIErrorHandler *inHandler);


/****************************************************************************
*
*        This section describes VLIEventBase and its children.  These clases
*        hold data used by callback routines.
*
***************************************************************************/

#if VLIAllowDeprecated
enum VLIEvent                       // DEPRECATED
{
    kVLIEventRenderDone,
    kVLIEventTransferDone,
    kVLIEventLightMap,
    kVLIEventLightMapChange,
    kVLIEventMultiPassBasePlane,
    kVLINumberOfEvents              // number of events
};

class VLIEXPORT VLIEventBase        // DEPRECATED
{
public:
    int                 buffer;
    virtual VLIEvent    EventType(void) const = 0;
    virtual             ~VLIEventBase(void) { }
                        VLIEventBase(int buffer);
};

class VLIEXPORT VLIEventRenderDone : public VLIEventBase        // DEPRECATED
{
public:
    VLIEvent            EventType(void) const;
                        VLIEventRenderDone(int buffer);
};

class VLIEXPORT VLIEventTransferDone : public VLIEventBase      // DEPRECATED
{
public:
    VLIEvent            EventType(void) const;
                        VLIEventTransferDone(int buffer);
};

typedef enum {kVLILightMapType1, kVLILightMapType2} VLILightMapType;                // DEPRECATED

class VLIEXPORT VLIEventLightMap : public VLIEventBase          // DEPRECATED
{
public:
    VLILightMapType     lightFormat;    // kVLILightMapType1 implies 2 maps
    int                 bytesPerElement;
    int                 numberOfElements;
    void                *pointerToMap1; // Diffuse
    void                *pointerToMap2; // Specular
    VLIEvent            EventType(void) const;
                        VLIEventLightMap(int buffer, VLILightMapType format, int eltSize,
                                            int nElts, void *map1, void *map2);
};

class VLIEXPORT VLIEventLightMapChange : public VLIEventBase    // DEPRECATED
{
public:
    VLILightMapType     lightFormat;    // kVLILightMapType1 implies 2 MapChanges
    int                 bytesPerElement;
    int                 numberOfElements;
    void                *pointerToMap1; // Diffuse
    void                *pointerToMap2; // Specular
    VLIEvent            EventType(void) const;
                        VLIEventLightMapChange(int buffer, VLILightMapType format, int eltSize,
                                    int nElts, void *MapChange1, void *MapChange2);
};


class VLIEXPORT VLIEventMultiPassBasePlane : public VLIEventBase// DEPRECATED
{
public:
    VLIEvent            EventType(void) const;
                        VLIEventMultiPassBasePlane (int buffer, int multiPassBuffer);
    int                 multiPassBuffer;
};


typedef void (*VLIEventCallback)(VLIEventBase *inEvent, void *inData);  // DEPRECATED

#endif // VLIAllowDeprecated

/****************************************************************************
*
*        This section describes the VLIContext class/object. This class
*        manages the rendering context and parameters.
*
***************************************************************************/

static const double * kVLILinearGradientRamp = 0;

class VLIEXPORT VLIContext : public VLIShareable
{
public:

    // Object factories

    static VLIContext *     Create (                            // Create a VLIContext object
                                VLILookupTable* inColor = 0     // color table 0
                            );

    // Camera access

    virtual VLICamera &     GetCamera (void                     // Return a reference to the camera in this context
                            ) = 0;

    virtual const VLICamera &   GetCamera (void                 // Return a reference to the camera in this context
                            ) const = 0;

    virtual VLIStatus       SetCamera (                         // Set the camera in this context
                                const VLICamera& inCamera       // new camera
                            ) = 0;


    // Lookup table access and combination operations

    virtual VLILookupTable* VLI_DEPRECATED(GetLookupTable) (void    // DEPRECATED Return lookup table 0 from the classifier
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetLookupTable) (    // DEPRECATED Set lookup table 0 in the classifier
                                VLILookupTable* inLUT           // pointer to lookup table
                            ) = 0;

    // Lights and Lighting properties


    virtual VLIStatus       AddLight (                          // Add this light to the context light list
                                VLILight *inLight               // pointer to light to be added
                            ) = 0;

    virtual VLIStatus       RemoveLight (                       // Remove this light from the context light list
                                VLILight *inLight               // pointer to light to be removed
                            ) = 0;

    virtual unsigned int    GetLightCount (void                 // Return the number of lights in the context light list
                            ) const = 0;

    virtual VLILight*       GetLight (                          // Return a pointer to the indicated light
                                unsigned int inI                // index of light to be returned
                            ) const = 0;

    virtual void            GetReflectionProperties (           // Return relection properties
                                double& outDiffuse,             // RETURNED diffuse coefficient (0.0 to 1.0)
                                double& outSpecular,            // RETURNED specular coefficient (0.0 to 1.0)
                                double& outEmissive,            // RETURNED emissive coefficient (0.0 to 1.0)
                                double& outSpecularExponent     // RETURNED specular exponent (>= 0.0)
                            ) const = 0;

    virtual double          GetReflectionPropertiesDiffuse (    // Return diffuse coefficient
                            ) const = 0;

    virtual double          GetReflectionPropertiesSpecular (   // Return specular coefficient
                            ) const = 0;

    virtual double          GetReflectionPropertiesEmissive (   // Return emissive coefficient
                            ) const = 0;

    virtual double          GetReflectionPropertiesSpecularExponent (   // Return specular exponent
                            ) const = 0;

    virtual VLIStatus       SetReflectionProperties (           // Set relection properties
                                double inDiffuse,               // diffuse coefficient (0.0 to 1.0)
                                double inSpecular,              // specular coefficient (0.0 to 1.0)
                                double inEmissive,              // emmisive coefficient (0.0 to 1.0)
                                double inSpecularExponent       // specular exponent (>= 0.0)
                            ) = 0;

    virtual void            GetSpecularColor (                  // Return specular color
                                double& outRed,                 // RETURNED red component (0.0 to 1.0)
                                double& outGreen,               // RETURNED green component (0.0 to 1.0)
                                double& outBlue                 // RETURNED blue component (0.0 to 1.0)
                            ) const = 0;

    virtual VLIRGBAFloat    GetSpecularColor (void              // Return specular color (ignore alpha)
                            ) const = 0;

    virtual VLIStatus       SetSpecularColor (                  // Set specular color
                                double inRed,                   // red component (0.0 to 1.0)
                                double inGreen,                 // green component (0.0 to 1.0)
                                double inBlue                   // blue component (0.0 to 1.0)
                            ) = 0;

    virtual VLIbool         GetGradientSpecularIlluminationModulation (void // Return specular modulation flag
                            ) const = 0;

    virtual VLIbool         GetGradientDiffuseIlluminationModulation (void  // Return diffuse modulation flag
                            ) const = 0;

    virtual VLIbool         GetGradientEmissiveIlluminationModulation (void // Return emmisive modulation flag
                            ) const = 0;

    virtual VLIStatus       SetGradientSpecularIlluminationModulation (     // Set specular modulation flag
                                VLIbool inGMIMSpecular                      // specular modulation desired
                            ) = 0;

    virtual VLIStatus       SetGradientDiffuseIlluminationModulation  (     // Set diffuse modulation flag
                                VLIbool inGMIMDiffuse                       // diffuse modulation desired
                            ) = 0;

    virtual VLIStatus       SetGradientEmissiveIlluminationModulation  (    // Set emissive modulation flag
                                VLIbool inGMIMEmissive                      // emissive modulation desired
                            ) = 0;

    // Opacity (Alpha) controls

    virtual VLIbool         GetCorrectOpacity (void             // Return opacity correction flag
                            ) const = 0;

    virtual VLIStatus       SetCorrectOpacity (                 // Set opacity correction flag
                                VLIbool inOn                    // opacity correction desired
                            ) = 0;

    virtual VLIbool         GetGradientOpacityModulation (void  // Return opacity modulation flag
                            ) const = 0;

    virtual VLIStatus       SetGradientOpacityModulation (      // Set opacity modulation flag
                                VLIbool inOn                    // opacity modulation desired
                            ) = 0;

    // Cut Plane access

    virtual VLIStatus       AddCutPlane (                       // Add this cut plane to the context cut plane list
                                VLICutPlane* inPlane            // cut plane to add
                            ) = 0;

    virtual VLIStatus       RemoveCutPlane (                    // Remove this cut plane to the context cut plane list
                                VLICutPlane* inPlane            // cut plane to remove
                            ) = 0;

    virtual unsigned int    GetCutPlaneCount (void              // Return the number of cut planes in the context cut plane list
                            ) const = 0;

    virtual VLICutPlane*    GetCutPlane (                       // Return a pointer to the indicated cut plane
                                unsigned int inI                // index of cut plane to be returned
                            ) const = 0;

    // Cropping

    virtual VLICrop &       GetCrop (void                       // Return a reference to the crop in this context
                            ) = 0;

    virtual const VLICrop & GetCrop (void                       // Return a reference to the crop in this context
                            ) const = 0;

    virtual VLIStatus       SetCrop (                           // Set the crop in this context
                                const VLICrop& inCrop           // new crop data
                            ) = 0;

    // The hardware 3D cursor DEPRECATED
    // Note: this is defined even if VLIDummyCursor is set to 0,
    // because we need to reserve the slot in the vtable.
    // However, the names have been changed.

#if VLIDummyCursor

    virtual VLICursor &     GetCursor (void                     // DEPRECATED Return a reference to the cursor in this context
                            ) = 0;

    virtual VLIStatus       SetCursor (                         // DEPRECATED Set the cursor in this context
                                const VLICursor& inCursor       // new cursor data
                            ) = 0;
#else // VLIDummyCursor is not set
    virtual VLICursorDeprecated &   GetCursorDeprecated (void   // DEPRECATED Return a reference to the cursor in this context
                            ) = 0;

    virtual VLIStatus       SetCursorDeprecated (               // DEPRECATED Set the cursor in this context
                                const VLICursorDeprecated& inCursor     // new cursor data
                            ) = 0;
#endif // VLIDummyCursor not set

    // The Classifier section

    virtual VLIClassifier & GetClassifier (void                 // Return a reference to the classifier in this context
                            ) = 0;

    virtual const VLIClassifier & GetClassifier (void           // Return a reference to the classifier in this context
                            ) const = 0;

    virtual VLIStatus       SetClassifier (                     // Set the classifer in this context
                                VLIClassifier const &inClassifier       // new classifier data
                            ) = 0;

    // Super Sampling controls

    virtual VLIStatus       SetSamplingFactor (                 // Set the super sampling factor
                                double inFactor                 // super sampling factor in Z (0.5 to 8.0)
                            ) = 0;

    virtual double          GetSamplingFactor (void             // Return the super sampling factor
                            ) const = 0;

    virtual void            VLI_DEPRECATED(GetSuperSamplingFactor) (    // DEPRECATED Return the super sampling factors
                                double& outX,                   // RETURNED super sampling factor in X (1.0 to 0.0625)
                                double& outY,                   // RETURNED super sampling factor in Y (1.0 to 0.0625)
                                double& outZ                    // RETURNED super sampling factor in Z (1.0 to 0.0625)
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetSuperSamplingFactor) (    // DEPRECATED Set the super sampling factors
                                double inX,                     // super sampling factor in X (1.0 to 0.0625)
                                double inY,                     // super sampling factor in Y (1.0 to 0.0625)
                                double inZ                      // super sampling factor in Z (1.0 to 0.0625)
                            ) = 0;

    enum VLI_DEPRECATED(AccumulationMode) {kGrowBasePlane, kBlendBasePlane};

    virtual VLI_DEPRECATED(AccumulationMode) VLI_DEPRECATED(GetBasePlaneAccumulation) (void // DEPRECATED Return the base plane accumulation mode
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetBasePlaneAccumulation) (      // DEPRECATED Set the base plane accumulation mode
                                VLI_DEPRECATED(AccumulationMode) inMode         // accumulation mode desired
                            ) = 0;

    virtual VLICoordinateSpace VLI_DEPRECATED(GetSuperSamplingSpace) (void  // DEPRECATED Return the super sampling space
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetSuperSamplingSpace) (         // DEPRECATED Set the super sampling space
                                VLICoordinateSpace inSpace      // super sampling space: kVLIObjectSpace or kVLICameraSpace
                            ) = 0;

    // Rendering state

    virtual VLIBlendMode    GetBlendMode (void                  // Return accumulation blend mode
                            ) const = 0;

    virtual VLIStatus       SetBlendMode (                      // Set accumulation blend mode
                                VLIBlendMode inMode             // accumulation blend mode
                            ) = 0;

    virtual VLIBlendMode    GetLastBlendMode (void              // Return last blend mode
                            ) const = 0;

    virtual VLIStatus       SetLastBlendMode (                  // Set last blend mode
                                VLIBlendMode inMode             // last blend mode
                            ) = 0;


    virtual VLIStatus       SetRayTermination (                 // Set early ray termination controls
                                double inThreshold,             // threshold value (0.0 to 1.0)
                                VLIbool inEnabled = VLItrue     // enable or disable, default enable
                            ) = 0;

    virtual void            GetRayTermination (                 // Return early ray termination controls
                                double &outThreshold,           // RETURNED threshold value
                                VLIbool &outEnabled             // RETURNED enable state
                            ) const = 0;

    virtual VLIbool         GetRayTerminationEnabled (void      // Return early ray termination enable state
                            ) const = 0;

    virtual double          GetRayTerminationThreshold (void    // Return early ray termination threshold
                            ) const = 0;

    // Filter range values and controls. To update a depth buffer or blend a sample,
    // all enabled filters must pass.

    virtual VLIStatus       SetGradientRange (                  // Set the gradient range filter
                                double inMinValue,              // minimum value to pass filter
                                double inMaxValue               // maximum value to pass filter
                            ) = 0;

    virtual void            GetGradientRange (                  // Return the gradient range filter
                                double & outMinValue,           // RETURNED minimum value to pass filter
                                double & outMaxValue            // RETURNED maximum value to pass filter
                            ) const = 0;

    virtual double          GetGradientRangeMin (void           // Return the gradient range minimum
                            ) const = 0;

    virtual double          GetGradientRangeMax (void           // Return the gradient range maximum
                            ) const = 0;

    virtual VLIStatus       SetSampleAlphaRange (               // Set the sample alpha range filter
                                double inMinValue,              // minimum value to pass filter
                                double inMaxValue               // maximum value to pass filter
                            ) = 0;

    virtual void            GetSampleAlphaRange (               // Return the sample alpha range filter
                                double & outMinValue,           // RETURNED minimum value to pass filter
                                double & outMaxValue            // RETURNED maximum value to pass filter
                            ) const = 0;

    virtual double          GetSampleAlphaRangeMin (void        // Return the sample alpha range minimum
                            ) const = 0;

    virtual double          GetSampleAlphaRangeMax (void        // Return the sample alpha range maximum
                            ) const = 0;

    virtual VLIStatus       SetAccumulatedAlphaRange (          // Set the accumulated alpha range filter
                                double inMinValue,              // minimum value to pass filter
                                double inMaxValue               // maximum value to pass filter
                            ) = 0;

    virtual void            GetAccumulatedAlphaRange (          // Return the accumulated alpha range filter
                                double & outMinValue,           // RETURNED minimum value to pass filter
                                double & outMaxValue            // RETURNED maximum value to pass filter
                            ) const = 0;

    virtual double          GetAccumulatedAlphaRangeMin (void   // Return the accumulated alpha range minimum
                            ) const = 0;

    virtual double          GetAccumulatedAlphaRangeMax (void   // Return the accumulated alpha range maximum
                            ) const = 0;

    enum FilterTest
    {
            kAlwaysPass             = 0,
            kAlwaysFail             = 3,

            kIgnoreFace             = 0,    // direction: Always pass
            kFrontFace              = 1,    // direction: Pass if front facing
            kBackFace               = 2,    // direction: Pass if back facing
            kFailFace               = 3,    // direction: Never pass

            kGradientMagnitude      = 4,    // If clear, pass. If set, pass if gradient magnitude is in range

            kSampleAlpha            = 8,    // If clear, pass. If set, pass if sample alpha is in range

            kAccumulatedAlpha       = 16,   // If clear, pass. If set, pass if accumulated alpha is in range

            ALLFILTERBITS           = 31
    };

    enum DepthBufferNumber
    {
        kDepthBuffer0,
        kDepthBuffer1
    };

    enum DepthTest
    {
        kDepthTestFail              = 0,
        kDepthTestLess              = 1,        // sample depth less than depth buffer value
        kDepthTestLessEqual         = 2,        // sample depth less than or equal to depth buffer value
        kDepthTestEqual             = 3,
        kDepthTestGreaterEqual      = 4,
        kDepthTestGreater           = 5,
        kDepthTestNotEqual          = 6,
        kDepthTestPass              = 7
    };

    enum DepthCombineFunction
    {
        kDepthCombineAll            = 0,        // Accept all samples; ignore depth tests
        kDepthCombineAnd            = 1,        // Accept samples that pass both depth tests
        kDepthCombineOr             = 2,        // Accept samples that pass either depth test or both
        kDepthCombineXor            = 3         // Accept samples that pass exactly one depth test
    };

    virtual VLIStatus       SetDepthTest (                      // Set the depth test for a buffer
                                DepthBufferNumber inDepthBuffer, // depth buffer to change
                                DepthTest inDepthTest           // depth test
                            ) = 0;

    virtual DepthTest       GetDepthTest (                      // Return the depth test for a buffer
                                DepthBufferNumber inDepthBuffer // depth buffer
                            ) const = 0;

    virtual VLIStatus       SetDepthCombineFunction (           // Set how depth tests are used
                                DepthCombineFunction inFunction // how to use depth tests
                            ) = 0;

    virtual DepthCombineFunction GetDepthCombineFunction (void  // Return how depth tests are used
                            ) const = 0;

    virtual VLIStatus       SetDepthUpdateFilter (              // Set the filter mode for updating a depth buffer
                                DepthBufferNumber inDepthBuffer, // depth buffer (0 or 1)
                                FilterTest inFilterTest         // filter test: bitwise or of FilterTest values
                            ) = 0;

    virtual FilterTest      GetDepthUpdateFilter (              // Return the filter mode for updating a depth buffer
                                DepthBufferNumber inDepthBuffer // depth buffer (0 or 1)
                            ) const = 0;

    virtual VLIStatus       SetSampleFilter (                   // Set the filter mode for excluding samples
                                FilterTest inFilterTest         // filter test: bitwise or of FilterTest values
                            ) = 0;

    virtual FilterTest      GetSampleFilter (                   // Return the filter mode for excluding samples
                            ) const = 0;

    enum ImageBufferNumber
    {
        kImageBuffer0,
        kImageBuffer1,
        kImageBufferNone
    };

    virtual VLIStatus       SetImageOutputBuffer (              // Set the image buffer output switch
                                ImageBufferNumber inBufferNumber // Which buffer should be written
                            ) = 0;

    virtual ImageBufferNumber GetImageOutputBuffer (            // Return the image buffer output switch
                            ) const = 0;

    // Source of gradient computation

    virtual VLIStatus       SetGradientSource (                 // Set the fields used to compute gradient (single)
                                VLIFieldNumber inGradientField  // field used to compute all gradient values
                            ) = 0;

    virtual VLIStatus       SetGradientSource (                     // Set the fields used to compute gradient (multiple)
                                VLIFieldNumber inGradientFieldX,    // field used to compute X gradient values
                                VLIFieldNumber inGradientFieldY,    // field used to compute Y gradient values
                                VLIFieldNumber inGradientFieldZ     // field used to compute Z gradient values
                            ) = 0;

    virtual void            GetGradientSource (                     // Set the fields used to compute gradient (multiple)
                                VLIFieldNumber & outGradientFieldX, // field used to compute X gradient values
                                VLIFieldNumber & outGradientFieldY, // field used to compute Y gradient values
                                VLIFieldNumber & outGradientFieldZ  // field used to compute Z gradient values
                            ) const = 0;

    virtual VLIFieldNumber  GetGradientSourceX (                // Return the field used to compute gradient in X
                            ) const = 0;

    virtual VLIFieldNumber  GetGradientSourceY (                // Return the field used to compute gradient in Y
                            ) const = 0;

    virtual VLIFieldNumber  GetGradientSourceZ (                // Return the field used to compute gradient in Z
                            ) const = 0;

    // Gradient controls -- function, interpolation, and correction

    enum GradientFunction
    {
        kUseField               = 0,    // The specified field contains the gradient (as signed repeating fractions)
        kCentralDifference      = 1,    // Compute gradient using central differences on the given field
        kHalfCentralDifference  = 2     // Compute gradient using half the central difference (useful for gradient fields)
    };

    virtual VLIStatus       SetGradientFunction (               // NOT SUPPORTED Set the gradient function
                                GradientFunction inFunction     // function to set
                            ) = 0;

    virtual GradientFunction GetGradientFunction (void          // NOT SUPPORTED Return the gradient function
                            ) const = 0;

    virtual VLIStatus       SetGradientInterpolationMode (      // Set the interpolation mode for gradients
                                VLIInterpolationMode inMode     // interpolation mode
                            ) = 0;

    virtual VLIInterpolationMode GetGradientInterpolationMode (void     // Return the interpolation mode for gradients
                            ) const = 0;

    virtual VLIbool         GetCorrectGradient (void             // Return gradient correction flag
                            ) const = 0;

    virtual VLIStatus       SetCorrectGradient (                 // Set gradient correction flag
                                VLIbool inOn                     // opacity correction desired
                            ) = 0;

    virtual const double *  GetGradientTable (void              // Return pointer to gradient table
                            ) const = 0;

    virtual VLIStatus       SetGradientTable (                  // Set gradient table
                                const double *inTable           // gradient table (0.0 to 1.0)
                            ) = 0;

    // Trim planes

    virtual VLIStatus       SetTrimPlanes (                     // Set the trim planes (object space)
                                double inMinX,                  // minimum x value
                                double inMaxX,                  // maximum x value
                                double inMinY,                  // minimum y value
                                double inMaxY,                  // maximum y value
                                double inMinZ,                  // minimum z value
                                double inMaxZ                   // maximum z value
                            ) = 0;

    virtual void            GetTrimPlanes (                     // Return the trim planes (object space)
                                double & outMinX,               // RETURNED minimum x value
                                double & outMaxX,               // RETURNED maximum x value
                                double & outMinY,               // RETURNED minimum y value
                                double & outMaxY,               // RETURNED maximum y value
                                double & outMinZ,               // RETURNED minimum z value
                                double & outMaxZ                // RETURNED maximum z value
                            ) const = 0;

    virtual double          GetTrimPlanesMinX (void             // Return the trim plane min X value
                            ) const = 0;

    virtual double          GetTrimPlanesMaxX (void             // Return the trim plane max X value
                            ) const = 0;

    virtual double          GetTrimPlanesMinY (void             // Return the trim plane min Y value
                            ) const = 0;

    virtual double          GetTrimPlanesMaxY (void             // Return the trim plane max Y value
                            ) const = 0;

    virtual double          GetTrimPlanesMinZ (void             // Return the trim plane min Z value
                            ) const = 0;

    virtual double          GetTrimPlanesMaxZ (void             // Return the trim plane max Z value
                            ) const = 0;

    // Event support DEPRECATED

#if VLIAllowDeprecated
    virtual VLIStatus       SetNotifyCallback (                 // DEPRECATED Set the callback for some event type
                                VLIEvent inEvent,               // event type
                                VLIEventCallback inNotify,      // callback routine or kVLIEventNoCallbackRoutine
                                void *inData                    // user data pointer
                            ) = 0;

    static VLIEventCallback kVLIEventNoCallbackRoutine;         // DEPRECATED Special constant to request event queuing

    virtual VLIStatus       WaitForEvent (                      // DEPRECATED Wait for the next event to occur
                                long inTimeoutMilliseconds,     // time to wait, or 0 for wait forever
                                VLIEventBase *&outEvent,        // RETURNED event (must be deleted by caller)
                                void *&outData                  // RETURNED user data pointer
                            ) = 0;
#else // not VLIAllowDeprecated
    // Need to use slots in the vtable
    virtual VLIStatus       SetNotifyCallbackDeprecated (void
                            ) = 0;

    virtual VLIStatus       WaitForEventDeprecated (void
                            ) = 0;
#endif

    // VLI 1.x Rendering support DEPRECATED

    virtual VLIStatus       VLI_DEPRECATED(RenderBasePlane) (   // DEPRECATED ASYNCHRONOUS Render a volume to a base plane buffer
                                VLIVolume *inVolume,            // volume to render
                                int inBuffer,                   // buffer number
                                VLIbool inLeaveOnBoard = VLIfalse // NOT SUPPORTED if VLItrue, don't DMA to buffer
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED(FetchBasePlane) (    // DEPRECATED Fetch a base plane buffer
                                int inBuffer,                   // buffer number
                                int &outBaseWidth,              // RETURNED width of base plane buffer (in pixels)
                                int &outBaseHeight,             // RETURNED height of base plane buffer (in pixels)
                                int &outImageWidth,             // RETURNED width of image in base plane buffer (in pixels)
                                int &outImageHeight,            // RETURNED height of image in base plane buffer (in pixels)
                                VLIPixel *&outBasePlane,        // RETURNED pointer to base plane buffer
                                VLIVector3D outHexGeometry[6],  // RETURNED eye-space geometry of hexagonal volume footprint
                                VLIVector2D outTexCoordinates[6] // RETURNED texture coordinates of hexagonal volume footprint
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED(ReleaseBasePlane) (  // DEPRECATED Release a base plane buffer
                                int inBuffer                    // buffer number
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED(RenderToGraphicsContext) (   // DEPRECATED ASYNCHRONOUS Render volume using graphic context interface
#if VLIAllowDeprecated
                                VLIVolume* inVolume,            // volume to be rendered
                                int inBuffer,                   // buffer number
                                VLIGraphicsContext& inGC        // graphics context object to handle base plane transfer and rendering
#else
                                void
#endif
                            ) = 0;

    virtual VLIStatus       VLI_DEPRECATED(Abort) (             // DEPRECATED Abort specified rendering operation
                                int inBuffer                    // buffer number
                            ) = 0;


    virtual VLIPixelFormat  VLI_DEPRECATED(GetBasePlaneFormat) (void    // DEPRECATED Return the pixel format
                            ) const = 0;

    virtual VLIStatus       VLI_DEPRECATED(SetBasePlaneFormat) (        // DEPRECATED Set the pixel format
                                VLIPixelFormat inFormat         // pixel format
                            ) = 0;

    // Utilities to convert enumerated pixel formats to useful descriptions

    static VLIStatus        DescribePixelFormat (               // Describe the pixel format
                                VLIPixelFormat inFormat,        // pixel format to be described
                                int& outRedWidth,               // RETURNED number of bits of red data
                                int& outRedOffset,              // RETURNED first bit of red data
                                int& outGreenWidth,             // RETURNED number of bits of green data
                                int& outGreenOffset,            // RETURNED first bit of green data
                                int& outBlueWidth,              // RETURNED number of bits of blue data
                                int& outBlueOffset,             // RETURNED first bit of blue data
                                int& outAlphaWidth,             // RETURNED number of bits of alpha data
                                int& outAlphaOffset             // RETURNED first bit of alpha data
                            );

    static VLIStatus        ConvertPixelFormatToFieldDescriptors (  // Describe the pixel format
                                VLIPixelFormat inFormat,            // pixel format to be described
                                VLIFieldDescriptor &outRed,         // RETURNED field descriptor for red data
                                VLIFieldDescriptor &outGreen,       // RETURNED field descriptor for green data
                                VLIFieldDescriptor &outBlue,        // RETURNED field descriptor for blue data
                                VLIFieldDescriptor &outAlpha,       // RETURNED field descriptor for alpha data
                                int & outPixelSize                  // RETURNED size of pixel in bits (16, 32 or 64)
                            );

    // Lighting mode
    enum LightMode
    {
        kBidirectionalSpecular  = 0,
        kUnidirectional         = 2
    };

    virtual VLIStatus       SetLightMode (                      // Set mode for lights
                                LightMode inMode                // light mode to set
                            );

    virtual LightMode       GetLightMode (void                  // Return light mode
                            ) const;

    enum RenderArea
    {
        kViewport,      // 3.0.0 style -- render entire viewport, intersected with output buffers
        kVolume         // Render the volume footprint union with input buffer ranges, intersected 
                        // with viewport and output buffers
    };

    virtual VLIStatus       SetRenderArea (                     // Set area to render
                                RenderArea inArea               // area to render
                            );

    virtual RenderArea      GetRenderArea (void                 // Return area to render
                            ) const;

    virtual VLIStatus Reserved04(void);
    virtual VLIStatus Reserved05(void);

    // Sampling factor control
    // The XY factors apply to perspective rendering only. The Z factors apply to all
    // renders, and are initialized from the Min & MaxZSupersample parameters at context
    // create time. The viewing distance also only applies to perspective.

    virtual VLIStatus       SetSamplingLimits (                 // Set sampling limits
                                double inMinXY,                 // min XY sampling factor
                                double inMaxXY,                 // max XY sampling factor
                                double inMinZ,                  // min Z sampling factor
                                double inMaxZ                   // max Z sampling factor
                            );

    virtual void            GetSamplingLimits (                 // Return sampling limits
                                double& outMinXY,               // RETURNED min XY sampling factor
                                double& outMaxXY,               // RETURNED max XY sampling factor
                                double& outMinZ,                // RETURNED min Z sampling factor
                                double& outMaxZ                 // RETURNED max Z sampling factor
                            ) const;

    virtual VLIStatus       SetMinViewingDistance (             // Set min camera to volume distance
                                double inDistance               // distance in voxels
                            );

    virtual double          GetMinViewingDistance (void         // Return min camera to volume distance
                            ) const;


    enum SpecularColorOpacityWeighting
    {
        kDisabled,
        kEnabled,
        kBasedOnBlendMode
    };
    virtual VLIStatus SetSpecularColorOpacityWeighting(SpecularColorOpacityWeighting inColorOpacityWeighting);
    virtual SpecularColorOpacityWeighting   GetSpecularColorOpacityWeighting(void) const;

    virtual VLIStatus Reserved12(void);
    virtual VLIStatus Reserved13(void);
    virtual VLIStatus Reserved14(void);
    virtual VLIStatus Reserved15(void);
    virtual VLIStatus Reserved16(void);
    virtual VLIStatus Reserved17(void);
    virtual VLIStatus Reserved18(void);
    virtual VLIStatus Reserved19(void);
    virtual VLIStatus Reserved20(void);
    virtual VLIStatus Reserved21(void);
    virtual VLIStatus Reserved22(void);
    virtual VLIStatus Reserved23(void);
    virtual VLIStatus Reserved24(void);
    virtual VLIStatus Reserved25(void);
    virtual VLIStatus Reserved26(void);
    virtual VLIStatus Reserved27(void);
    virtual VLIStatus Reserved28(void);
    virtual VLIStatus Reserved29(void);


protected:
                            VLIContext (void) {}
    virtual                 ~VLIContext (void) {}


};

// bitwise or, and and invert for VLIContext::FilterTest

inline VLIContext::FilterTest operator | (VLIContext::FilterTest a, VLIContext::FilterTest b)
{ return VLIContext::FilterTest ( (unsigned int) (a) | (unsigned int) (b) ); }

inline VLIContext::FilterTest operator & (VLIContext::FilterTest a, VLIContext::FilterTest b)
{ return VLIContext::FilterTest ( (unsigned int) (a) & (unsigned int) (b) ); }

inline VLIContext::FilterTest operator ~ (VLIContext::FilterTest a)
{ return VLIContext::ALLFILTERBITS & VLIContext::FilterTest ( ~ (unsigned int) (a) ); }

inline VLIContext::FilterTest & operator |= (VLIContext::FilterTest &a, VLIContext::FilterTest b)
{ a = a | b; return a; }

inline VLIContext::FilterTest & operator &= (VLIContext::FilterTest &a, VLIContext::FilterTest b)
{ a = a & b; return a; }


#undef VLI_DEPRECATED
#undef VLI_OBSOLETE

}   // end of namespace vli3


/******************************** END OF FILE *********************************/

#endif /* VLI3_H */
