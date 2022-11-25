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
// You should have received a copy of the GNU General Public License aS32
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef MFCOOL_H
#define MFCOOL_H

///////////////////////////////////////////////////////////////////////////////
// Includes.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Macros.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Protos.
///////////////////////////////////////////////////////////////////////////////
// Associates all icon style buttons that are children of pdlg with the icon
// specified by the window text of that button.
// Returns 0 on success.
extern short Iconize(CDialog* pdlg);

// Store position of this window in the module's INI.
// Returns 0 on success.
extern short StorePosition(CWnd* pwnd);

// Restore position of this window from the module's INI.  No change if no
// settings stored.
// Returns 0 on success.
extern short RestorePosition(CWnd* pwnd);

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
#endif   // MFCOOL_H
