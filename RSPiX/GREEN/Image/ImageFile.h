////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 RWS Inc, All Rights Reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of version 2 of the GNU General Public License as published by
// the Free Software Foundation
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//////////////////////////////////////////////////////////////////////////////
//
// IMAGEFILE.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef IMAGEFILE_H
#define IMAGEFILE_H

//////////////////////////////////////////////////////////////////////////////
// C Headers -- Must be included before RSPiX.h b/c RSPiX utilizes SHMalloc.
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// RSPiX Headers.
// If PATHS_IN_INCLUDES macro is defined, we can utilize relative
// paths to a header file.  In this case we generally go off of our
// RSPiX root directory.  System.h MUST be included before this macro
// is evaluated.  System.h is the header that, based on the current
// platform (or more so in this case on the compiler), defines
// PATHS_IN_INCLUDES.  Blue.h includes system.h so you can include that
// instead.
///////////////////////////////////////////////////////////////////////////////
#include "System.h"

#ifdef PATHS_IN_INCLUDES
   #include "GREEN/Image/Image.h"
#else
   #include "Image.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Protos.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Typedefs.
//////////////////////////////////////////////////////////////////////////////
class RImageFile
{
///////////////////////////////////////////////////////////////////////////
// Con/Destruction.
///////////////////////////////////////////////////////////////////////////
public:

///////////////////////////////////////////////////////////////////////////
// Methods.
///////////////////////////////////////////////////////////////////////////
public:

// Maps a particular file load onto the appropriate function, if available.
static short Load(         // Returns SUCCESS on success or FAILURE on failure.
   RImage*   pim,           // Image to load into.
   RFile*   pfile);        // File to load from.

protected:

// Loads an RImage with file version 1 into pim from pfile.
static short LoadVersion1(       // Returns SUCCESS on success or FAILURE on
                                 // failure.
   RImage*   pim,                 // Image to load into.
   RFile*   pfile);              // File to load from.

// Loads an RImage with file version 2 into pim from pfile.
static short LoadVersion2(       // Returns SUCCESS on success or FAILURE on
                                 // failure.
   RImage*   pim,                 // Image to load into.
   RFile*   pfile);              // File to load from.

// Loads an RImage with file version 3 into pim from pfile.
static short LoadVersion3(       // Returns SUCCESS on success or FAILURE on
                                 // failure.
   RImage*   pim,                 // Image to load into.
   RFile*   pfile);              // File to load from.

// Loads an RImage with file version 4 into pim from pfile.
static short LoadVersion4(       // Returns SUCCESS on success or FAILURE on
                                 // failure.
   RImage*   pim,                 // Image to load into.
   RFile*   pfile);              // File to load from.

// Loads an RImage with file version 5 into pim from pfile.
static short LoadVersion5(       // Returns SUCCESS on success or FAILURE on
                                 // failure.
   RImage*   pim,                 // Image to load into.
   RFile*   pfile);              // File to load from.

///////////////////////////////////////////////////////////////////////////
// Querries.
///////////////////////////////////////////////////////////////////////////
public:


///////////////////////////////////////////////////////////////////////////
// Instantiable data.
///////////////////////////////////////////////////////////////////////////
public:

///////////////////////////////////////////////////////////////////////////
// Static data.
///////////////////////////////////////////////////////////////////////////
public:
};

#endif   // IMAGEFILE_H
//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
