/*
    Crystal Space 3D engine
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

#ifndef __ICOLLIDE_H__
#define __ICOLLIDE_H__

#include "csutil/scf.h"
#include "csgeom/vector3.h"
#include "iplugin.h"

struct iPolygonMesh;
class csTransform;

SCF_VERSION (iCollider, 0, 1, 0);

/**
 * A structure used to return collision pairs.
 */
struct csCollisionPair
{
  csVector3 a1, b1, c1;	// First triangle
  csVector3 a2, b2, c2;	// Second triangle
};

/**
 * A collider.
 */
struct iCollider : public iBase
{
  /// Return a reference to the polygon mesh for this collider.
  virtual iPolygonMesh* GetGeometry () = 0;
};

SCF_VERSION (iCollideSystem, 0, 0, 1);

/**
 * This is the Collide plug-in. This plugin is a factory for creating
 * iCollider entities. A collider represents an entity in the
 * collision detection world. It uses the geometry data as given by
 * iPolygonMesh.
 */
struct iCollideSystem : public iPlugIn
{
  /// Create an iCollider for the given geometry.
  virtual iCollider* CreateCollider (iPolygonMesh* mesh) = 0;

  /**
   * Test collision between two colliders.
   * This is only supported for iCollider objects created by
   * this plugin. Returns false if no collision or else true.
   * The collisions will be added to the collision pair array
   * that you can query with GetCollisionPairs and reset/clear
   * with ResetCollisionPairs. Every call to Collide will
   * add to that array.
   */
  virtual bool Collide (iCollider* collider1, const csTransform* trans1,
  	iCollider* collider2, const csTransform* trans2) = 0;

  /**
   * Get pointer to current array of collision pairs.
   * This array will grow with every call to Collide until you clear
   * it using 'ResetCollisionPairs'.
   */
  virtual csCollisionPair* GetCollisionPairs () = 0;

  /**
   * Get number of collision pairs in array.
   */
  virtual int GetNumCollisionPairs () = 0;

  /**
   * Reset the array with collision pairs.
   */
  virtual void ResetCollisionPairs () = 0;

  /**
   * Indicate if we are interested only in the first hit that is found.
   * This is only valid for CD algorithms that actually allow the
   * detection of multiple CD hit points.
   */
  virtual void SetOneHitOnly (bool o) = 0;

  /**
   * Return true if this CD system will only return the first hit
   * that is found. For CD systems that support multiple hits this
   * will return the value set by the SetOneHitOnly() function.
   * For CD systems that support one hit only this will always return true.
   */
  virtual bool GetOneHitOnly () = 0;
};

#endif // __ICOLLIDE_H__

