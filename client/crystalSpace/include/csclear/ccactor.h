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

#ifndef __CC_CCACTOR_H__
#define __CC_CCACTOR_H__

#include "csgeom/math3d.h"
#include "csutil/scf.h"
#include "csclear/iactor.h"
#include "csclear/iactrvel.h"
#include "csclear/ccentity.h"

class csMeshFactoryWrapper;
class csMeshWrapper;
class csThing;
class csThing;
struct iCollideSystem;

class ccActor : public iActor
{
public:
  DECLARE_IBASE;

  ccActor(iSystem *system);
  virtual ~ccActor();

  /// Assign a sprite template for the actor
  virtual void SetTemplate(csMeshFactoryWrapper &spritetpl);

  /// Assign a thing template for the actor
  virtual void SetTemplate(csThing &thingtpl);

  /// Set the actor's current sector
  virtual void SetSector(csSector *sector);

  virtual bool Initialize();
  virtual const csVector3 &GetLook() const;
  virtual void SetLook(const csVector3 &lookat, float speed = 0.0, bool cd = true);
  virtual void LookAt(const csVector3 &displacement, float speed = 0.0);
  virtual const csVector3 &GetUpwards() const;
  virtual void SetUpwards(const csVector3 &up, float speed = 0.0, bool cd = true);
  virtual void ChangeUpwards(const csVector3 &up_delta, float speed = 0.0);
  virtual void Prepare(cs_time elapsed_time, cs_time current_time);
  virtual void SetVisible(bool vis);
  virtual void SetPosition(csVector3 &position, bool cd = true);
  virtual void Move(const csVector3 &displacement, bool cd = true);
  virtual void SetRotation(csMatrix3 &trans, bool cd = true);
  virtual void Rotate(csVector3 &rot, float angle, bool cd = true);
  virtual iActorVelocity *CreateVelocity(bool relative = true);

protected:

  virtual csSector *GetSector() const;

  class ActorVel : public iActorVelocity
  {
    friend ccActor;
  public:
    DECLARE_IBASE;
    ActorVel(bool relative);
    virtual ~ActorVel();
    virtual iActor *GetActor() const;
    virtual float GetVelocity() const;
    virtual void SetVelocity(float velocity, float speed = 0.0);
    virtual void ChangeVelocity(float vel_delta, float speed = 0.0);
    virtual const csVector3 &GetDirection() const;
    virtual void SetDirection(const csVector3 &dir, float speed = 0.0);
    virtual void ChangeDirection(const csVector3 &dir_delta, float speed = 0.0);
    virtual const csVector3 &GetFocalPt() const;
    virtual void SetFocalPt(const csVector3 &pt, float speed = 0.0);
    virtual void ChangeFocalPt(const csVector3 &pt, float speed = 0.0);
    virtual const csVector3 &GetAxis() const;
    virtual void SetAxis(const csVector3 &pt, float speed = 0.0);
    virtual void ChangeAxis(const csVector3 &pt, float speed = 0.0);
    virtual float GetAngle() const;
    virtual void SetAngle(float angle, float speed = 0.0);
    virtual void ChangeAxis(float angle, float speed = 0.0);
  protected:
    csVector3 forward, fwd_delta, focal, focal_delta, axis, axis_delta;
    float velocity, v_delta, v_speed, f_speed, v_sign, focal_speed, axis_speed, angle, angle_delta, angle_speed;
    bool pref, rel, linear;
    void ApplyChange(cs_time elapsed_time);
    ccActor *actor;
  };
  friend ActorVel;

  DECLARE_TYPED_VECTOR(VelocityVectorBase, ActorVel);

  class VelocityVector : public VelocityVectorBase
  {
  public:
    virtual bool FreeItem(csSome Item);
  };

  VelocityVector velocities;

  enum ccActorType
  {
    None = 0,
    Sprite = 1,
    Thing = 2
  };

  enum ccActorMotion
  {
    LookAtMotion,
    UpVecMotion,
  };

  struct MotionData
  {
    ccActorMotion MotionType;
    csVector3 vec;
    float speed;
  };

  DECLARE_TYPED_VECTOR(MotionVectorBase, MotionData);

  class MotionVector : public MotionVectorBase
  {
  public:
    void DeleteByType(ccActorMotion type);
  };

  MotionVector motions;

  csMeshWrapper *sprite;
  csThing *thing;
  ccActorType type;
  iSystem *piSystem;
  csEngine *engine;
  csSector *sector;
  iCollideSystem* cdsys;
  bool mirrored_sector;

  csVector3 face;
  csVector3 upvec;

  iActorVelocity *activeVel;

  virtual void Collide(ccColEvent *col);

};

#endif // ! __CC_CCACTOR_H__
