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

#ifndef __ITRIGGER_H__
#define __ITRIGGER_H__

#include "csutil/scf.h"
#include "csclear/ientity.h"

SCF_VERSION (iTrigger, 0, 0, 1);

/***
 * Subclasses of this entity are spaces or things in the world which react
 * to a player or monsters presence or action.
 */
struct iTrigger : public iEntity
{
  // No new methods, but the code for this class generates ccTriggerImpulses
  // May add specialized methods later
}

#endif // ! __ITRIGGER_H__
