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

#include "cssysdef.h"
#include "cssys/system.h"
#include "simple.h"
#include "csengine/sector.h"
#include "csengine/engine.h"
#include "csengine/csview.h"
#include "csengine/camera.h"
#include "csengine/light.h"
#include "csengine/polygon.h"
#include "csengine/meshobj.h"
#include "csengine/texture.h"
#include "csengine/thing.h"
#include "csfx/proctex.h"
#include "csfx/prdots.h"
#include "csfx/prplasma.h"
#include "csfx/prfire.h"
#include "csfx/prwater.h"
#include "csparser/csloader.h"
#include "igraph3d.h"
#include "itxtmgr.h"
#include "iconsole.h"
#include "imeshobj.h"
#include "imcube.h"
#include "imball.h"
#include "imspr3d.h"

//------------------------------------------------- We need the 3D engine -----

REGISTER_STATIC_LIBRARY (engine)

//-----------------------------------------------------------------------------

Simple::Simple ()
{
  view = NULL;
  engine = NULL;
  dynlight = NULL;
  cube = NULL;
  ball = NULL;
  motion_flags = 0;
  prDots = NULL;
  prPlasma = NULL;
  prWater = NULL;
  prFire = NULL;
  cube_factory = NULL;
  ball_factory = NULL;
}

Simple::~Simple ()
{
  delete view;
  delete prDots;
  delete prPlasma;
  delete prWater;
  delete prFire;
  cube->DecRef ();
  ball->DecRef ();
}

void cleanup ()
{
  System->console_out ("Cleaning up...\n");
  delete System;
}

bool Simple::InitProcDemo ()
{
  iTextureManager* txtmgr = G3D->GetTextureManager ();
  prDots = new csProcDots ();
  csMaterialWrapper* matDots = prDots->Initialize (this, engine, txtmgr, "dots");
  prPlasma = new csProcPlasma ();
  matPlasma = prPlasma->Initialize (this, engine, txtmgr, "plasma");
  prWater = new csProcWater ();
  csMaterialWrapper* matWater = prWater->Initialize (this, engine, txtmgr, "water");
  prFire = new csProcFire ();
  csMaterialWrapper* matFire = prFire->Initialize (this, engine, txtmgr, "fire");

  csThing* thing = new csThing (engine);
  engine->things.Push (thing);
  thing->SetName ("procCube");
  float dx = 1, dy = 1, dz = 1;
  csPolygon3D* p;
  p = thing->NewPolygon (matDots);
  p->AddVertex (+dx, +dy, -dz);
  p->AddVertex (-dx, +dy, -dz);
  p->AddVertex (-dx, +dy, +dz);
  p->AddVertex (+dx, +dy, +dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), .5);

  p = thing->NewPolygon (matPlasma);
  p->AddVertex (+dx, -dy, +dz);
  p->AddVertex (-dx, -dy, +dz);
  p->AddVertex (-dx, -dy, -dz);
  p->AddVertex (+dx, -dy, -dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 1);

  p = thing->NewPolygon (matFire);
  p->flags.SetBool (CS_POLY_LIGHTING, false);
  p->AddVertex (-dx, +dy, -dz);
  p->AddVertex (+dx, +dy, -dz);
  p->AddVertex (+dx, -dy, -dz);
  p->AddVertex (-dx, -dy, -dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 2);

  p = thing->NewPolygon (matWater);
  p->flags.SetBool (CS_POLY_LIGHTING, false);
  p->AddVertex (-dx, -dy, -dz);
  p->AddVertex (-dx, -dy, +dz);
  p->AddVertex (-dx, +dy, +dz);
  p->AddVertex (-dx, +dy, -dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 1);

  p = thing->NewPolygon (matDots);
  p->AddVertex (+dx, -dy, +dz);
  p->AddVertex (+dx, -dy, -dz);
  p->AddVertex (+dx, +dy, -dz);
  p->AddVertex (+dx, +dy, +dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), .5);

  p = thing->NewPolygon (matDots);
  p->AddVertex (-dx, -dy, +dz);
  p->AddVertex (+dx, -dy, +dz);
  p->AddVertex (+dx, +dy, +dz);
  p->AddVertex (-dx, +dy, +dz);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), .5);

  thing->GetMovable ().SetSector (room);
  thing->GetMovable ().SetPosition (csVector3 (2, 7, 1));
  thing->GetMovable ().UpdateMove ();
  
  thing->Prepare (room);
  thing->InitLightMaps (false);
  room->ShineLights (thing);
  thing->CreateLightMaps (G3D);
  return true;
}

bool Simple::Initialize (int argc, const char* const argv[],
  const char *iConfigName)
{
  if (!superclass::Initialize (argc, argv, iConfigName))
    return false;

  // Find the pointer to engine plugin
  iEngine *Engine = QUERY_PLUGIN (this, iEngine);
  if (!Engine)
  {
    CsPrintf (MSG_FATAL_ERROR, "No iEngine plugin!\n");
    abort ();
  }
  engine = Engine->GetCsEngine ();
  Engine->DecRef ();

  // Open the main system. This will open all the previously loaded plug-ins.
  if (!Open ("Simple Crystal Space Application"))
  {
    Printf (MSG_FATAL_ERROR, "Error opening system!\n");
    cleanup ();
    exit (1);
  }

  // Setup the texture manager
  iTextureManager* txtmgr = G3D->GetTextureManager ();
  txtmgr->SetVerbose (true);

  // Initialize the texture manager
  txtmgr->ResetPalette ();
  
  // Allocate a uniformly distributed in R,G,B space palette for console
  // The console will crash on some platforms if this isn't initialize properly
  int r,g,b;
  for (r = 0; r < 8; r++)
    for (g = 0; g < 8; g++)
      for (b = 0; b < 4; b++)
	txtmgr->ReserveColor (r * 32, g * 32, b * 64);
  txtmgr->SetPalette ();

  // Some commercials...
  Printf (MSG_INITIALIZATION,
    "Simple Crystal Space Application version 0.1.\n");

  // First disable the lighting cache. Our app is simple enough
  // not to need this.
  engine->EnableLightingCache (false);

  // Create our world.
  Printf (MSG_INITIALIZATION, "Creating world!...\n");

  // Get the cube mesh object plugin.
  cube_factory = engine->CreateMeshFactory ("crystalspace.mesh.object.cube", "cube_factory");
  if (!cube_factory) Printf (MSG_WARNING, "Could not create cube factory!\n");

  // Get the ball mesh object plugin.
  ball_factory = engine->CreateMeshFactory ("crystalspace.mesh.object.ball", "ball_factory");
  if (!ball_factory) Printf (MSG_WARNING, "Could not create ball factory!\n");

  csLoader::LoadTexture (engine, "stone", "/lib/std/stone4.gif");
  csMaterialWrapper* tm = engine->GetMaterials ()->FindByName ("stone");

  room = engine->CreateCsSector ("room");
  csPolygon3D* p;
  p = room->NewPolygon (tm);
  p->AddVertex (-5, 0, 5);
  p->AddVertex (5, 0, 5);
  p->AddVertex (5, 0, -5);
  p->AddVertex (-5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = room->NewPolygon (tm);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (5, 20, -5);
  p->AddVertex (5, 20, 5);
  p->AddVertex (-5, 20, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = room->NewPolygon (tm);
  p->AddVertex (-5, 20, 5);
  p->AddVertex (5, 20, 5);
  p->AddVertex (5, 0, 5);
  p->AddVertex (-5, 0, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = room->NewPolygon (tm);
  p->AddVertex (5, 20, 5);
  p->AddVertex (5, 20, -5);
  p->AddVertex (5, 0, -5);
  p->AddVertex (5, 0, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = room->NewPolygon (tm);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (-5, 20, 5);
  p->AddVertex (-5, 0, 5);
  p->AddVertex (-5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = room->NewPolygon (tm);
  p->AddVertex (5, 20, -5);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (-5, 0, -5);
  p->AddVertex (5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  csStatLight* light;
  light = new csStatLight (-3, 5, 0, 10, 1, 0, 0, false);
  room->AddLight (light);
  light = new csStatLight (3, 5, 0, 10, 0, 0, 1, false);
  room->AddLight (light);
  light = new csStatLight (0, 5, -3, 10, 0, 1, 0, false);
  room->AddLight (light);

  engine->Prepare ();

  // Create a -dynamic light.
  angle = 0;
  dynlight = new csDynLight (cos (angle)*3, 17, sin (angle)*3, 7, 1, 0, 0);
  engine->AddDynLight (dynlight);
  dynlight->SetSector (room);
  dynlight->Setup ();

  Printf (MSG_INITIALIZATION, "--------------------------------------\n");

  // csView is a view encapsulating both a camera and a clipper.
  // You don't have to use csView as you can do the same by
  // manually creating a camera and a clipper but it makes things a little
  // easier.
  view = new csView (engine, G3D);
  view->SetSector (room);
  view->GetCamera ()->SetPosition (csVector3 (0, 5, -3));
  view->SetRectangle (0, 0, FrameWidth, FrameHeight);

  txtmgr->SetPalette ();

  // Load a texture for our sprite.
  csTextureWrapper* txt = csLoader::LoadTexture (engine, "spark",
  	"/lib/std/spark.png");
  if (txt == NULL)
  {
    Printf (MSG_FATAL_ERROR, "Error loading texture!\n");
    cleanup ();
    exit (1);
  }
  txt->Register (txtmgr);
  txt->GetTextureHandle ()->Prepare ();
  csMaterialWrapper* mat = engine->GetMaterials ()->FindByName ("spark");
  mat->Register (txtmgr);
  mat->GetMaterialHandle ()->Prepare ();

  // Load a sprite template from disk.
  csMeshFactoryWrapper* spritetmpl = csLoader::LoadMeshObjectFactory (engine,
  	"/lib/std/sprite1");
  if (spritetmpl == NULL)
  {
    Printf (MSG_FATAL_ERROR, "Error loading mesh object factory!\n");
    cleanup ();
    exit (1);
  }

  iSector* iroom = QUERY_INTERFACE (room, iSector);

  // Add the sprite to the engine.
  iMeshFactoryWrapper* imeshfact = QUERY_INTERFACE (spritetmpl, iMeshFactoryWrapper);
  iMeshWrapper* sprite = engine->CreateMeshObject (
  	imeshfact, "MySprite", iroom,
	csVector3 (-3, 5, 3));
  csMatrix3 m; m.Identity (); m *= 5.;
  sprite->GetMovable ()->SetTransform (m);
  sprite->GetMovable ()->UpdateMove ();
  iSprite3DState* spstate = QUERY_INTERFACE (sprite->GetMeshObject (), iSprite3DState);
  spstate->SetAction ("default");
  imeshfact->DecRef ();
  spstate->DecRef ();

  InitProcDemo ();

  iMaterialWrapper* imatPlasma = QUERY_INTERFACE (matPlasma, iMaterialWrapper);

  // Make a cube using the cube plugin.
  iCubeFactoryState* cubeLook = QUERY_INTERFACE (cube_factory->GetMeshObjectFactory (),
  	iCubeFactoryState);
  cubeLook->SetSize (.5, .5, .5);
  cubeLook->SetMaterialWrapper (imatPlasma);
  cubeLook->SetMixMode (CS_FX_ADD);
  cubeLook->DecRef ();

  cube = engine->CreateMeshObject (cube_factory, "MyCube",
  	iroom, csVector3 (-3, -5, -3));
  cube->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);

  // Make a ball using the ball plugin.
  ball = engine->CreateMeshObject (ball_factory, "MyBall",
  	iroom, csVector3 (-3, 5, -3));
  iBallState* ballLook = QUERY_INTERFACE (ball->GetMeshObject (), iBallState);
  ballLook->SetRadius (.5, .5, .5);
  ballLook->SetMaterialWrapper (imatPlasma);
  ballLook->SetRimVertices (12);
  ball->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
  ballLook->DecRef ();

  imatPlasma->DecRef ();
  iroom->DecRef ();

  // @@@ DEBUG: IF THIS IS REMOVED THE SPRITE CRASHES!
  engine->Prepare ();

  return true;
}

void Simple::NextFrame ()
{
  SysSystemDriver::NextFrame ();
  cs_time elapsed_time, current_time;
  GetElapsedTime (elapsed_time, current_time);

  // Now rotate the camera according to keyboard state
  float speed = (elapsed_time / 1000.) * (0.03 * 20);

  if (GetKeyState (CSKEY_RIGHT))
    view->GetCamera ()->Rotate (VEC_ROT_RIGHT, speed);
  if (GetKeyState (CSKEY_LEFT))
    view->GetCamera ()->Rotate (VEC_ROT_LEFT, speed);
  if (GetKeyState (CSKEY_PGUP))
    view->GetCamera ()->Rotate (VEC_TILT_UP, speed);
  if (GetKeyState (CSKEY_PGDN))
    view->GetCamera ()->Rotate (VEC_TILT_DOWN, speed);
  if (GetKeyState (CSKEY_UP))
    view->GetCamera ()->Move (VEC_FORWARD * 4.0f * speed);
  if (GetKeyState (CSKEY_DOWN))
    view->GetCamera ()->Move (VEC_BACKWARD * 4.0f * speed);

  // Move the -dynamic light around.
  angle += elapsed_time * 0.4 / 1000.;
  while (angle >= 2.*3.1415926) angle -= 2.*3.1415926;
  csVector3 pos (cos (angle)*3, 17, sin (angle)*3);
  dynlight->Move (room, pos.x, pos.y, pos.z);
  dynlight->Setup ();

  // Move the cube.
  pos.y = 5;
  cube->GetMovable ()->SetPosition (pos);
  cube->GetMovable ()->UpdateMove ();
  cube->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);

  // Move the ball.
  pos.y = 4;
  ball->GetMovable ()->SetPosition (pos);
  ball->GetMovable ()->UpdateMove ();
  ball->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);

  // Tell 3D driver we're going to display 3D things.
  if (!G3D->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
    return;

  view->Draw ();

  // Start drawing 2D graphics.
  if (!G3D->BeginDraw (CSDRAW_2DGRAPHICS)) return;

  // Drawing code ends here.
  G3D->FinishDraw ();
  // Print the final output.
  G3D->Print (NULL);
}

bool Simple::HandleEvent (iEvent &Event)
{
  if (superclass::HandleEvent (Event))
    return true;

  if ((Event.Type == csevKeyDown) && (Event.Key.Code == CSKEY_ESC))
  {
    Shutdown = true;
    return true;
  }

  return false;
}

/*---------------------------------------------------------------------*
 * Main function
 *---------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  srand (time (NULL));

  // Create our main class.
  System = new Simple ();

  // We want at least the minimal set of plugins
/*  System->RequestPlugin ("crystalspace.kernel.vfs:VFS");
  System->RequestPlugin ("crystalspace.font.server.default:FontServer");
  System->RequestPlugin ("crystalspace.graphics3d.software:VideoDriver");
  System->RequestPlugin ("crystalspace.engine.core:Engine");
  System->RequestPlugin ("crystalspace.console.output.standard:Console");
*/
  // Initialize the main system. This will load all needed plug-ins
  // (3D, 2D, network, sound, ...) and initialize them.
  //if (!System->Initialize (argc, argv, NULL))
  if (!System->Initialize (argc, argv, "/config/cryst.cfg"))
  {
    System->Printf (MSG_FATAL_ERROR, "Error initializing system!\n");
    cleanup ();
    exit (1);
  }

  // Main loop.
  System->Loop ();

  // Cleanup.
  cleanup ();

  return 0;
}
