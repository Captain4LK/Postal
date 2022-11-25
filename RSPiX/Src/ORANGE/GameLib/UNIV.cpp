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
//*****************************************************************************
//
// UNIV.CPP
//
// History:
//      01/26/95 BH      Started work on the animation loading functions
//
//*****************************************************************************
//
//   This file contains functions that are used to set up a particular world,
//   stage and section of the game.  It includes functions that load data 
//   generated by the linker into memory where they can be used during a 
//   particular level of the game.
//   
//*****************************************************************************
               
#include <stdio.h>
#include <windows.h>
#include <fstream.h>
#include <assert.h>
#include "univ.h"
#include "rifflib.h"

//*****************************************************************************
//
// Default Constructor
// 
//*****************************************************************************

CUniverse::CUniverse()
{
//   m_strFileGame = "h:\\zootopia\\code\\library\\zoo.gme";
//   m_strFileAnim = "h:\\zootopia\\code\\library\\zoo.anm";
//   m_strFileBack = "h:\\zootopia\\code\\library\\zoo.bg";
//   m_strFileAPal = "h:\\zootopia\\code\\library\\zoo.apl";
//   m_strFileBPal = "h:\\zootopia\\code\\library\\zoo.bpl";

   m_pSection = NULL;
   m_pAnimData = NULL;
   m_pBackData = NULL;
   m_pBackgroundPal = NULL;
   m_pSpritePal = NULL;
   m_pAttrMap = NULL;
   m_pAttrCont = NULL;
   m_pEventData = NULL;
   m_pImageData = NULL;
   sprintf(m_strDataPath, "c:\\w\\rspix\\yellow\\sprtest\\");
}

//*****************************************************************************
//
// Default Destructor
//
//*****************************************************************************

CUniverse::~CUniverse()
{
   if (m_pSection != NULL)
      delete m_pSection;
//   free(m_pvImageChunks);

}

//*****************************************************************************
//
// SetSection
//
// Description:
//      This function opens the game info file and reads a SECTION structure
//      from the file into memory and sets the member m_pSection to point to
//      this structure.  Other modules can then call member functions such
//      as GetSpritePal or GetAnimBuildData to get information from the
//      current Section structure.
//
// Input:
//      sWorld = Current world number which contains desired section to load
//      sStage = Current section within world containing desired section to load
//      sSection = Section number to load
//
// Uses:
//      Uses the filename members to open the files and then assigns correct
//      values to the pointer members
//
// Output:
//      none
//
// Return:
//      0 if all files were opened and read successfully
//      ERROR code if there was a problem opening file
//
//*****************************************************************************

#if 0

short CUniverse::SetSection(
         short sWorld,         // World to load
         short sStage,         // Stage to load
         short sSection)      // Section to load
{
    FILE* fp;
   short   sNumEntries;
   S32   lOffset;

   // open the game information file
   if (!(fp = fopen(m_strFileGame, "rb")))
      return UNIVERSE_CANT_OPEN_FILE;

//-----------------------------------------------------------------------------
// universe
//      dw n
//      da world0
//      da worldn
//-----------------------------------------------------------------------------
   // Get number of worlds and check validity of user's selection
   fread(&sNumEntries, sizeof(short), 1, fp);   
   if (sNumEntries > sWorld || sWorld < 0)
      return UNIVERSE_INVALID_WORLD;

   // Get offset to world block (list of stage pointers)
   int i;
   for (i = 0; i <= sWorld; i++)
       fread(&lOffset, sizeof(void*), 1, fp);         // get offset for world
   
   // Go to list of stage pointers
   if (fseek(fp, lOffset, SEEK_SET) != 0)
      return UNIVERSE_OFFSET_ERROR;

//------------------------------------------------------------------------------
// world0
//      dw n
//      da stage0
//      da stage1
//      da stagen
//------------------------------------------------------------------------------
   // Get number of stages in this world and check validity of user's selection
   fread(&sNumEntries, sizeof(short), 1, fp);
   if (sNumEntries > sStage || sStage < 0)
      return UNIVERSE_INVALID_STAGE;

   // Get offset to stage block (list of section pointers)
   for (i = 0; i <= sStage; i++)
      fread(&lOffset, sizeof(void*), 1, fp);
   
   // Go to list of section pointers
   if (fseek(fp, lOffset, SEEK_SET) != 0)
      return UNIVERSE_OFFSET_ERROR;

//------------------------------------------------------------------------------
// stage0
//      dw   n
//      da   stage0section0
//    da stage0section1
//      da stage0sectionn
//------------------------------------------------------------------------------
   // Get number of sections in stage and check validity of user's selection
   fread(&sNumEntries, sizeof(short), 1, fp);
   if (sNumEntries > sSection || sSection < 0)
      return UNIVERSE_INVALID_SECTION;

   // Get offset to section structures (data)
   for (i = 0; i <= sSection;i++)
      fread(&lOffset, sizeof(void*), 1, fp);

   // Go to desired section structure
   if (fseek(fp, lOffset, SEEK_SET) != 0)
      return UNIVERSE_OFFSET_ERROR;

//------------------------------------------------------------------------------
// allocate memory for the section structure and read the data from the file
//------------------------------------------------------------------------------

   // don't need structure, just get file names and put into filename members
//   m_pSection = new SECTION;
//   fread(m_pSection, sizeof(SECTION), 1, fp);

   fread(m_strFileAPal, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileAnim, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileBPal, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileBack, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileAttr, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileEvent, sizeof(char), UNIVERSE_MAX_FILENAME, fp);
   fread(m_strFileImage, sizeof(char), UNIVERSE_MAX_FILENAME, fp);

   LoadAnimData();
   LoadBackgroundData();
   m_pSpritePal = LoadPalette(m_strFileAPal);
   m_pBackgroundPal = LoadPalette(m_strFileBPal);
   LoadAttributeData();
   LoadEventData();

   fclose(fp);

   return 0;
}

#endif

//*****************************************************************************
//
// New file format
//
//*****************************************************************************

short CUniverse::SetSection(
   short sWorld,               // World to load
   short sStage,               // Stage to load
   short sSection)            // Section to load
{

    fstream fsGameInfo("zoo.gme", ios::in, filebuf::sh_read);
   if (!fsGameInfo.is_open())
      return UNIVERSE_CANT_OPEN_FILE;

   char  strLine[UNIVERSE_MAXLINE];      // buffer for reading
   char  strLevelName[UNIVERSE_MAXLINE];

   sprintf(strLevelName, "level%.2x%.2x%.2x", sWorld, sStage, sSection);

//   fsGameInfo.getline(strLine, UNIVERSE_MAXLINE, '\n');

   do
   {
       fsGameInfo.getline(strLine, UNIVERSE_MAXLINE, '\n');
   }
   while (stricmp(strLine, strLevelName) != 0 && !fsGameInfo.eof());

   if (fsGameInfo.eof())
      return UNIVERSE_INVALID_LEVEL;

   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileAnim, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileAPal, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileImage, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileZone, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileBack, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileBPal, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileAttr, UNIVERSE_MAXLINE);
   fsGameInfo.eatwhite();
   fsGameInfo.getline(m_strFileEvent, UNIVERSE_MAXLINE);

   short sReturnValue;

   if (sReturnValue = LoadAnimData())
      return sReturnValue;
//   LoadBackgroundData();
//   m_pSpritePal = LoadPalette(m_strFileAPal);
//   m_pBackgroundPal = LoadPalette(m_strFileBPal);
//   LoadAttributeData();
//   LoadEventData();

   return 0;
}

//*****************************************************************************
//
// LoadAnimData
//
//   Description:
//      Private function to load the animation data file into memory for a
//      section selected by SetSection().  All of the data from the animation
//      file for this section will be loaded into memory.  The offsets that
//      are stored in the data file will be converted into pointers.
//
// Input:
//      none
//
//   Uses:
//      m_strFileAnim to get the animation build data filename
//      m_pAnimData gets set to the ANIM structure in memory
//
// Output:
//      none
//
//   Return:
//      0 if the file was loaded correctly
//      ERROR code if it was not loaded correctly
//
//*****************************************************************************

short CUniverse::LoadAnimData()
{
   char    strPathName[UNIVERSE_MAX_PATHNAME];
   CRiff AnimFile;
   
   sprintf(strPathName, "%s%s", m_strDataPath, m_strFileAnim);
   if (AnimFile.Open(strPathName, READ))
      return UNIVERSE_CANT_OPEN_FILE;

   CRiff ImageFile;
   sprintf(strPathName, "%s%s", m_strDataPath, m_strFileImage);
   if (ImageFile.Open(strPathName, READ))
      return UNIVERSE_CANT_OPEN_FILE;


   CHUNK* pChunkHeader = (CHUNK*) malloc(sizeof(CHUNK));
   AnimFile.ReadHeader(pChunkHeader);

   if (pChunkHeader->dwType != CRiff::StringToCode("ANST"))
   {
      free(pChunkHeader);
      return UNIVERSE_WRONG_FILETYPE;
   }

     ANIMSET* pAnimSet = (ANIMSET*) malloc(sizeof(ANIMSET) + 
                                           (pChunkHeader->dwSub - 1) * sizeof(ANIM*));

   pAnimSet->lNumAnims = pChunkHeader->dwSub;

   AnimFile.DescendChunk();

   S32 lAnim;
   short sFrame;

   for (lAnim = 0; lAnim < pAnimSet->lNumAnims; lAnim++)
   {
      AnimFile.ReadHeader(pChunkHeader);
      pAnimSet->apAnims[lAnim] = (ANIM*) malloc(pChunkHeader->dwSize);
      AnimFile.ReadData((void*) pAnimSet->apAnims[lAnim]);

      for (sFrame = 0; sFrame < pAnimSet->apAnims[lAnim]->sNumFrames; sFrame++)
      {
          ImageFile.SetCurrentChunk((S32) pAnimSet->apAnims[lAnim]->aFrames[sFrame].pImage);
         ImageFile.ReadHeader(pChunkHeader);
         pAnimSet->apAnims[lAnim]->aFrames[sFrame].pImage = 
            (IMAGE*) malloc(pChunkHeader->dwSize);
         ImageFile.ReadData((void*) pAnimSet->apAnims[lAnim]->aFrames[sFrame].pImage);

         //ZoneFile.SetCurrentChunk(pAnimSet->apAnims[lAnim]->aFrames[sFrame].pZoneSet);
         //ZoneFile.ReadHeader(pChunkHeader);
         //pAnimSet->apAnims[lAnim]->aFrames[sFrame].pZoneSet =
         //   (ZONESET*) malloc(pChunkHeader->dwSize);
         //ZoneFile.ReadData((void*) pAnimSet->apAnims[lAnim]->aFrames[sFrame].pZoneSet);
      }      

   }

    free(pChunkHeader);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
// alternate version
///////////////////////////////////////////////////////////////////////////////

#if 0

short CUniverse::LoadAnimData()
{
   char    strPathName[UNIVERSE_MAX_PATHNAME];
   CRiff AnimFile;
   
   sprintf(strPathName, "%s%s", m_strDataPath, m_strFileAnim);
   if (AnimFile.Open(strPathName, READ)
      return UNIVERSE_CANT_OPEN_FILE;

   CRiff ImageFile;
   sprintf(strPathName, "%s%s", m_strDataPath, m_strFileImage);
   if (ImageFile.Open(strPathName, READ)
      return UNIVERSE_CANT_OPEN_FILE;

   CHUNK* pChunkHeader = (CHUNK*) malloc(sizeof(CHUNK));

   // Read in all of the pictures
   ImageFile.ReadHeader(pChunkHeader);
   m_pvImageChunks = malloc(sizeof(pChunkHeader->dwSize));
   ImageFile.ReadData(m_pvImageChunks);


   AnimFile.ReadHeader(pChunkHeader);

   if (pChunkHeader->dwType != CRiff::StringToCode("ANST"))
   {
      free(pChunkHeader);
      return UNIVERSE_WRONG_FILETYPE;
   }

     ANIMSET* pAnimSet = (ANIMSET*) malloc(sizeof(ANIMSET) + 
                                           (pChunkHeader->dwSub - 1) * sizeof(ANIM*));

   pAnimSet->lNumAnims = pChunkHeader->dwSub;

   AnimFile.DescendChunk();

   S32 lAnim;
   short sFrames;

   for (lAnim = 0; lAnim < pAnimSet->lNumAnims; lAnim++)
   {
      AnimFile.ReadHeader(pChunkHeader);
      pAnimSet->apAnims[lAnim] = (ANIM*) malloc(pChunkHeader->dwSize);
      AnimFile.ReadData((void*) pAnimSet->apAnims[lAnim]);

      for (sFrames = 0; sFrames < pAnimSet->apAnims[lAnim]->sNumFrames; sFrames++)
      {
         pAnimSet->apAnims[lAnim]->aFrames[sFrame].pImage += m_pvImageChunks;
         //pAnimSet->apAnims[lAnim]->aFrames[sFrame].pZoneSet += m_pvZoneChunks;
      }      




   }
   free (pChunkHeader);
   return 0;
}


#endif

//*****************************************************************************
//
// SetAnimOffsets
//
// Description:
//      This function is called by LoadAnimData to resolve the pointers of
//      a particular animation.  This function use the ANIM info to traverse
//      the array of FRAME structures that follow.  Within the FRAME structure
//      there are two fields that need to be changed from offsets within the
//      data file, to actual memory pointers.  The base pointer for the image
//      data will be added to the pImage field, and the base pointer for the
//      animation data will be added to the pZoneSet field.
//
// Input:
//      ANIM* pAnimation = a pointer to an ANIM structure to be updated
//
// Uses:
//      m_pAnimData pointer as the base pointer to which the pZoneSet offset
//                  will be added
//      m_pImageData pointer as teh base poniter to which the pImage offset
//                  will be added
//
// Output:
//      none
//
// Return:
//      none
//
// Note:
//      In the file, offset values of 0 indicate that the pointer should
//      be NULL.  For example if a particular frame does not use an Image
//      then the offset for the pImage field will be 0 and the pointer
//      pImage should remain 0 (NULL).
//
//*****************************************************************************

void CUniverse::SetAnimOffsets(
   ANIM* pAnimation)                     // Pointer to ANIM structure
{
   short s;

   for (s = 0; s < pAnimation->sNumFrames; s++)
   {
      if (pAnimation->aFrames[s].pImage)
//          pAnimation->aFrames[s].pImage += (BYTE*) m_pImageData;
         pAnimation->aFrames[s].pImage = (IMAGE*)
          ((BYTE*) pAnimation->aFrames[s].pImage +
           (DWORD) m_pImageData - (DWORD) 4
          );
      if (pAnimation->aFrames[s].pZoneSet)
      {
//         pAnimation->aFrames[s].pZoneSet += (BYTE*) m_pAnimData;
         pAnimation->aFrames[s].pZoneSet = (ZONESET*)
            ((BYTE*) pAnimation->aFrames[s].pZoneSet +
             (DWORD) m_pAnimData - (DWORD) 4
            );
         SetZonesetOffsets(pAnimation->aFrames[s].pZoneSet);
      }
   }
}

//*****************************************************************************
//
// SetZonesetOffsets
//
// Description:
//
//*****************************************************************************

void CUniverse::SetZonesetOffsets(
   ZONESET*   pZoneset)                  // Pointer to ZONESET structure
{
    short s;

   for (s = 0; s < MAX_ZONETYPES; s++)
   {
       if (pZoneset->apZoneLists[s])
//         pZoneset->apZoneLists[s] += (ZONELIST*) m_pAnimData;
         pZoneset->apZoneLists[s] = (ZONELIST*)
            ((BYTE*) pZoneset->apZoneLists[s] + 
             (DWORD) m_pAnimData - (DWORD) 4
            );
   }
}

//*****************************************************************************
//
// LoadBackgroundData
//
// Description:
//      Private function called by SetSection to load the background data file
//      for the section selected by SetSection().  All of the data from the 
//      file corresponding to the current section will be loaded into memory
//      and the member pointer m_pBackData will be set.
//
// Input:
//      none
//
//   Uses:
//      m_strFileBack to get the background build data filename
//      m_pBackData gets set to the BG structure in memory
//
// Output:
//      none
//
//   Return:
//      0 if the file was loaded correctly
//      ERROR code if it was not loaded correctly
//
//*****************************************************************************

short CUniverse::LoadBackgroundData()
{
   return 0;
}

//*****************************************************************************
//
// LoadAttributeData
//
// Description:
//      Private function called by SetSection to load the attribute data file
//      for this particular section.  The member pointer m_pAttributeData will
//      be set to the ATTRIB structure in memory.
//
// Input:
//      none
//
//   Uses:
//      m_strFileAttr to get the attribute data filename
//      m_pAttributeMap gets set to the attribute map array
//      m_pAttributeCont gets set to the array of attribute contours
//
// Output:
//      none
//
//   Return:
//      0 if the file was loaded correctly
//      ERROR code if the file was not loaded correctly
//
//*****************************************************************************

short CUniverse::LoadAttributeData()
{
   FILE* fpAttrData;

   fpAttrData = fopen(m_strFileAttr, "rb");


   short sLength;

//   ifstream ifsAttrData(m_strFileAttrib, ios::in, filebuf::sh_read);

   fread(&m_sMapWidth, sizeof(short), 1, fpAttrData);   //get width
     fread(&sLength, sizeof(short), 1, fpAttrData);      //get height
   sLength *= m_sMapWidth;                              //data = width*height
//   ifsAttrData >> sLength;
   m_pAttrMap = new WORD[sLength];
//   ifsAttrData.read();
   fread(m_pAttrMap, sizeof(WORD), sLength, fpAttrData);

     fread(&sLength, sizeof(WORD), 1, fpAttrData);
   m_pAttrCont = (CONTOUR*) new WORD[sLength];
   fread(m_pAttrCont, sizeof(WORD), sLength, fpAttrData);

//   ifsAttrData >> sLength;
//   m_pAttrCont = new short[sLength];
//   ifsAttrData.read();

   return 0;
}


//*****************************************************************************
//
// LoadEventData
//
// Description:
//      Private function called by SetSection to load the event data file
//      for this particular section.  The member pointer m_pEventData will
//      be set to the EVENT structure in memory.
//
// Input:
//      none
//
//   Uses:
//      m_strFileEvent to get the event data filename
//      m_pEventData gets set to the EVENT structure
//
// Output:
//      none
//
//   Return:
//      0 if the file was loaded correctly
//      ERROR code if the file was not loaded correctly
//
//*****************************************************************************

short CUniverse::LoadEventData()
{
   return 0;
}

//*****************************************************************************
//
// LoadPalette
//
// Description:
//      Private function called by SetSection to load the palette for either
//      the animation set or the backgrounds.  The function returns a pointer
//      to the palette structure.
//
// Input:
//      strFileName = the filename of a palette file to be loaded
//
// Uses:
//      none
//
// Output:
//      none
//
//   Return:
//      PALETTE* (pointer to palette structure)
//      NULL if there was an error in reading the file
//
//*****************************************************************************

PALETTE* CUniverse::LoadPalette(
   LPSTR lpstrFilePalette)               // Palette file to be loaded
{
   return NULL;
}

//*****************************************************************************
// EOF
//*****************************************************************************

