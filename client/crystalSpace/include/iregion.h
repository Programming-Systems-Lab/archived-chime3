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

#ifndef __IREGION_H__
#define __IREGION_H__

#include "csutil/scf.h"

struct iSector;
struct iThing;
struct iSprite;
struct iSpriteTemplate;
struct iMeshWrapper;
struct iMeshFactoryWrapper;
struct iTextureWrapper;
struct iMaterialWrapper;
struct iCameraPosition;
struct iObject;

SCF_VERSION (iRegion, 0, 1, 5);

/**
 * A region. A region is basically a collection of objects in the
 * 3D engine that can be treated as a unit.
 */
struct iRegion : public iBase
{
  /**
   * Clear this region without removing the entities in it. The entities
   * will simply get unconnected from this region.
   */
  virtual void Clear () = 0;

  /**
   * Delete all entities in this region.
   */
  virtual void DeleteAll () = 0;

  /**
   * Prepare all textures and materials in this region.
   */
  virtual bool PrepareTextures () = 0;

  /**
   * Prepare all sectors in this region.
   */
  virtual bool PrepareSectors () = 0;

  /**
   * Do lighting calculations (or read from cache).
   */
  virtual bool ShineLights () = 0;

  /**
   * Prepare all objects in this region. This has to be called
   * directly after loading new objects.
   * This function is equivalent to calling PrepareTextures()
   * followed by PrepareSectors().
   */
  virtual bool Prepare () = 0;

  /// Find a sector in this region by name.
  virtual iSector *FindSector (const char *iName) = 0;
  /// Find a thing in this region by name
  virtual iThing *FindThing (const char *iName) = 0;
  /// Find a sky thing in this region by name
  virtual iThing *FindSky (const char *iName) = 0;
  /// Find a thing template in this region by name
  virtual iThing *FindThingTemplate (const char *iName) = 0;
  /// Find a sprite in this region by name
  virtual iMeshWrapper *FindMeshObject (const char *iName) = 0;
  /// Find a mesh factory in this region by name
  virtual iMeshFactoryWrapper *FindMeshFactory (const char *iName) = 0;
  /// Find a texture in this region by name
  virtual iTextureWrapper *FindTexture (const char *iName) = 0;
  /// Find a material in this region by name
  virtual iMaterialWrapper *FindMaterial (const char *iName) = 0;
  /// Find a camera position in this region by name
  virtual iCameraPosition *FindCameraPosition (const char *iName) = 0;

  /**
   * Check if some object is in this region.
   * The speed of this function is independent of the number of
   * objects in this region (i.e. very fast).
   */
  virtual bool IsInRegion (iObject* obj) = 0;
};

#endif // __IREGION_H__
