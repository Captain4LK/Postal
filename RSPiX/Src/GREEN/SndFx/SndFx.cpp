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
///////////////////////////////////////////////////////////////////////////////
//
//	SndFx.cpp
//
// History:
//		09/06/96 JMI	Started.
//
//		09/10/96	JMI	Now uses a precalculated table (generated by a call to
//							SetFadeAccuracy()) to do fades.
//
//		09/10/96	JMI	Now effects can optionally have a destination other than
//							the source.
//
//		09/11/96	JMI	Fade outs for 8 bit were sending the fade in variables
//							to the decimate unsigned template.
//
//		09/24/96 MJR	Added <stdlib.h> so it would compile on Mac.
//
//		10/30/96	JMI	Changed:
//							Old label:				New label:
//							=========				=========
//							CSndFx					RSndFx
//							MAX_BITSPERSAMPLE		RSP_SNDFX_MAX_BITSPERSAMPLE
//							NUM_FADE_INTERVALS	RSP_SNDFX_NUM_FADE_INTERVALS
//							SNDFX_*					*	Macros changed to enum/typedef.
//							m_fx						m_fx
//
//////////////////////////////////////////////////////////////////////////////
//
// Implements sound effects on PCM data.
//
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "Blue.h"

#ifdef PATHS_IN_INCLUDES
   #include "GREEN/SndFx/SndFx.h"
#else
   #include "SndFx.h"
#endif // PATHS_IN_INCLUDES


//////////////////////////////////////////////////////////////////////////////
// Initialize static member variables.
//////////////////////////////////////////////////////////////////////////////

RSndFx::PCMINFO RSndFx::ms_apcminfo[RSP_SNDFX_MAX_BITSPERSAMPLE + 1] =     // Stores info
                                                                       // particular to each
                                                                       // PCM type.
{
   { 0x00000000, 0x00000000	},    // 0 Bit.
   { 0x00000000, 0x00000001	},    // 1 Bit.
   { 0x00000000, 0x00000003	},    // 2 Bit.
   { 0x00000000, 0x00000007	},    // 3 Bit.
   { 0x00000000, 0x0000000F	},    // 4 Bit.
   { 0x00000000, 0x0000001F	},    // 5 Bit.
   { 0x00000000, 0x0000003F	},    // 6 Bit.
   { 0x00000000, 0x0000007F	},    // 7 Bit.
   { 0x00000000, 0x000000FF	},    // 8 Bit.
   { 0x00000080, 0x000000FF	},    // 9 Bit.
   { 0x00000100, 0x000001FF	},    // 10 Bit.
   { 0x00000200, 0x000003FF	},    // 11 Bit.
   { 0x00000400, 0x000007FF	},    // 12 Bit.
   { 0x00000800, 0x00000FFF	},    // 13 Bit.
   { 0x00001000, 0x00001FFF	},    // 14 Bit.
   { 0x00002000, 0x00003FFF	},    // 15 Bit.
   { -32768,		32767	},    // 16 Bit.
   { 0x00008000, 0x0000FFFF	},    // 17 Bit.
   { 0x00010000, 0x0001FFFF	},    // 18 Bit.
   { 0x00020000, 0x0003FFFF	},    // 19 Bit.
   { 0x00040000, 0x0007FFFF	},    // 20 Bit.
   { 0x00080000, 0x000FFFFF	},    // 21 Bit.
   { 0x00100000, 0x001FFFFF	},    // 22 Bit.
   { 0x00200000, 0x003FFFFF	},    // 23 Bit.
   { 0x00400000, 0x007FFFFF	},    // 24 Bit.
   { 0x00800000, 0x00FFFFFF	},    // 25 Bit.
   { 0x01000000, 0x01FFFFFF	},    // 26 Bit.
   { 0x02000000, 0x03FFFFFF	},    // 27 Bit.
   { 0x04000000, 0x07FFFFFF	},    // 28 Bit.
   { 0x08000000, 0x0FFFFFFF	},    // 29 Bit.
   { 0x10000000, 0x1FFFFFFF	},    // 30 Bit.
   { 0x20000000, 0x3FFFFFFF	},    // 31 Bit.
   { 0x40000000, 0x7FFFFFFF	},    // 32 Bit.
};

U8*	RSndFx::ms_pu8Fade	= NULL;  // Unsigned 8 bit output
                                    // fade table.
S16*	RSndFx::ms_ps16Fade	= NULL;  // Signed 16 bit output
                                    // fade table.

short	RSndFx::ms_sNumFadeSteps	= 0;  // Number of fade steps.

S32 RSndFx::ms_lSamplesPerSec  = 0;    // Samples per second.
S32 RSndFx::ms_lBitsPerSample  = 0;    // Bits per sample.
S32 RSndFx::ms_lNumChannels    = 0;    // Number of channels.

S32 RSndFx::ms_lBitsPerSec  = 0;    // Number of bits per second.
// Can be used to convert bytes to milliseconds
// and convert milliseconds into bytes.  See
// macros BYTES2MS and MS2BYTES in SndFx.CPP.

//////////////////////////////////////////////////////////////////////////////
// Macros.
//////////////////////////////////////////////////////////////////////////////

// Converts bytes into milliseconds using a pre calculated variable.
// 8000L consists of 8 (for bytes to bits) and 1000 (for seconds
// to milliseconds).
#define BYTES2MS(bytes)	(((bytes) * 8000L) / ms_lBitsPerSec)
// Converts milliseconds into bytes using a pre calculated variable.
// 8000L consists of 8 (for bits to bytes) and 1000 (for milliseconds
// to seconds).
#define MS2BYTES(ms)		(((ms) * ms_lBitsPerSec) / 8000L)

#define PCM8_SILENCE		0x7F     // 0x80 will not work for unsigned algorithm.
#define PCM16_SILENCE	0x0000

#define FIXED_POINT_DEPTH		1000     // Depth of fixed point notation (e.g.,
                                       // 1000 == depth of 1/1000).

//////////////////////////////////////////////////////////////////////////////
// Functions.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Default Constructor.
// Returns nothing.
// (public)
//
//////////////////////////////////////////////////////////////////////////////
RSndFx::RSndFx()
{
   // Initializes members.
   Init();
}

//////////////////////////////////////////////////////////////////////////////
//
// Destructor.
// Returns nothing.
// (public)
//
//////////////////////////////////////////////////////////////////////////////
RSndFx::~RSndFx()
{
   // Clear and free.
   Clear();
}

//////////////////////////////////////////////////////////////////////////////
//
// Initialize instantiable members.
// Returns nothing.
// (public)
//
//////////////////////////////////////////////////////////////////////////////
void RSndFx::Init(void)
{
   // Initialize members.
   m_fx					= NoFX;  // Currently active effects.

   ///////////////// Fade In /////////////////////////////////////////

   m_lFadeInMillisecondsDuration		= 0;  // Original duration.
   m_lFadeInBytesDurationAffected	= 0;  // Amount left to fade.

   ///////////////// Fade Out ////////////////////////////////////////

   m_lFadeOutMillisecondsDuration	= 0;  // Original duration.
   m_lFadeOutBytesDurationRemaining	= 0;  // Amount left to fade.
}

///////////////////////////////////////////////////////////////////////////////
//
// Release any dynamic memory and reset variables.
// Clears all effects.
// (public)
//
///////////////////////////////////////////////////////////////////////////////
void RSndFx::Clear(void)
{
   Init();
}

///////////////////////////////////////////////////////////////////////////////
//
// Release any dynamic memory referenced by static members.
// (static/public)
//
///////////////////////////////////////////////////////////////////////////////
void RSndFx::CleanUp(void)
{
   if (ms_pu8Fade != NULL)
   {
      free(ms_pu8Fade);
      ms_pu8Fade	= NULL;
   }

   if (ms_ps16Fade != NULL)
   {
      free(ms_ps16Fade);
      ms_ps16Fade	= NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
//
// Reset effects to start over.
//
///////////////////////////////////////////////////////////////////////////////
void RSndFx::Reset(void)
{
   ///////////////// Fade In /////////////////////////////////////////////////

   SetUpFadeIn(m_lFadeInMillisecondsDuration);

   ///////////////// Fade Out ////////////////////////////////////////////////

   SetUpFadeOut(m_lFadeOutMillisecondsDuration);
}

//////////////////////////////////////////////////////////////////////////////
//
// Sets the fade accuracy (i.e., the number of steps to perform a fade).
// This costs sNumStemps * 256 * bits per sample / 8 bytes of memory.
// This function should only be called after the bits per sample have
// been set via a call to SetDataType().
//
//
//
//												Power A
// Power difference in dB	= 10 log	-------
//												Power B
//
// Every 3dB, the audible loudness doubles.
// For acoustics, 0dB often means the threshold of hearing, 0.0002ubar
// (Microbars: a bar is the "normal" pressure of air).
// (static).
//
//////////////////////////////////////////////////////////////////////////////
short RSndFx::SetFadeAccuracy(   // Returns 0 on success.
   short sNumSteps)              // Number of steps to fades; see above.
{
   short	sRes	= 0;  // Assume success.

   if (ms_lBitsPerSec != 0)
   {
      // Clean up any existing memory.
      CleanUp();

      // If bits per sample is value that indicates signage . . .
      switch (ms_lBitsPerSample)
      {
      case 8:
      {
         ms_pu8Fade	= (U8*)malloc((S32)sNumSteps * 256L * sizeof(S16));
         if (ms_pu8Fade != NULL)
         {
            short	sStep;
            short	sSample;
            float	fStep				= 1.0F / (float)sNumSteps;
            float	fCurDecimation	= fStep;
            U8*	pu8	= ms_pu8Fade;
            for (sStep	= 0; sStep < sNumSteps; sStep++)
            {
               for (sSample = 0; sSample < 256; sSample++)
               {
                  *pu8++	= (U8)( (float)(sSample - 0x80) * fCurDecimation) + 0x80;
               }

               fCurDecimation	+= fStep;
            }

            // Success.
            ms_sNumFadeSteps	= sNumSteps;

            static short sSetUpAtExit	= FALSE;    // Remember if we already did this.
            // If we haven't already set up the at exit function . . .
            if (sSetUpAtExit == FALSE)
            {
               // Set up atexit to deallocate memory.
               atexit(CleanUp);
               sSetUpAtExit	= TRUE;
            }
         }
         else
         {
            TRACE("SetFadeAccuracy(): Failed to allocate fade lookup table.\n");
            sRes	= -1;
         }
         break;
      }

      case 16:
      {
         ms_ps16Fade	= (S16*)malloc((S32)sNumSteps * 256L * sizeof(S16));
         if (ms_ps16Fade != NULL)
         {
            short	sStep;
            short	sSample;
            float	fStep				= 1.0F / (float)sNumSteps;
            float	fCurDecimation	= fStep;
            S16*	ps16	= ms_ps16Fade;
            for (sStep	= 0; sStep < sNumSteps; sStep++)
            {
               for (sSample = 0; sSample < 256; sSample++)
               {
                  *ps16++	= (S16)( (float)((sSample - 128) << 8) * fCurDecimation);
               }

               fCurDecimation	+= fStep;
            }

            // Success.
            ms_sNumFadeSteps	= sNumSteps;

            static short sSetUpAtExit	= FALSE;    // Remember if we already did this.
            // If we haven't already set up the at exit function . . .
            if (sSetUpAtExit == FALSE)
            {
               // Set up atexit to deallocate memory.
               atexit(CleanUp);
               sSetUpAtExit	= TRUE;
            }
         }
         else
         {
            TRACE("SetFadeAccuracy(): Failed to allocate fade lookup table.\n");
            sRes	= -1;
         }
         break;
      }

      default:
         TRACE("SetFadeAccuracy(): %ld bits per sample not supported.\n", ms_lBitsPerSample);
         break;
      }
   }
   else
   {
      TRACE("SetFadeAccuracy(): No data type set.  Use SetDataType().\n");
      sRes	= 1;
   }

   return sRes;
}

//////////////////////////////////////////////////////////////////////////////
//
// Set type of PCM data in use.  This will reset all effects.
// (static).
//
//////////////////////////////////////////////////////////////////////////////
void RSndFx::SetDataType(  // Returns nothing.
   S32 lSamplesPerSec,     // Samples per second.
   S32 lBitsPerSample,     // Bits per sample.
   S32 lNumChannels)    // Number of channels.
{
   ms_lSamplesPerSec	= lSamplesPerSec;
   ms_lBitsPerSample	= lBitsPerSample;
   ms_lNumChannels	= lNumChannels;

   // Precalculate bits per second to aid in computation time.
   ms_lBitsPerSec	= lSamplesPerSec * lBitsPerSample * lNumChannels;
}

/////////////////////////////////////////////////////////////////////////
//
// Fades in lNumBytes of pnData for signed TYPE.
//
/////////////////////////////////////////////////////////////////////////
template <
   class TYPE>          // Data type, signed.
inline void Decimate(   // Returns nothing.
   TYPE* psnSrcData,    // Data buffer to decimate.
   TYPE* psnDstData,    // Decimation destination.
   S32 lNumBytes,    // Number of bytes in buffer.
   TYPE* psnTable)      // Attenuation table.
{
   S32 lNumSamples = lNumBytes / sizeof(TYPE);
   while (lNumSamples-- > 0)
   {
      *psnDstData++	= psnTable[((*psnSrcData++ + 0x0080) >> 8) + 128];
   }
}

/////////////////////////////////////////////////////////////////////////
//
// Fades in lNumBytes of punData for unsigned UTYPE.
//
/////////////////////////////////////////////////////////////////////////
template <
   class UTYPE>               // Data type, unsigned.
inline void UnsignedDecimate( // Returns nothing.
   UTYPE* punSrcData,         // Data buffer to decimate.
   UTYPE* punDstData,         // Decimation destination.
   S32 lNumBytes,          // Number of bytes in buffer.
   UTYPE* punTable)           // Attenuation table.
{

   S32 lNumSamples = lNumBytes / sizeof(UTYPE);
   while (lNumSamples-- > 0)
   {
      *punDstData++	= punTable[*punSrcData++];
   }
}

/////////////////////////////////////////////////////////////////////////
//
// Implements the effect on the provided buffer.
//
/////////////////////////////////////////////////////////////////////////
void RSndFx::Do(                 // Returns nothing.
   UCHAR* pucSrcData,            // Data to affect.
   S32 lBufSize,                 // Amount of data.
   UCHAR* pucDstData /*= NULL*/) // Destination for data, defaults
                                 // to same as source.
{
   if (ms_lBitsPerSec > 0)
   {
      // If any active effects . . .
      if (m_fx != 0)
      {
         // If there is no destination . . .
         if (pucDstData == NULL)
         {
            pucDstData	= pucSrcData;
         }

         if (m_fx & FadeIn)
         {
            if (m_lFadeInBytesDurationAffected < m_lFadeInBytesDuration)
            {
               switch (ms_lBitsPerSample)
               {
               case 8:
               {
                  UnsignedDecimate(                      // Returns nothing.
                     (U8*)pucSrcData,                    // Data buffer to fade.
                     (U8*)pucDstData,                    // Dest data buffer.
                     lBufSize,                           // Number of bytes in buffer.
                     ms_pu8Fade + (m_lFadeInBytesDurationAffected / m_lFadeInRate) * 256 );           // Attenuation Table.
                  break;
               }
               case 16:
               {
                  Decimate(                              // Returns nothing.
                     (S16*)pucSrcData,                   // Data buffer to fade.
                     (S16*)pucDstData,                   // Dest data buffer.
                     lBufSize,                           // Number of bytes in buffer.
                     ms_ps16Fade + (m_lFadeInBytesDurationAffected / m_lFadeInRate) * 256 );          // Attenuation Table.
                  break;
               }
               default:
               {
                  TRACE("Do(): Unsupported bits per sample.\n");
                  break;
               }
               }

               m_lFadeInBytesDurationAffected	+= lBufSize;
            }
            else
            {
               // Done.
               ActivateFadeIn(FALSE);
            }
         }
         else
         {
            if (m_fx & FadeOut)
            {
               if (m_lFadeOutBytesDurationRemaining > 0)
               {
                  switch (ms_lBitsPerSample)
                  {
                  case 8:
                  {
                     UnsignedDecimate(                      // Returns nothing.
                        (U8*)pucSrcData,                    // Data buffer to fade.
                        (U8*)pucDstData,                    // Dest data buffer.
                        lBufSize,                           // Number of bytes in buffer.
                        ms_pu8Fade + (m_lFadeOutBytesDurationRemaining / m_lFadeOutRate) * 256 );           // Attenuation Table.
                     break;
                  }
                  case 16:
                  {
                     Decimate(                              // Returns nothing.
                        (S16*)pucSrcData,                   // Data buffer to fade.
                        (S16*)pucDstData,                   // Dest data buffer.
                        lBufSize,                           // Number of bytes in buffer.
                        ms_ps16Fade + (m_lFadeOutBytesDurationRemaining / m_lFadeOutRate) * 256 );          // Attenuation table.
                     break;
                  }
                  default:
                  {
                     TRACE("Do(): Unsupported bits per sample.\n");
                     break;
                  }
                  }

                  m_lFadeOutBytesDurationRemaining	-= lBufSize;
               }
               else
               {
                  // Done.
                  ActivateFadeOut(FALSE);
               }
            }
         }
      }
   }
   else
   {
      TRACE("Do(): Called, but no data format set.  Use SetDataType().\n");
   }
}

/////////////////////////////////////////////////////////////////////////
// Various FX.
/////////////////////////////////////////////////////////////////////////

//////////////////////////// Fade In ////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// Set up a fade in.
//
/////////////////////////////////////////////////////////////////////////
short RSndFx::SetUpFadeIn( // Returns 0 on success.
   S32 lDuration)       // Duration until silence in milliseconds.
{
   short	sRes	= 0;  // Assume success.

   // Must set data type before setting up effect . . .
   if (ms_lBitsPerSec > 0)
   {
      if (lDuration > 0)
      {
         m_lFadeInMillisecondsDuration		= lDuration;
         m_lFadeInBytesDurationAffected	= 0;
         m_lFadeInBytesDuration				= MS2BYTES(m_lFadeInMillisecondsDuration);

         // Determine the number of bytes per table index.
         m_lFadeInRate							= m_lFadeInBytesDuration / ms_sNumFadeSteps;
      }
   }
   else
   {
      TRACE("FadeIn(): Data type not set.  Use SetDataType().\n");
      sRes	= 1;
   }

   return sRes;
}

/////////////////////////////////////////////////////////////////////////
//
// Activate/Deactivate fade in.
//
/////////////////////////////////////////////////////////////////////////
void RSndFx::ActivateFadeIn(  // Returns nothing.
   short	sActivate)           // TRUE to activate, FALSE to deactivate.
{
   if (sActivate == FALSE)
   {
      m_fx	= (FX)(m_fx & (~FadeIn));
   }
   else
   {
      m_fx	= (FX)(m_fx | FadeIn);
   }
}

/////////////////////// Fade Out ////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
//
// Set up a fade out.
//
/////////////////////////////////////////////////////////////////////////
short RSndFx::SetUpFadeOut(   // Returns 0 on success.
   S32 lDuration)          // Duration until full volume in milliseconds.
{
   short	sRes	= 0;  // Assume success.

   // Must set data type before setting up effect . . .
   if (ms_lBitsPerSec > 0)
   {
      if (lDuration > 0)
      {
         m_lFadeOutMillisecondsDuration	= lDuration;
         m_lFadeOutBytesDurationRemaining	= MS2BYTES(m_lFadeOutMillisecondsDuration);
         m_lFadeOutBytesDuration				= m_lFadeOutBytesDurationRemaining;

         // Determine the number of bytes per table index.
         m_lFadeOutRate							= m_lFadeOutBytesDuration / ms_sNumFadeSteps;
      }
   }
   else
   {
      TRACE("FadeOut(): Data type not set.  Use SetDataType().\n");
      sRes	= 1;
   }

   return sRes;
}

/////////////////////////////////////////////////////////////////////////
//
// Activate/Deactivate fade out.
//
/////////////////////////////////////////////////////////////////////////
void RSndFx::ActivateFadeOut( // Returns nothing.
   short	sActivate)           // TRUE to activate, FALSE to deactivate.
{
   if (sActivate != FALSE)
   {
      // Deactivates fade ins.
      ActivateFadeIn(FALSE);

      m_fx	= (FX)(m_fx | FadeOut);
   }
   else
   {
      m_fx	= (FX)(m_fx & (~FadeOut));
   }
}

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////