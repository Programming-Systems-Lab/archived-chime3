/*
    Copyright (C) 1999 by Michael Long

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

#ifndef __IPHYSICS_H__
#define __IPHYSICS_H__

#include "csutil/scf.h"
#include "csclear/ientman.h"

struct iActor;
struct iActorVelocity;
struct iTrigger;
class csVector3;

//@@@ This probably isn't the best place for this
#define ccPhysicsList 0x01006878
struct PhysicsAssocData : public AssocData
{
  float mass;
  iActorVelocity *vel; // Actor Velocity for gravity
};

SCF_VERSION(iPhysicsConfig, 0, 0, 1);

struct iPhysicsConfig : public iBase
{

  // Create a new force with the given strength, radius, position it pulls 
  // towards, and whether it is accumulative (as most forces are).  If radius
  // is negative, force applies everywhere, which is good for gravity.
  // Returns an id for the new force
  virtual int NewForce(float strength, float radius, csVector3 &center, bool accumulative = true) = 0;
  // Delete the given force
  virtual void DeleteForce(int force) = 0;

};

#endif // ! __IPHYSICS_H__
