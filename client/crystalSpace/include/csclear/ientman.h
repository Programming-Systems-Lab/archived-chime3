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

#ifndef __IENTITYMANAGER_H__
#define __IENTITYMANAGER_H__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/csvector.h"
#include "iplugin.h"

struct iEntity;

/** The base associated data structure.  All associated data structures derive
 *  from this one.
 */
struct AssocData 
{
  /** The signature is calculated from the following (each field is one byte 
   *  in length unless noted otherwise, in case of overflow just wraparound):
   *  Size of the complete structure, including superclasses in bits (2 bytes!),
   *  the ASCII code of the second character of the name
   *  and a random number from 0 to 255 formatted like this:
   *  { StructSize byte 1, StructSize byte 2, second character, random number }
   *  So for example, this one's signature would be 0x004073FF
   */
  ULong signature;

  /// The entity associated with this data
  iEntity *entity;

};

/** Entity data
 *  This defines whether an entity is on the render list.  All active entities
 *  should be on this list even if they are never rendered.
 */
#define ccEntityList 0x00606E2E
struct EntityAssocData : public AssocData
{
  bool rendered;
};

DECLARE_TYPED_VECTOR(AssocVectorBase, AssocData);

/// This vector class is designed to help manage lists of associated entities
class AssocVector : public AssocVectorBase
{
public:
  /// Find an given entity
  int FindEntity(const iEntity *entity) const {
    int i;
    for(i = 0; i < Length(); i++)
      if(REINTERPRET_CAST(AssocData*, root[i])->entity == entity)
	return i;
    return -1;
  }
};

/**
 * The iEntityManager is the interface for registering CrystalClear entities
 * for use by various ImpulseTargets.
 */
SCF_VERSION (iEntityManager, 0, 0, 3);
struct iEntityManager : public iPlugIn
{
  /** Called by appliction on NextFrame().  This should not be called by
   * anything else.  Tells each perceivable (visible, audible, etc.) 
   * entities to prepare their data for rendering.
   */
  virtual void PrepareFrame(cs_time elapsed_time, cs_time current_time) = 0;

  /// Associate an entity with data specified by a given signature
  virtual void Associate(AssocData *data) = 0;

  /// Remove the association of an entity and it's data.  Deletes the data in
  /// the process.
  virtual void Desociate(AssocData *data) = 0;

  /// Retrieve a list of entities and their data associated with a given signature
  virtual AssocVector *GetAssociated(ULong signature) = 0;

  /// Get the data of an entity for a given signature
  virtual AssocData *GetAssocData(ULong signature, const iEntity *entity) = 0;

};

#endif // __IENTITYMANAGER_H__
