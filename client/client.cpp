//*******************************************************************
//*
//* Implementation of Client class.
//*
//*******************************************************************

#include "cssysdef.h"
#include "cssys/system.h"
#include "client.h"

#include "csengine/engine.h"
#include "csengine/csview.h"
#include "csengine/sector.h"
#include "csengine/polygon.h"
#include "csparser/csloader.h"
#include "csparser/crossbld.h"


#include "imspr3d.h"
#include "itxtmgr.h"

//------------------------------------------------- We need the 3D engine -----

REGISTER_STATIC_LIBRARY (engine)

//-----------------------------------------------------------------------------

//Shutdown the engine.
void Cleanup()
{
	System->console_out ("Cleaning up...\n");
	delete System;
}

Client::Client()
{
	engine = NULL;
	curSector = NULL;
	freeLook = false;
	camAngle.Set(0, 0, 0);
}

Client::~Client()
{
	
}

//**********************************************************************
//*
//* This function loads all the necessary plugins used by 
//* Crystal-Space engine.
//*
//**********************************************************************
bool Client::Initialize(int argc, const char *const argv[], const char *iConfigName)
{
	srand (time (NULL));

	//Initialize basic things that crystal-space must need.
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
	if (!Open ("Chime Client 0.1"))
	{
		Printf (MSG_FATAL_ERROR, "Error opening system!\n");
		Cleanup ();
		exit (1);
	}

	//Turn of light caching
	engine->EnableLightingCache (false);
	//Load chime objects and textures
	LoadChimeLib("chimelib.txt");
	
	//Temporary . Must be removed	  
	Printf (MSG_INITIALIZATION, "Building  Sector 1!...\n");
	curSector = new chimeSector(System, engine);
	curSector->ReadRoom("room1.txt");
	curSector->ReadRoom("room2.txt");
	curSector->ReadRoom("room3.txt");

/**/
	csSector *room1 = curSector->GetRoom(0);	
	csSector *room2 = curSector->GetRoom(1);
	csSector *room3 = curSector->GetRoom(2);

	csPolygon3D *p1 = room1->GetPolygon3D(4);
	csPolygon3D *p2 = room2->GetPolygon3D(7);
	csPolygon3D *p3 = room3->GetPolygon3D(4);
	csPolygon3D *p4 = room3->GetPolygon3D(5);

	p1->SetCSPortal(room3);
	p1->SetAlpha(0);
	p4->SetCSPortal(room1);
	p4->SetAlpha(0);

	p2->SetCSPortal(room3);
	p2->SetAlpha(0);
	p3->SetCSPortal(room2);
	p3->SetAlpha(0);



	// csView is a view encapsulating both a camera and a clipper.
	// You don't have to use csView as you can do the same by
	// manually creating a camera and a clipper but it makes things a little
	// easier.
	view = new csView (engine, G3D);
	view->SetSector (curSector->GetRoom(0));
	view->GetCamera ()->SetPosition (csVector3 (0, 2, -3));
	view->SetRectangle (0, 0, FrameWidth, FrameHeight);
	
	engine->Prepare ();	
	
	return true;
}

//**********************************************************************
//*
//* Function responsible for preparing the next frame.
//* to be displayed.
//*
//**********************************************************************
void Client::NextFrame ()
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

//**********************************************************************
//*
//* Transport user from one sector to a another sector.
//*
//**********************************************************************
bool Client::Transport(csSector *room, csVector3 pos, csVector3 lookPos, csVector3 lookUp)
{	
	if(!room)
		return false;

	view->SetSector (room);
	view->GetCamera ()->SetPosition (pos);
	view->GetCamera ()->LookAt(lookPos, lookUp);
	view->SetRectangle (0, 0, FrameWidth, FrameHeight);
	
	return true;
}

//**********************************************************************
//*
//* Function responsible for handling keyboard events only
//* 
//**********************************************************************
bool Client::HandleKeyEvent (iEvent &Event)
{
	//Camera speed
	
    //cs_time elapsed_time, current_time;
    //GetElapsedTime (elapsed_time, current_time);  
    //float moveSpeed = 5*(elapsed_time / 1000.) * (0.03 * 20);	 
	float moveSpeed = 0.1;	 

	switch (Event.Type)
	{	
	case csevKeyUp:
		break;      
	case csevKeyDown:	
		if(Event.Key.Code == CSKEY_ESC)  
		{
			Shutdown = true;
			return true;
		}
		if(Event.Key.Code == 'a')  
		{
			view->GetCamera ()->Move (VEC_LEFT *  moveSpeed);
			return true;
		}
		if(Event.Key.Code == 'd')  
		{
			view->GetCamera ()->Move (VEC_RIGHT *  moveSpeed);
			return true;
		}
		if(Event.Key.Code == 'w')  
		{
			view->GetCamera ()->Move (VEC_FORWARD *  moveSpeed);
			return true;
		}
		if(Event.Key.Code == 's')  
		{
			view->GetCamera ()->Move (VEC_BACKWARD *  moveSpeed);
			return true;
		}
		if(Event.Key.Code == 'f')  
		{
			if( freeLook ) freeLook = false;
			else freeLook = true;
			return true;
		}
  

 
//    view->GetCamera ()->Rotate (VEC_ROT_RIGHT, speed);

		break;
	}

	return false;
}
		
//**********************************************************************
//*
//* Function responsible for handling all the mouse, 
//* keyboard and broadcast events.
//*
//**********************************************************************
bool Client::HandleEvent (iEvent &Event)
{
	//Check if the event is for the superclass
	if (superclass::HandleEvent (Event))
		return true;
	
	
	switch (Event.Type)
	{
	case csevBroadcast:	  
		break;   
	case csevMouseDown:
		break;	  
	case csevMouseUp:      
		break;
	case csevMouseMove: 
		{
			static bool first_time = true;
			if (freeLook)
			{
				int last_x, last_y;
				last_x = Event.Mouse.x;
				last_y = Event.Mouse.y;
				
				System->G2D->SetMousePosition (FrameWidth / 2, FrameHeight / 2);
				if (!first_time)
				{
					camAngle.x =((float)(last_x - (FrameWidth / 2) )) / (FrameWidth*2);
					camAngle.y =(((float)(last_y - (FrameHeight / 2) )) / (FrameHeight*2)*(1-2*(int)1));
					
					view->GetCamera ()->Rotate (VEC_ROT_RIGHT, 3*camAngle.x );
					view->GetCamera ()->Rotate (VEC_TILT_UP, 3*camAngle.y );
					
					//view->GetCamera ()->RotateWorld (csVector3 (0,1,0), 3*camAngle.x);
					//view->GetCamera ()->RotateWorld (csVector3 (1,0,0), 3*camAngle.y);
					/*
					if(move_3d)
					view->GetCamera ()->Rotate (VEC_ROT_RIGHT, ((float)( last_x - (FRAME_WIDTH / 2) )) / (FRAME_WIDTH*2) );
					else
					view->GetCamera ()->RotateWorld (VEC_ROT_RIGHT, ((float)( last_x - (FRAME_WIDTH / 2) )) / (FRAME_WIDTH*2) );
					view->GetCamera ()->Rotate (VEC_TILT_UP, -((float)( last_y - (FRAME_HEIGHT / 2) )) / (FRAME_HEIGHT*2) );
					*/
					
					//this->angle.y+=((float)(last_x - (FRAME_WIDTH / 2) )) / (FRAME_WIDTH*2);
					//this->angle.x+=((float)(last_y - (FRAME_HEIGHT / 2) )) / (FRAME_HEIGHT*2)*(1-2*(int)inverse_mouse);
				}
				else
					first_time = false;
			}
			else
				first_time = true;
		}
		break;
	case csevKeyUp:
		break;      
	case csevKeyDown:
		HandleKeyEvent(Event);
		if(Event.Key.Code == CSKEY_ESC)  
		{
			Shutdown = true;
			return true;
		}
		if(Event.Key.Code == 116)
		{
			csSector *room = curSector->GetRoom(1);	
			
			if(room == NULL)
			{
				Printf (MSG_INITIALIZATION, "Building  New room\n");
				curSector->ReadRoom("room2.txt");
			}

			room = curSector->GetRoom(1);	
			
			Transport(room, csVector3(0,3, 15), csVector3(0,0, -1), csVector3(0,-1, 0));

			return true;
		}
		if(Event.Key.Code == 117)
		{
			csSector *room = curSector->GetRoom(0);
			
			Transport(room, csVector3(0,2.5, -3), csVector3(0,0, 1), csVector3(0,-1, 0));
			
			return true;
		}

		if(Event.Key.Code == 118)
		{
			csSector *room = curSector->GetRoom(0);
			
			csPolygon3D *p = room->GetPolygon3D(0); 
			csMaterialWrapper* mat = engine->GetMaterials ()->FindByName ("marble");
			p->SetMaterial(mat);
			engine->Prepare();
		}

	}//Switch 
	return false;
}

//**********************************************************************
//*
//* Main function
//* This is where Chime client starts its execution.
//*
//**********************************************************************
 
int main (int argc, char* argv[])
{
  srand (time (NULL));

  // Create our main class.
  System = new Client();

  // Initialize the main system. This will load all needed plug-ins
  // (3D, 2D, network, sound, ...) and initialize them.
  if(!System->Initialize (argc, argv, "/config/chime.cfg"))
  {
    System->Printf (MSG_FATAL_ERROR, "Error initializing system!\n");
	Cleanup ();
    exit (1);
  }

  // Main loop.
  System->Loop ();

  // Cleanup.
  Cleanup();

  return 0;
}

//**********************************************************************
//*
//* Load chime objects and textures
//*
//********************************************************************** 
bool Client::LoadChimeLib(char *fileName)
{		
	//Initialize Texture manager
	iTextureManager* txtmgr = G3D->GetTextureManager ();
	txtmgr->SetVerbose (true);
	
	// Initialize the texture manager
	txtmgr->ResetPalette ();
	
	// Allocate a uniformly distributed in R,G,B space palette for console
	// The console will crash on some platforms if this isn't initialize properly
	int r,g,b;
	for (r = 0; r < 8; r++)
	{
		for (g = 0; g < 8; g++)
		{
			for (b = 0; b < 4; b++)
			{
				txtmgr->ReserveColor (r * 32, g * 32, b * 64);
			}
		}
	}

	//Read library from the file
	FILE *fp = fopen(fileName, "r");
	if(!fp) return false;
	int bufSize = 100;
	char buf[100];
	char command[100];
	char err[200];
	
	fgets(buf, bufSize, fp);
	sscanf(buf, "%s", command);

	while( !feof(fp) && strcmp(command, "END"))
	{
		if(command[0] == ';')
		{//Skip comments
		}
		else if(!strcmp(command, "TEXT"))
		{//Read texture 
			char name[100], file[100];
			sscanf(buf, "%s %s %s", command, name, file);
			if(!csLoader::LoadTexture (engine, name, file))
			{
				strcpy(err,"Error Loading Texture: ");
				strcat(err, name);
				Printf (MSG_FATAL_ERROR, err);
				return false;
			}
		}
		else if(!strcmp(command, "OBJ"))
		{//Read 3D object		
			char name[100], file[100], text[100];
			sscanf(buf, "%s %s %s %s", command, name, file, text);
			if(!LoadMeshObj (file, name, text))
			{
				strcpy(err,"Error Loading Object: ");
				strcat(err, name);
				Printf (MSG_FATAL_ERROR, err);
				return false;
			}
		}
		else if(!strcmp(command, "MOD"))
		{//Read 3D model
			char name[100], file[100], text[100];
			sscanf(buf, "%s %s %s %s", command, name, file, text);
			if(!LoadMeshObj (file, name, text))
			{
				strcpy(err,"Error Loading Model: ");
				strcat(err, name);
				Printf (MSG_FATAL_ERROR, err);
				return false;
			}
		}		

		fgets(buf, bufSize, fp);
		sscanf(buf, "%s", command);
	}
	
	txtmgr->PrepareMaterials();
	txtmgr->SetPalette ();
	engine->PrepareTextures();
	engine->PrepareMeshes();

	fclose(fp);

	return false;
}



//*********************************************************************************
//*
//* Load a mesh object factory from a general format (3DS, MD2, ...)
//* This creates a mesh object factory which you can then add using add_meshobj ().
//*
//*********************************************************************************

bool Client::LoadMeshObj (char *filename, char *templatename, char* txtname)
{
  // First check if the texture exists.
  if (!engine->GetMaterials ()->FindByName (txtname))
  {
    System->Printf (MSG_CONSOLE, "Can't find material '%s' in memory!\n", txtname);
    return false;
  }

  // read in the model file
  converter * filedata = new converter;
  if (filedata->ivcon (filename, true, false, NULL, System->VFS) == ERROR)
  {
    System->Printf (MSG_CONSOLE, "There was an error reading the data!\n");
    delete filedata;
    return false;
  }

  // convert data from the 'filedata' structure into a CS sprite template
  csCrossBuild_SpriteTemplateFactory builder;
  iMeshObjectFactory *result = (iMeshObjectFactory *)builder.CrossBuild (*filedata);
  delete filedata;

  // Add this sprite template to the engine.
  iSprite3DFactoryState* fstate = QUERY_INTERFACE (result, iSprite3DFactoryState);
  fstate->SetMaterialWrapper (engine->FindMaterial (txtname));
  fstate->DecRef ();

  csMeshFactoryWrapper* meshwrap = new csMeshFactoryWrapper (result);
  meshwrap->SetName (templatename);
  engine->mesh_factories.Push (meshwrap);

  return true;
}

//*********************************************************************************
//*
//* Add mesh object in a given room at a given location.
//*
//*********************************************************************************
iMeshWrapper* Client::AddMeshObj (char* tname, char* sname, csSector* where,
	csVector3 const& pos, float size)
{
  iMeshFactoryWrapper* tmpl = engine->FindMeshFactory (tname);
  if (!tmpl)
  {
    System->Printf (MSG_CONSOLE, "Unknown mesh factory '%s'!\n", tname);
    return NULL;
  }
  iSector* isector = QUERY_INTERFACE (where, iSector);
  iMeshWrapper* spr = engine->CreateMeshObject (tmpl, sname,
  	isector, pos);
  isector->DecRef ();
  csMatrix3 m; m.Identity (); m = m * size;
  spr->GetMovable ()->SetTransform (m);
  spr->GetMovable ()->UpdateMove ();

  spr->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
  return spr;
}

