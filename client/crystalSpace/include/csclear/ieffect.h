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

#ifndef __IEFFECT_H__
#define __IEFFECT_H__

#include "csutil/scf.h"
#include "csclear/ientity.h"

SCF_VERSION (iEffect, 0, 0, 1);

/***
 * Subclasses of this entity are special effects, such as smoke,
 * explosions, pools of liquids, etc.
 */
struct iEffect : public iEntity
{
  // This class doesn't contain anything special, it's just a logical class
  // holder.  That may change in the future, however.
}

#endif // ! __IEFFECT_H__
