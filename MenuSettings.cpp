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
// MenuSettings.cpp
// Project: Nostril (aka Postal)
//
// History:
//		06/11/97 JMI	Started.
//
//		07/18/97	JMI	Added more menus to those scanned.
//
//		07/20/97	JMI	Added menuVideoOptions, menuAudioOptions, and
//							menuPlayOptions.
//
//		08/04/97	JMI	Added menuRotation.
//
//		08/27/97	JMI	Now section names are "Menu " instead of "Menu_" since
//							RPrefs now supports spaces in vars and sections.
//
//		06/28/01 MJR	Added menuChallenge.
//
//////////////////////////////////////////////////////////////////////////////
//
// Implementation for CMenuSettings object.  Each instance contains settings
// for Postal.
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// C Headers -- Must be included before RSPiX.h b/c RSPiX utilizes SHMalloc.
//////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// RSPiX Headers.
///////////////////////////////////////////////////////////////////////////////
#include "RSPiX.h"

///////////////////////////////////////////////////////////////////////////////
// Postal Headers.
///////////////////////////////////////////////////////////////////////////////

#include "MenuSettings.h"
#include "menus.h"

//////////////////////////////////////////////////////////////////////////////
// Module specific macros.
//////////////////////////////////////////////////////////////////////////////

// Determines the number of elements in the passed array at compile time.
#define NUM_ELEMENTS(a)		(sizeof(a) / sizeof(a[0]) )


//////////////////////////////////////////////////////////////////////////////
// Module specific typedefs.
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Exported (extern) variables.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Module specific (static) variables / Instantiate class statics.
//////////////////////////////////////////////////////////////////////////////

// Items pointed to by elements of this array are the ones that are checked
// for settings.
static Menu*	ms_apmenus[]	=
{
   &menuMain,
   &menuClientGame,
   &menuEditor,
   &g_menuVerifyQuitGame,
   &menuStart,
   &menuStartSingle,
   &menuStartMulti,
   &menuStartDemo,
   &menuChallenge,
   &menuOptions,
   &menuControls,
   &menuKeyboard,
   &menuMouse,
   &menuJoystick,
   &menuVerifyExit,
   &menuMultiOptions,
   &menuFeatures,
   &menuChallenge,
   &menuVolumes,
   &menuVideoOptions,
   &menuAudioOptions,
   &menuPlayOptions,
   &menuRotation,
};

//////////////////////////////////////////////////////////////////////////////
// Module specific (static) protos.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Functions.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Set settings to default values
//////////////////////////////////////////////////////////////////////////////
CMenuSettings::CMenuSettings(void)
{}

//////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////
CMenuSettings::~CMenuSettings()
{}

//////////////////////////////////////////////////////////////////////////////
// Read settings that are stored in preference file
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::LoadPrefs(
   RPrefs* pPrefs)
{
   short sResult = 0;

   // Check for entries for all our menus.
   short	sMenu;
   short	sMenuItem;
   char szSection[256];
   for (sMenu = 0; sMenu < NUM_ELEMENTS(ms_apmenus); sMenu++)
   {
      // Create section name.
      sprintf(szSection, "Menu %s", ms_apmenus[sMenu]->menuheader.pszHeaderText);

      for (sMenuItem = 0; ms_apmenus[sMenu]->ami[sMenuItem].pszText != NULL; sMenuItem++)
      {
         // Check for var name.
         pPrefs->GetVal(
            szSection,                                         // In:  Section.
            ms_apmenus[sMenu]->ami[sMenuItem].pszText,         // In:  Var.
            ms_apmenus[sMenu]->ami[sMenuItem].sEnabled,        // In:  Default.
            &(ms_apmenus[sMenu]->ami[sMenuItem].sEnabled) );   // Out: Value.
      }
   }

   if (!sResult)
   {
      if (pPrefs->IsError())
         sResult = -1;
   }

   return sResult;
}

//////////////////////////////////////////////////////////////////////////////
// Write settings that are stored in preference file
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::SavePrefs(
   RPrefs* pPrefs)
{
   return pPrefs->IsError();
}

//////////////////////////////////////////////////////////////////////////////
// Load settings that are stored in game file
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::LoadGame(
   RFile* pFile)
{
   return 0;
}


//////////////////////////////////////////////////////////////////////////////
// Save settings that are stored in game file
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::SaveGame(
   RFile* pFile)
{
   return 0;
}


//////////////////////////////////////////////////////////////////////////////
// Temporarily set settings for demo mode (file is for saving current settings)
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::PreDemo(
   RFile* pFile)
{
   return 0;
}


//////////////////////////////////////////////////////////////////////////////
// Restore settings to what they were prior to demo mode
//////////////////////////////////////////////////////////////////////////////
short CMenuSettings::PostDemo(
   RFile* pFile)
{
   return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Internal functions.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
