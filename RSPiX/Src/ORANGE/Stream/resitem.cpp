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
//////////////////////////////////////////////////////////////////////////////
//
// RES.CPP
//
// History:
//      09/22/95 JMI   Started.
//
//////////////////////////////////////////////////////////////////////////////
//
// This class stores data identified by a string.
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// C Headers.
//////////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////
// Blue Headers.
//////////////////////////////////////////////////////////////////////////////
#include "System.h"
#include "bdebug.h"

//////////////////////////////////////////////////////////////////////////////
// Green Headers.
//////////////////////////////////////////////////////////////////////////////
#include "resitem.h"

//////////////////////////////////////////////////////////////////////////////
// Orange Headers.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Yellow Headers.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Module specific macros.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Module specific typedefs.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Module specific (static) variables.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction Functions.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Default constructor.
//
//////////////////////////////////////////////////////////////////////////////
CResItem::CResItem()
{
   m_pszName   = NULL;
   m_puc         = NULL;
   m_lSize      = 0L;
   m_sRefCnt   = 0;
   m_pRes      = NULL;
}

//////////////////////////////////////////////////////////////////////////////
//
// Constructura Especial! Ole!
//
//////////////////////////////////////////////////////////////////////////////
CResItem::CResItem(char* pszName, UCHAR* puc, S32 lSize, CRes* pRes)
{
   m_pszName   = pszName;
   m_puc         = puc;
   m_lSize      = lSize;
   m_sRefCnt   = 0;
   m_pRes      = pRes;
}


//////////////////////////////////////////////////////////////////////////////
//
// Destructor.
//
//////////////////////////////////////////////////////////////////////////////
CResItem::~CResItem()
{
   if (m_sRefCnt != 0)
   {
      TRACE("~CResItem(): Destroyed with reference count of %d!\n",
            m_sRefCnt);
   }

   if (m_pszName != NULL)
   {
      // Remember that the name is the ptr that was allocated and includes
      // the resource data (where m_puc points).
      free(m_pszName);
      m_pszName   = NULL;
      m_puc         = NULL;
   }
}

//////////////////////////////////////////////////////////////////////////////
// Methods.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Lock this item b4 using.
// Returns new reference count.
//
//////////////////////////////////////////////////////////////////////////////
short CResItem::Lock(void)
{
   return ++m_sRefCnt;
}

//////////////////////////////////////////////////////////////////////////////
//
// Unlock this item when done.  Deletes object if m_sRefCnt hits 0.
// Returns new reference count.
//
//////////////////////////////////////////////////////////////////////////////
short CResItem::Unlock(void)
{
   ASSERT(m_sRefCnt > 0);

   return --m_sRefCnt;
}

//////////////////////////////////////////////////////////////////////////////
// Internal Functions.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
