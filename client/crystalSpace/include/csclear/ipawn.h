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

#ifndef __CC_IPAWN_H__
#define __CC_IPAWN_H__

#include "csutil/scf.h"
#include "csclear/ccactor.h"

struct iActor;

// Information for an actor bound to a pawn
struct ActorInfo {
  iActor *external;   // The original actor pointer (required for AddActor())
  iActor *proxy;      // The proxy actor returned by AddActor() (must be NULL for AddActor())
  csVector3 *actorpt; // A point in actor space that corresponds to pawnpt
  csVector3 *pawnpt;  // A point in pawn space that corresponds to actorpt
  csVector3 *dir;     // The direction of the actor in pawn space
  csVector3 *up;      // The up vector of the actor in pawn space
};

/// Vector of actor info
DECLARE_TYPED_VECTOR(ActorVectorBase, ActorInfo);
class ActorVector : public ActorVectorBase {
public:
  int FindExtActor(iActor *actor) const {
    int i;
    for(i = 0; i < Length(); i++)
      if(REINTERPRET_CAST(ActorInfo*, root[i])->external==actor)
	return i;
    return -1;
  }

  int FindProxy(iActor *actor) const {
    int i;
    for(i = 0; i < Length(); i++)
      if(REINTERPRET_CAST(ActorInfo*, root[i])->proxy==actor)
	return i;
    return -1;
  }
};

/**
 * Subclasses of this entity are characters in the world, including the player
 * and beings he/she/it interacts with.
 */
SCF_VERSION (iPawn, 0, 0, 3);
struct iPawn : public ccActor
{

  iPawn(iSystem *system) : ccActor(system) { }

  // Add an actor to the list of internal actors
  virtual iActor *AddActor(ActorInfo *info) = 0;

  // Remove an actor from the list of internal actors
  virtual void RemoveActor(iActor *actor) = 0;

  // Retrieve the list of internal actors
  virtual const ActorVector *GetActorVector() const = 0;

};

#endif // ! __CC_IPAWN_H__
