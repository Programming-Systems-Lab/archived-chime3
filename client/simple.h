/*
    Copyright (C) 1998-2000 by Jorrit Tyberghein

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

#ifndef SIMPLE_H
#define SIMPLE_H

#include <stdarg.h>
#include "cssys/sysdriv.h"
#include "csgeom/math2d.h"
#include "csgeom/math3d.h"

struct iMeshFactoryWrapper;
struct iMeshWrapper;
class csSector;
class csView;
class csEngine;
class csDynLight;
class csProcDots;
class csProcPlasma;
class csProcWater;
class csProcFire;
class csMaterialWrapper;

class Simple : public SysSystemDriver
{
  typedef SysSystemDriver superclass;
private:
  csSector* room;
  csView* view;
  csEngine* engine;
  csDynLight* dynlight;
  iMeshWrapper* cube;
  iMeshWrapper* ball;
  csMaterialWrapper* matPlasma;
  float angle;
  int motion_flags;

  iMeshFactoryWrapper* cube_factory;
  iMeshFactoryWrapper* ball_factory;

  csProcDots* prDots;
  csProcPlasma* prPlasma;
  csProcFire* prFire;
  csProcWater* prWater;

  bool InitProcDemo ();

public:
  Simple ();
  virtual ~Simple ();

  virtual bool Initialize (int argc, const char* const argv[],
    const char *iConfigName);
  virtual void NextFrame ();
  virtual bool HandleEvent (iEvent &Event);
};

#endif // SIMPLE_H

