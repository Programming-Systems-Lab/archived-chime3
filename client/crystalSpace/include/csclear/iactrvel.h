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

#ifndef __CC_IACTOR_VELOCITY_H__
#define __CC_IACTOR_VELOCITY_H__

#include "csutil/scf.h"

struct iActor;

SCF_VERSION (iActorVelocity, 0, 0, 4);
struct iActorVelocity : public iBase
{

  // Returns the actor associated associated with this velocity
  virtual iActor *GetActor() const = 0;

  // Get and set velocity in meters per second.  Speed is the speed it changes
  // in meters per second.  0.0 is instantaneous change.
  virtual float GetVelocity() const = 0;
  virtual void SetVelocity(float velocity, float speed = 0.0) = 0;

  // Change the velocity to the given relative velocity at the given speed
  virtual void ChangeVelocity(float vel_delta, float speed = 0.0) = 0;

  /***************** Linear velocity functions ********************
   * In this case, the velocity is the speed of motion along the 
   * defined direction
   */

  // Get and set the direction of motion
  virtual const csVector3 &GetDirection() const = 0;
  virtual void SetDirection(const csVector3 &dir, float speed = 0.0) = 0;

  // Change the direction of motion.  Using Set/ChangeDirection implies
  // linear motion (as opposed to rotational motion)
  virtual void ChangeDirection(const csVector3 &dir_delta, float speed = 0.0) = 0;

  /***************** Rotational velocity functions ********************
   * In this case, the velocity is the speed of clockwise rotation 
   * around the given focal point/axis
   */

  // Get and set the focal point of rotation
  virtual const csVector3 &GetFocalPt() const = 0;
  virtual void SetFocalPt(const csVector3 &pt, float speed = 0.0) = 0;

  // Change the focal point of rotation
  virtual void ChangeFocalPt(const csVector3 &pt, float speed = 0.0) = 0;

  // Get and set the axis of rotation
  virtual const csVector3 &GetAxis() const = 0;
  virtual void SetAxis(const csVector3 &pt, float speed = 0.0) = 0;

  // Change the axis of rotation
  virtual void ChangeAxis(const csVector3 &pt, float speed = 0.0) = 0;

  // Get and set the angle of rotation
  virtual float GetAngle() const = 0;
  virtual void SetAngle(float angle, float speed = 0.0) = 0;

  // Change the angle of rotation
  virtual void ChangeAxis(float angle, float speed = 0.0) = 0;

};

#endif // ! __CC_IACTOR_VELOCITY_H__
