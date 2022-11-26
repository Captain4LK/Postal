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
// shapes.h
//
//
// History:
//      12/08/95   BRH   Started.
//
//      11/01/96   JMI   Changed:
//                     Old label:      New label:
//                     =========      =========
//                     CPt            RPt
//                     C2DPoint         R2DPoint
//                     C3DPoint         R3DPoint
//                     CRay            RRay
//                     C2DRay         R2DRay
//                     C3DRay         R3dRay
//                     CRectangle      RRectangle
//                     CCube            RCube
//                     CCircle         RCircle
//                     CSphere         RSphere
//
//      02/17/97   JMI   Removed ~RSphere() proto (had no body).
//
//      02/18/97   JMI   Added empty bodies to ~RRectangle(), ~RCube(), and
//                     ~RCircle().
//                     Also, added R2DLine and R3DLine.
//
//////////////////////////////////////////////////////////////////////////////
//
//   These shape classes were initially created to be used with the
// regions and in the games.  For most of the shapes there are 2D and 3D
// versions.  The one you choose would be based on the type of game being
// created.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SHAPES_H
#define SHAPES_H

#include "Blue.h"

class RPt
{
public:
S32 X;
S32 Y;

RPt()
{X = Y = 0;};

RPt(S32 lX, S32 lY)
{ X = lX; Y = lY; }

~RPt() {};
};

class R2DPoint : public RPt
{
public:
R2DPoint(S32 lX, S32 lY)
{X = lX; Y = lY;};
};

class R3DPoint : public RPt
{
public:
S32 Z;

R3DPoint()
{X = Y = Z = 0;};
R3DPoint(S32 lX, S32 lY, S32 lZ)
{X = lX; Y = lY; Z = lZ;};
};


class RRay
{
public:
S32 X;               // X originating position
S32 Y;               // Y originating position
float fXVect;        // X element of unit vector
float fYVect;        // Y element of unit vector

// Constructors
RRay()
{X = Y = 0; fXVect = fYVect = (float) 0.0;};

// Destructor
~RRay() {};
};

class R2DRay : public RRay
{
public:
// Constructors
R2DRay(S32 lX, S32 lY, float fXunit, float fYunit)
{X = lX; Y = lY, fXVect = fXunit; fYVect = fYunit;};
R2DRay(S32 lX1, S32 lY1, S32 lX2, S32 lY2);

// Destructor
~R2DRay() {};
};

class R3DRay : public RRay
{
public:
S32 Z;               // Z originating position
float fZVect;        // Z element of unit vector

// Constructors
R3DRay()
{X = Y = Z = 0; fXVect = fYVect = fZVect = (float) 0.0;};
R3DRay(S32 lX, S32 lY, S32 lZ, float fXunit, float fYunit, float fZunit)
{X = lX; Y = lY; Z = lZ; fXVect = fXunit; fYVect = fYunit; fZVect = fZunit;};
R3DRay(S32 lX1, S32 lY1, S32 lZ1, S32 lX2, S32 lY2, S32 lZ2);

// Destructor
~R3DRay() {};
};

class R2DLine
{
public:
S32 X1;                 // X originating position
S32 Y1;                 // Y originating position
S32 X2;                 // X ending position
S32 Y2;                 // Y ending position

// Constructors
R2DLine()
{X1 = Y1 = X2 = Y2 = 0; }

// Destructor
~R2DLine() {};
};

class R3DLine : public R2DLine
{
public:
S32 Z1;                 // Z originating position
S32 Z2;                 // Z ending position

// Constructors
R3DLine()
{Z1 = Z2 = 0; }

// Destructor
~R3DLine() {};
};

class RRectangle
{
public:
S32 lLeft;           // Left side
S32 lRight;       // Right side
S32 lTop;            // Top side
S32 lBottom;        // Bottom side

// Constructors
RRectangle()
{lLeft = lRight = lTop = lBottom = 0;};
RRectangle(S32 lL, S32 lR, S32 lT, S32 lB)
{lLeft = lL; lRight = lR; lTop = lT; lBottom = lB;};

// Destructor
~RRectangle() {};
};

class RCube
{
public:
S32 lLeft;           // Left side
S32 lRight;       // Right side
S32 lTop;            // Top side
S32 lBottom;         // Bottom side
S32 lFront;       // Front side
S32 lBack;           // Back side

// Constructors
RCube()
{lLeft = lRight = lTop = lBottom = lFront = lBack = 0;};
RCube(S32 lL, S32 lR, S32 lT, S32 lB, S32 lF, S32 lBk)
{lLeft = lL; lRight = lR; lTop = lT; lBottom = lB; lFront = lF; lBack = lBk;};

// Destructor
~RCube() {};
};

class RCircle
{
public:
S32 X;               // X coordinate of center point
S32 Y;               // Y coordinate of center point
S32 lRadius;         // Radius of the circle

// Constructors
RCircle()
{X = Y = lRadius = 0;};
RCircle(S32 lX, S32 lY, S32 lR)
{X = lX; Y = lY; lRadius = lR;};

// Destructor
~RCircle() {};
};

class RSphere
{
public:
S32 X;               // X coordinate of center point
S32 Y;               // Y coordinate of center point
S32 Z;               // Z coordinate of center point
S32 lRadius;         // Radius of the sphere

// Constructors
RSphere()
{X = Y = Z = lRadius = 0;};
RSphere(S32 lX, S32 lY, S32 lZ, S32 lR)
{X = lX; Y = lY; Z = lZ; lRadius = lR;};
};




#endif //SHAPES_H

//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
