/*
    Copyright (C) 2000 by Jorrit Tyberghein
  
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

#ifndef __CS_MOVABLE_H__
#define __CS_MOVABLE_H__

#include "csgeom/transfrm.h"
#include "csutil/csvector.h"
#include "imovable.h"

class csSector;
class csObject;

/**
 * This class represents an entity that can move in the engine.
 * It has a list of sectors and a position (a list of sectors
 * because an entity can overlap several sectors at the same time
 * through portals). This class itself does not have geometry.
 * It is only responsible for managing movement.
 */
class csMovable : public iBase
{
private:
  /// World to object transformation.
  csReversibleTransform obj;
  /// List of sectors.
  csVector sectors;

  /**
   * Parent (for hierachical transformations).
   * Note that if the parent is not NULL then the list of
   * sectors is ignored for this movable (the parent list is
   * returned) and the 'obj' transformation is relative to
   * the parent one.
   */
  csMovable* parent;
  /// SCF interface to movable.
  iMovable* iparent;

  /**
   * Object on which this movable operates (csThing or
   * csSprite currently).
   */
  csObject* object;

public:
  /**
   * Create a default movable.
   */
  csMovable ();

  /// Destructor.
  virtual ~csMovable ();

  /// Set object on which this movable operates.
  void SetObject (csObject* obj) { object = obj; }

  /// Set the parent movable.
  void SetParent (csMovable* parent);

  /// Get the parent movable.
  csMovable* GetParent ()
  {
    return parent;
  }

  /**
   * Initialize the list of sectors to one sector where
   * this thing is. This is a conveniance funcion.
   * This function does not update the corresponding list in
   * the sector. It only does a local update here.
   * This function does not do anything if the parent is not NULL.
   */
  void SetSector (csSector* sector)
  {
    ClearSectors ();
    AddSector (sector);
  }

  /**
   * Clear the list of sectors.
   * This function does not do anything if the parent is not NULL.
   */
  void ClearSectors ();

  /**
   * Add a sector to the list of sectors.
   * This function does not do anything if the parent is not NULL.
   */
  void AddSector (csSector* sector);

  /**
   * Get list of sectors for this entity.
   * This will return the sectors of the parent if there
   * is a parent.
   */
  csVector& GetSectors ()
  {
    if (parent) return parent->GetSectors ();
    else return sectors;
  }

  /**
   * Get the specified sector where this entity lives.
   * (conveniance function).
   */
  csSector* GetSector (int idx) { return (csSector*)GetSectors ()[idx]; }

  /**
   * Return true if we are placed in a sector.
   */
  bool InSector ()
  {
    return GetSectors ().Length () > 0;
  }

  /**
   * Set the transformation vector and sector to move to
   * some position.
   */
  void SetPosition (csSector* home, const csVector3& v);

  /**
   * Set the transformation vector for this object. Note
   * that the sectors are unchanged.
   */
  void SetPosition (const csVector3& v)
  {
    obj.SetOrigin (v);
  }

  /**
   * Get the current local position.
   */
  const csVector3& GetPosition () const { return obj.GetOrigin (); }

  /**
   * Get the current position using the full transformation (using
   * possible parent transformations).
   * @@@ Currently not very efficient!
   */
  const csVector3 GetFullPosition () const { return GetFullTransform ().GetOrigin (); }

  /**
   * Set the transformation matrix for this entity.
   */
  void SetTransform (const csMatrix3& matrix);

  /**
   * Set the local world to object tranformation.
   */
  void SetTransform (const csReversibleTransform& t) { obj = t; }

  /**
   * Get the local world to object tranformation.
   */
  csReversibleTransform& GetTransform () { return obj; }

  /**
   * Get the local world to object tranformation.
   */
  const csReversibleTransform& GetTransform () const { return obj; }

  /**
   * Construct the full world to object transformation given
   * this transformation and possible parents transformations.
   */
  csReversibleTransform GetFullTransform () const;

  /**
   * Relative move.
   */
  void MovePosition (const csVector3& v);

  /**
   * Relative transform.
   */
  void Transform (const csMatrix3& matrix);

  /**
   * After all movement has been done you need to
   * call UpdateMove() to make the final changes to the entity
   * that is controlled by this movable. This is very important!
   */
  void UpdateMove ();

  DECLARE_IBASE;

  //------------------------- iMovable interface -------------------------------
  struct eiMovable : public iMovable
  {
    DECLARE_EMBEDDED_IBASE (csMovable);
    virtual iMovable* GetParent ()
    {
      return scfParent->iparent;
    }
    virtual void SetSector (iSector* sector);
    virtual void ClearSectors ()
    {
      scfParent->ClearSectors ();
    }
    virtual void AddSector (iSector* sector);
    virtual csVector& GetSectors ()
    {
      return scfParent->GetSectors ();
    }
    virtual iSector* GetSector (int idx);
    virtual bool InSector ()
    {
      return scfParent->InSector ();
    }
    virtual void SetPosition (iSector* home, const csVector3& v);
    virtual void SetPosition (const csVector3& v)
    {
      scfParent->SetPosition (v);
    }
    virtual const csVector3& GetPosition ()
    {
      return scfParent->GetPosition ();
    }
    virtual const csVector3 GetFullPosition ()
    {
      return scfParent->GetFullPosition ();
    }
    virtual void SetTransform (const csMatrix3& matrix)
    {
      scfParent->SetTransform (matrix);
    }
    virtual void SetTransform (const csReversibleTransform& t)
    {
      scfParent->SetTransform (t);
    }
    virtual csReversibleTransform& GetTransform ()
    {
      return scfParent->GetTransform ();
    }
    virtual csReversibleTransform GetFullTransform ()
    {
      return scfParent->GetFullTransform ();
    }
    virtual void MovePosition (const csVector3& v)
    {
      scfParent->MovePosition (v);
    }
    virtual void Transform (csMatrix3& matrix)
    {
      scfParent->Transform (matrix);
    }
    virtual void UpdateMove ()
    {
      scfParent->UpdateMove ();
    }
  } scfiMovable;
  friend struct eiMovable;
};

#endif // __CS_MOVABLE_H__
