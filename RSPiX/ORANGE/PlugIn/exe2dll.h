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
#ifndef PLUGIN_H
#define PLUGIN_H

///////////////////////////////////////////////////////////////////////////////
// Includes.
///////////////////////////////////////////////////////////////////////////////
#include "PlugIn/EXE2DLL.H"
#include "Image/Image.h"
#include "MFC_CNTL/RSPXBlue.h"

///////////////////////////////////////////////////////////////////////////////
// Macros.
///////////////////////////////////////////////////////////////////////////////
#define DllExport   __declspec( dllexport )
#define DllImport   __declspec( dllimport )

#ifdef _WINDLL
   #define DLL2EXE   DllExport
   #define EXE2DLL   DllImport
#else
   #define DLL2EXE   DllImport
   #define EXE2DLL   DllExport
#endif   // _WINDLL

///////////////////////////////////////////////////////////////////////////////
// Typedefs.
///////////////////////////////////////////////////////////////////////////////