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

#ifndef __CC_CCENTITY_H__
#define __CC_CCENTITY_H__

#include "csgeom/math3d.h"
#include "csgeom/matrix3.h"
#include "csutil/scf.h"
#include "csclear/ientity.h"
#include "ievent.h"

struct iSystem;
struct iEntityManager;
class csCollider;
class csSector;
class ccColEvent;

class ccEntity : public iEntity
{
public:
  DECLARE_IBASE;

  ccEntity(iSystem *system);
  virtual ~ccEntity();

  struct EventPlugInterface : public iEventPlug
  {
    DECLARE_EMBEDDED_IBASE(ccEntity);
    virtual unsigned GetPotentiallyConflictingEvents () { return 0; }
    virtual unsigned QueryEventPriority (unsigned /*iType*/) { return 100; }
    virtual void EnableEvents (unsigned /*iType*/, bool /*iEnable*/) {}
  } scfiEventPlug;

  /// Initialize the entity
  virtual bool Initialize();

  /// Render the entity
  /// For use by the Entity Manager only!
  virtual void Prepare(cs_time, cs_time) { }

  /// Get and set the entity's visibility
  virtual bool GetVisible() const;
  virtual void SetVisible(bool vis);

  /// Get and set the entity's world coordinates
  virtual const csVector3 &GetPosition() const;
  virtual void SetPosition(const csVector3 &position, bool cd = true);

  /// Move entity relative to current coordinates
  virtual void Move(const csVector3 &displacement, bool cd = true);

  // Get and set the entity's transformation matrix
  virtual const csMatrix3 &GetRotation() const;
  virtual void SetRotation(const csMatrix3 &trans, bool cd = true);

  // Rotate entity around given vector, angle units
  virtual void Rotate(const csVector3 &rot, float angle, bool cd = true);

  // Get and set scaling factor for each axis
  virtual const csVector3 &GetScaling() const;
  virtual void SetScaling(const csVector3 &scale, bool cd = true);

  // Change scaling factor relative to current scale
  virtual void InflateScale(const csVector3 &scale, bool cd = true);

protected:

  // Position vector, scaling vector, and rotation matrix, respectfully
  csVector3 pos, scale;
  csMatrix3 rot;

  // csCollide object for entity and Collide method, which can be overriden
  // to provide specialized CD code.
  csCollider *collider;
  virtual void Collide(ccColEvent *col);

  // Used for retrieving the current sector
  virtual csSector *GetSector() const;

  iSystem *piSystem;
  iEntityManager *piEntityMgr;

  iEventOutlet *cOutlet;

};

#endif // ! __CC_CCENTITY_H__
