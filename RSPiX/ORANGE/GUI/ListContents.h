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
// ListContents.H
//
// History:
//      01/21/97 JMI   Started.
//
//      02/05/97   JMI   Added override for OnLoseChild() to pass it on to
//                     parent, if it is an RListBox.
//
//////////////////////////////////////////////////////////////////////////////
//
// This is a very simple class designed to be used as a container specifically
// for use with an RListBox.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef LISTCONTENTS_H
#define LISTCONTENTS_H

//////////////////////////////////////////////////////////////////////////////
// Please see the CPP file for an explanation of this API.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Headers.
//////////////////////////////////////////////////////////////////////////////

#include "System.h"
// If PATHS_IN_INCLUDES macro is defined, we can utilized relative
// paths to a header file.  In this case we generally go off of our
// RSPiX root directory.  System.h MUST be included before this macro
// is evaluated.  System.h is the header that, based on the current
// platform (or more so in this case on the compiler), defines
// PATHS_IN_INCLUDES.  Blue.h includes system.h so you can include that
// instead.
#ifdef PATHS_IN_INCLUDES
   #include "ORANGE/GUI/Frame.h"
#else
   #include "frame.h"
#endif // PATHS_IN_INCLUDES

//////////////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Typedefs.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
class RListContents : public RFrame
{
public:     // Construction/Destruction.
// Default constructor.
RListContents(void)
{}

// Destructor.
~RListContents(void)
{}

//////////////////////////////////////////////////////////////////////////////

public:     // Methods.

////////////////////////////////////////////////////////////////////////
// Methods.
////////////////////////////////////////////////////////////////////////

// Called by SetParent() when a GUI is losing a child item.
virtual                 // Overridden here.
void OnLoseChild(                // Returns nothing.
   RGuiItem*   pguiChild)        // Child item we're about to lose.
{
   // Let parent (RListBox know).
   RGuiItem*   pguiParent   = GetParent();
   // If there is a parent (safety) . . .
   if (pguiParent != NULL)
   {
      // If it is a listbox (should be) . . .
      if (pguiParent->m_type == ListBox)
      {
         pguiParent->OnLoseChild(pguiChild);
      }
   }
}

////////////////////////////////////////////////////////////////////////
// Querries.
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

public:     // Static

//////////////////////////////////////////////////////////////////////////////

public:     // Querries.

//////////////////////////////////////////////////////////////////////////////

protected:     // Internal functions.

// Save item's children to the specified file.
virtual                    // Overridden here.
short SaveChildren(        // Returns 0 on success.
   RFile*   pfile);        // File to save to.

// Load item's children from the specified file.
virtual                    // Overridden here.
short LoadChildren(        // Returns 0 on success.
   RFile*   pfile);        // File to load from.

//////////////////////////////////////////////////////////////////////////////

public:     // Member variables.

protected:     // Internal typedefs.

protected:     // Protected member variables.

};

#endif // LISTCONTENTS_H
//////////////////////////////////////////////////////////////////////////////
//   EOF
//////////////////////////////////////////////////////////////////////////////
