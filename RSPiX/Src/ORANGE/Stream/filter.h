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
#ifndef FILTER_H
#define FILTER_H

//////////////////////////////////////////////////////////////////////////////
// Headers.
//////////////////////////////////////////////////////////////////////////////
#include "filewin.h"
#include "list.h"

//////////////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Typedefs.
//////////////////////////////////////////////////////////////////////////////

// Encapsulate our buffer and info.
typedef struct
{
   UCHAR*	puc;        // Beginning of chunk.
   S32 lSize;           // Total size of chunk (puc).
   USHORT usType;       // Type of buffer.
   UCHAR	ucFlags;       // Flags for buffer.
   S32 lId;             // Id of buffer.
   S32 lTime;           // Time buffer is supposed to arrive.
   S32 lPos;            // Position for next piece.
} RTCHUNK, *PRTCHUNK;

// This type is used to call the user to allow them to allocate space for the
// data and pass it back to be filled.
typedef UCHAR* (*ALLOC_FILTERFUNC)(	S32 lSize, USHORT usType, UCHAR ucFlags,
                                    S32 lUser);

// This type is used to call the user to allow them to DEallocate space al-
// located by a previous call to their ALLOC_FILTERFUNC.
typedef void (*FREE_FILTERFUNC)(	UCHAR* puc, USHORT usType, UCHAR ucFlags,
                                 S32 lUser);

// This type is used to pass the copied chunk to the user ready to be used.
typedef void (*USE_FILTERFUNC)(	UCHAR* puc, S32 lSize, USHORT usType,
                                 UCHAR ucFlags, S32 lTime, S32 lUser);

class CFilter
{
public:        // Construction/Destruction.
// Default constructor.
CFilter();
// Destructor.
~CFilter();

public:        // Methods.
void SetFilter(U32 ulFilterMask)
{ m_ulFilter = ulFilterMask; }

void SetFileWin(CFileWin* pfw)
{
   m_pfw = pfw;
   if (pfw != NULL)
   {
      // Point callback at our CFileWin callback dispatcher (calls
      // implied this version (WinCall)).
      m_pfw->m_call			= (FWFUNC)WinCallStatic;
      m_pfw->m_lUser			= (S32)this;
   }
}

// Resets members. Deallocates memory if necessary.
void Reset(void);

public:        // Querries.
// Returns the current filter mask.
U32 GetFilter(void)
{ return m_ulFilter; }

protected:     // Internal methods.

// Sets members w/o regard for current value.
void Set(void);
// Handles callbacks from file window.
void WinCall(PPANE ppane);

// Callback dispatcher (calls the implied this version).
static void WinCallStatic(PPANE ppane, CFilter* pFilter);


// Returns ptr to chunk via lId, returns NULL if not found.
PRTCHUNK GetChunk(S32 lId);

// Add a chunk header.
// Returns chunk on success, NULL otherwise.
PRTCHUNK AddChunk(S32 lSize, USHORT usType, UCHAR ucFlags, S32 Id,
                  S32 lTime);

// Removes a chunk header.
// Returns 0 on success.
short RemoveChunk(PRTCHUNK pChunk);

// Adds a buffer to a chunk.
// Returns amount added.
S32 AddToChunk(CNFile* pfile, S32 lBufSize);

// Allocates data via user callback if defined or malloc, otherwise.
// Returns 0 on success.  See comment of this function in filter.cpp
// for greater details.
short AllocChunk(UCHAR** ppuc, S32 lSize, USHORT usType, UCHAR ucFlags);
// Deallocates data via user callback if defined or free if both
// m_fnAlloc AND m_fnFree are NOT defined.
void FreeChunk(UCHAR* puc, USHORT usType, UCHAR ucFlags);


public:        // Members.
ALLOC_FILTERFUNC m_fnAlloc;            // Where to ask for data allocation.
FREE_FILTERFUNC m_fnFree;              // Where to ask for data DEallocation.
USE_FILTERFUNC	m_fnUse;                // Where to pass completed chunks.
S32 m_lUser;                           // User defined value.


protected:     // Members.
U32 m_ulFilter;                        // Channels allowed to pass.
S32 m_lPadSize;                        // Size of current padding.
S32 m_lBufRemaining;                   // Amount of current buffer remaining.
PRTCHUNK	m_pChunk;                     // Current chunk.
CFileWin*	m_pfw;                     // File window.
CList	<RTCHUNK>	m_listPartial;       // List of partial buffers.
};


#endif // FILTER_H
//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
