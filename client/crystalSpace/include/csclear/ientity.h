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

#ifndef __CC_IENTITY_H__
#define __CC_IENTITY_H__

#include "csutil/scf.h"

class csVector3;
class csMatrix3;

SCF_VERSION (iEntity, 0, 0, 7);

struct iEntity : public iBase
{
  /// Initialize the entity
  virtual bool Initialize() = 0;

  /// Called before the rendering cycle if the entity is visible
  /// For use by the Entity Manager only!
  virtual void Prepare(cs_time elapsed_time, cs_time current_time) = 0;

  /// Get and set the entity's visibility
  virtual bool GetVisible() const = 0;
  virtual void SetVisible(bool vis) = 0;

  /// Get and set the entity's world coordinates
  virtual const csVector3 &GetPosition() const = 0;
  virtual void SetPosition(const csVector3 &position, bool cd = true) = 0;

  /// Move entity relative to current coordinates
  virtual void Move(const csVector3 &displacement, bool cd = true) = 0;

  // Get and set the entity's rotation matrix
  virtual const csMatrix3 &GetRotation() const = 0;
  virtual void SetRotation(const csMatrix3 &trans, bool cd = true) = 0;

  // Rotate entity around given vector, angle units
  virtual void Rotate(const csVector3 &rot, float angle, bool cd = true) = 0;

  // Get and set scaling factor for each axis
  virtual const csVector3 &GetScaling() const = 0;
  virtual void SetScaling(const csVector3 &scale, bool cd = true) = 0;

  // Change scaling factor relative to current scale
  virtual void InflateScale(const csVector3 &scale, bool cd = true) = 0;

};

#endif // ! __CC_IENTITY_H__
