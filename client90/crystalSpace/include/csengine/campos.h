/*
    Copyright (C) 2000 by Andrew Zabolotny
  
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

#ifndef __CS_CAMPOS_H__
#define __CS_CAMPOS_H__

#include "csutil/csobject.h"
#include "csutil/nobjvec.h"
#include "csgeom/vector3.h"
#include "csengine/camera.h"
#include "iengine/campos.h"

struct iEngine;
struct iCamera;

/**
 * Camera position structure. This structure is used internally by the
 * engine to store named camera positions than can be retrieved by
 * client programs to store starting map points, teleporter positions
 * and so on. In the map file you can use CAMERA (...) keyword to
 * define such points.
 */
class csCameraPosition : public csObject
{
private:
  /// Destroy this object and free all associated memory
  virtual ~csCameraPosition ();

public:
  /// The sector this camera points to
  char *Sector;
  /// The camera position
  csVector3 Position;
  /// Camera orientation: forward vector
  csVector3 Forward;
  /// Camera orientation: upward vector
  csVector3 Upward;

  /// Initialize the camera position object
  csCameraPosition (const char *iName, const char *iSector,
    const csVector3 &iPosition,
    const csVector3 &iForward, const csVector3 &iUpward);

  /// Change camera position object
  void Set (const char *iSector, const csVector3 &iPosition,
    const csVector3 &iForward, const csVector3 &iUpward);

  /// Load the camera position into a camera object
  bool Load (iCamera*, iEngine*);

  SCF_DECLARE_IBASE_EXT (csObject);

  //--------------------- iCameraPosition implementation ----------------------
  struct CameraPosition : public iCameraPosition
  {
    SCF_DECLARE_EMBEDDED_IBASE(csCameraPosition);

    virtual iObject *QueryObject()
    { return scfParent; }
    virtual const char *GetSector()
    { return scfParent->Sector; }
    virtual csVector3 GetPosition()
    { return scfParent->Position; }
    virtual csVector3 GetUpwardVector()
    { return scfParent->Upward; }
    virtual csVector3 GetForwardVector()
    { return scfParent->Forward; }
    virtual bool Load (iCamera *c, iEngine *e)
    { return scfParent->Load (c, e); }
    virtual void Set (const char *sector, const csVector3 &pos,
      const csVector3 &forward, const csVector3 &upward)
    {
      scfParent->Set (sector, pos, forward, upward);
    }
  } scfiCameraPosition;
  friend struct CameraPosition;
};

#endif // __CS_CAMPOS_H__
