/*
    Copyright (C) 2000 by Michael Long

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CC_COLEVENT_H__
#define __CC_COLEVENT_H__

#include "cstypes.h"
#include "cssys/csevent.h"

class csVector3;
class csMatrix3;
struct iEntity;
struct iActorVelocity;

// Collision motion used for interpreting how a collision occurred.  
// All coordinates are in world coordinates
class ccColEvent : public csEvent
{
public:
  DECLARE_IBASE;

  ccColEvent(const csVector3 &Delta, iEntity *Collided, bool Motion = true,
	     iEntity *Collider = NULL, iActorVelocity *Velocity = NULL);

  ccColEvent(const csVector3 &Rotation, float Angle, iEntity *Collided, 
	     iEntity *Collider = NULL, iActorVelocity *Velocity = NULL);

  ccColEvent(const csMatrix3 &Matrix, iEntity *Collided, 
	     iEntity *Collider = NULL, iActorVelocity *Velocity = NULL);

  ~ccColEvent();

  // This tells you how to interpret the motion data in CollideMotion
  enum CMType
  {
    cmPosition = 0,  // The delta vector is a positional displacement
    cmRotation,      // The delta vector is a vector of rotation, and angle is the angle of rotation
    cmMatrix,        // The original_matrix contains the original rotation matrix, all else is undefined
    cmScale          // The delta vector is a scale factor for each axis
  };

  CMType coltype;
  csVector3 *delta;    // Undefined if type==Matrix, otherwise defined as above
  csMatrix3 *original_matrix; // Only used by type==Matrix, otherwise value is undefined!
  iActorVelocity *velocity; // If the entity is an actor and a velocity caused this collision, this is the velocity, otherwise it must be NULL
  float angle; // Only used by type==Rotation, otherwise value is undefined!
  iEntity *collider;    // The culprit (if NULL, then culprit was world geometry)
  iEntity *collided;    // The victim
};

#define CC_COL_EVENT(x) (STATIC_CAST(ccColEvent*,&x))

#define CC_PHYSICS_EVENT 0xDC
#define CC_COLLISION_EVENT 0x93

#endif // ! __CC_COLEVENT_H__
