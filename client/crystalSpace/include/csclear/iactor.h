/*
    Copyright (C) 1999,2000 by Michael Long

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

#ifndef __CC_IACTOR_H__
#define __CC_IACTOR_H__

#include "csutil/scf.h"
#include "csclear/ccentity.h"

struct iActorVelocity;

SCF_VERSION (iActor, 0, 0, 5);

struct iActor : public ccEntity
{

  iActor(iSystem *system) : ccEntity(system) { }

  // Get and set the direction the entity is facing
  virtual const csVector3 &GetLook() const = 0;
  virtual void SetLook(const csVector3 &lookat, float speed = 0.0, bool cd = true) = 0;

  // Change the lookat to the given relative  position at the given speed 
  // (meters/second).  speed = 0.0 means instant movement.
  virtual void LookAt(const csVector3 &displacement, float speed = 0.0) = 0;

  // Get and set the upward orientation
  virtual const csVector3 &GetUpwards() const = 0;
  virtual void SetUpwards(const csVector3 &up, float speed = 0.0, bool cd = true) = 0;

  // Change the upward orientation to the given relative position at the 
  // given speed(meters/second).  speed = 0.0 means instant movement.
  virtual void ChangeUpwards(const csVector3 &up_delta, float speed = 0.0) = 0;

  // Create a new velocity to manipulate this iActor with.  If relative is
  // true, the new velocity direction is relative to the direction of the
  // LookAt vector, otherwise is it absolute (in actor space).
  virtual iActorVelocity *CreateVelocity(bool relative = true) = 0;

};

#endif // ! __CC_IACTOR_H__
