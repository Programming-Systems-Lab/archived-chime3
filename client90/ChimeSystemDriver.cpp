//*******************************************************************
//*
//* Implementation of ChimeSystemDriver class.
//*
//*******************************************************************

#include "cssysdef.h"
#include "cssys/system.h"

#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ivideo/fontserv.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"

#include "ChimeApp.h"
#include "ChimeSystemDriver.h"
#include "ChimeWorldView.h"
#include "ChimeSector.h"
#include "ChimeInfoEvent.h"

#include "csengine/engine.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "csengine/sector.h"
#include "csengine/polygon.h"
#include "isound/loader.h"
#include "imesh/crossbld.h"

#include "imesh/sprite3d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/natwin.h"
#include "ivaria/collider.h"
#include "ivaria/perfstat.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "iutil/virtclk.h"
#include "ivideo/fontserv.h"
#include "iutil/plugin.h"
#include "cstool/mdldata.h"
#include "cstool/mdltool.h"
#include "imesh/mdlconv.h"
#include "imesh/crossbld.h"
#include "imap/parser.h"
#include "iengine/movable.h"
#include "iengine/motion.h"
#include <process.h>

extern ChimeSystemDriver *Sys;

//------------------------------------------------- We need the 3D engine -----
CS_IMPLEMENT_APPLICATION

// need to register the engine explicit here when not building static
#if !defined(CS_STATIC_LINKED)
SCF_REGISTER_STATIC_LIBRARY (engine)
#endif

//-----------------------------------------------------------------------------

//this is the method we register with the registry
static bool ChimeHandleEvent(iEvent& ev) {
	
	int i = 0;

	if (ev.Type == csevBroadcast && ev.Command.Code == cscmdProcess) {
		Sys->SetupFrame();
		return true;
	}

	else if (ev.Type == csevBroadcast && ev.Command.Code == cscmdFinalProcess) {
		Sys->FinishFrame();
		return true;
	}

	else
		return Sys ? Sys->HandleEvent(ev): false;
}

//Shutdown the engine.
/*
void Cleanup()
{
	System->console_out ("Cleaning up...\n");
	delete System;
}
*/

ChimeSystemDriver::ChimeSystemDriver()
{
	//create the Mutex object the first time we create this class
	hMutex=CreateMutex(NULL,FALSE,NULL); // create a mutex object with no name

	ChimeSystemDriver::app = NULL;
	engine = NULL;
	myG3D = NULL;
	myG2D = NULL;
	curSector = NULL;
	currentSector = NULL;
	
	//the menu wasn't drawn
	menu_drawn = false;
	menu = NULL;

	for(int i = 0; i < NUM_SECT; i++)
	{
		sector[i] = NULL;
	}
	curSector = nextSector = 0;

	//Default camera params
	freeLook = false;
	camAngle.Set(0, 0, 0);
	SPEED = 2;

	selectedMesh = NULL;
	meshSelected = false;

	active = false;
	collide_system = NULL;

	reqAtSec = NULL;
	reqAtDoor = 0;
	strcpy(reqRoomUrl, "");

	strcpy(testRoom, "http://www.yahoo.com/ 10 5 20 7\nhttp://www.cnn.com/ cube Component Component 1\nhttp://www.altavista.com/ violin image image 0 2 0.0 13.0\n");
	strcat(testRoom, "http://www.google.com/ stool Connector Connector 1\n");
	//strcat(testRoom, "denis mdl1 User 192.168.1.100 1\n");
	strcat(testRoom, "http://www.cs.brandeis.edu/ stool Connector Connector 1\n");
	//strcat(testRoom, "suhit ninja User 192.168.1.102 1\n");
	strcat(testRoom, "http://www.navy.mil/ stool Connector Connector 1\n");
	strcat(testRoom, "http://www.philgross.com/ stool Connector Connector 1\n");
	strcat(testRoom, "http://www.suhit.com/ stool Connector Connector 1\n");
	//strcat(testRoom, "navdeep mdl1 User 192.168.1.103 1\n");
	strcpy(reqRoomUrl, "http://www.yahoo.com/");

}

//**********************************************************************
//*
//* Transport to an optimal location within a room
//*
//**********************************************************************
bool ChimeSystemDriver::TransportToRoom(char *name) {
	csVector3 *camLocation;
	ChimeSector *sec = FindSector(name);
	
	if(sec) {
		camLocation = sec->GetCamLocation();
		Transport(sec->GetRoom(0), *camLocation, csVector3(0,0, 1), csVector3(0,-1, 0));
		return true;
	}

	return false;
}

//**********************************************************************
//*
//* This function converts the Coordinates from Local To Global
//* We need this because the Chime System is responsible for everything
//*
//**********************************************************************
bool ChimeSystemDriver::ConvertCoordinates(csVector2 *screenCoord) {
	int x = (int) screenCoord->x;
	int y = (int) screenCoord->y;
	CoordinateConvertor->LocalToGlobal(x, y);
	screenCoord->x = (float) x;
	screenCoord->y = (float) y;
	return true;
}


//**********************************************************************
//*
//* This function sets the Coordinate Convertor which we need in a multi-Window
//* Environment
//*
//**********************************************************************
void ChimeSystemDriver::setCoordinateConvertor(csComponent *Parent) {
	CoordinateConvertor = Parent;
}


//**********************************************************************
//*
//* Destructor
//*
//**********************************************************************
ChimeSystemDriver::~ChimeSystemDriver()
{
	SCF_DEC_REF(vc);
	SCF_DEC_REF(plugin_mgr);
	SCF_DEC_REF(myG2D);
	SCF_DEC_REF(myG3D);
	SCF_DEC_REF(collide_system);
	SCF_DEC_REF(view);
}

//**********************************************************************
//*
//* This function is a callback function from the communicator class. 
//* Whenever a network event comes in that concerns the ChimeSystemDriver it gets
//* routed here
//*
//**********************************************************************
void ChimeSystemDriver::GetFunction(int method, char *received)
{
	//Communication thread waits here until main thread
	//releases the lock on hMutex
	WaitForSingleObject(hMutex,INFINITE);

	HandleNetworkEvent(method, received);

	ReleaseMutex(hMutex);

}

//**********************************************************************
//*
//* Find room corresponding to a given room url
//*
//**********************************************************************
iSector* ChimeSystemDriver::FindRoom(char *roomUrl)
{
	ChimeSector *sec = NULL;
	iSector	*room = NULL;

	for(int i = 0; i < NUM_SECT; i++)
	{
		if(sector[i] && !strcmp(roomUrl, sector[i]->GetUrl()) )
		{
			sec = sector[i];
			room = sec->GetRoom(0);
			return room;
		}
	}
	return room;
}

//**********************************************************************
//*
//* Find sector corresponding to a given room url
//*
//********************************************************************** 
ChimeSector* ChimeSystemDriver::FindSector(char *roomUrl)
{
	ChimeSector *sec = NULL;

	for(int i = 0; i < NUM_SECT; i++)
	{
		if(sector[i] && !strcmp(roomUrl, sector[i]->GetUrl()) )
		{
			sec = sector[i];
			return sec;
		}
	}
	return sec;
}

//**********************************************************************
//*
//* Find sector corresponding to a given room
//*
//********************************************************************** 
ChimeSector* ChimeSystemDriver::FindSector(iSector *room)
{
	ChimeSector *sec = NULL;

	for(int i = 0; i < NUM_SECT; i++)
	{
		if(sector[i] && sector[i]->IsRoomOf(room) )
		{
			sec = sector[i];
			return sec;
		}
	}
	return sec;
}

//**********************************************************************
//*
//* Find  an object in a given room
//*
//********************************************************************** 
iMeshWrapper* ChimeSystemDriver::FindObject(iSector *room, char *objectUrl)
{
	iMeshWrapper* obj = NULL;
	iMeshList *mesh_list = room->GetMeshes();

	for (int i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);

		if( !strcmp(objectUrl, mesh->QueryObject()->GetName()) )
		{
			obj = mesh;
			return obj;
		}
	}

	return obj;
}

//**********************************************************************
//*
//* User has moved send notification to all clients.
//*
//********************************************************************** 
void ChimeSystemDriver::UserMoved()
{
	csVector3 newPos, roomOrigin;
	ChimeSector  *sec, *prevSector;
	char *roomUrl;

	prevSector = currentSector;
	currentSector = sec = GetCurChimeSector();

	char username[50];
	info->GetUsername(username);

	if (strcmp(username, "") == 0) 
		return;

	char my_ip_address[50];
	info->GetMyIPAddress(my_ip_address);

	if (strcmp(my_ip_address, "") == 0)
		return;

	if(prevSector && currentSector != prevSector)
	{

		int roomType = currentSector->IsRoomOf(view->GetCamera()->GetSector());
		if( roomType == HALLWAY)
		{
			if( sector[nextSector] )
			{
				comm.UnsubscribeRoom(sector[nextSector]->GetUrl(), username);
				RemoveChimeSector(sector[nextSector]);
			}

			nextSector--;			//This sector can be removed if needed.
		}
		else
		{
			//RemoveChimeSector( prevSector );
			nextSector++;			//protect this room from caching out
		}
		
		comm.UserLeftRoom(username, my_ip_address, prevSector->GetUrl());

		/*
		newPos = view->GetCamera()->GetOrigin();
		roomOrigin = sec->GetOrigin();
		newPos -= roomOrigin;
		*/

		newPos = view->GetCamera()->GetTransform().GetOrigin();
		roomOrigin = csVector3(0, 0, 0);
		newPos -= roomOrigin;

		comm.UserEnteredRoom(username, my_ip_address, sec->GetUrl(), newPos.x, newPos.y, newPos.z);
		ResetLocalChatBuddies(sec);
	}
	else
	{
		roomUrl = sec->GetUrl();

		/*
		newPos = view->GetCamera()->GetOrigin();
		roomOrigin = sec->GetOrigin();
		newPos -= roomOrigin;
		*/

		newPos = view->GetCamera()->GetTransform().GetOrigin();
		roomOrigin = csVector3(0, 0, 0);
		newPos -= roomOrigin;

		comm.MoveUser(roomUrl, username, my_ip_address, newPos.x, 0, newPos.z, sec->GetUserList());
		//MoveUser(roomUrl, "1.1.1.1", newPos.x, 0, newPos.z+4);
	}

}

//**********************************************************************
//*
//* Get rid of everyone in the local chat window
//*
//********************************************************************** 
void ChimeSystemDriver::ResetLocalChatBuddies(ChimeSector *cur_sec) {
	if (app && app->chatWindow) {
		//app->chatWindow->DeleteAllLocalUsers();
		app->chatWindow->AddLocalUsers(cur_sec->GetUserList());
	}
}


//handy routing for printing out errors
void ChimeSystemDriver::Report (int severity, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);
  iReporter* rep = CS_QUERY_REGISTRY (object_reg, iReporter);
  if (rep)
  {
    rep->ReportV (severity, "crystalspace.system", msg, arg);
    rep->DecRef ();
  }
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
  }
  va_end (arg);
}


//**********************************************************************
//*
//* This function loads all the necessary plugins used by
//* Crystal-Space engine.
//*
//**********************************************************************
bool ChimeSystemDriver::Initialize(int argc, const char *const argv[], const char *iConfigName, InfoStorer* info, iObjectRegistry *object_reg)
{
	ChimeSystemDriver::info = info;
	ChimeSystemDriver::object_reg = object_reg;

	historyWindow = NULL;
	
	srand (time (NULL));

	if (!csInitializer::SetupConfigManager (object_reg, iConfigName))
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "Failed to init config!");
		return false;
	}

	if (!csInitializer::RequestPlugins (object_reg,
  	CS_REQUEST_REPORTER,
  	CS_REQUEST_REPORTERLISTENER,
  	CS_REQUEST_END
	))
	{
    Report (CS_REPORTER_SEVERITY_ERROR, "Failed to initialize!");
    return false;
	}
	

	//SetDefaults();

	if (!csInitializer::SetupEventHandler(object_reg, ChimeHandleEvent))
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "Failed to Initialize!");
		return false;
	}

	plugin_mgr = CS_QUERY_REGISTRY(object_reg, iPluginManager);
	
	if (!plugin_mgr)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "PlugIn Manager Failed!");
		return false;
	}


	vc = CS_QUERY_REGISTRY(object_reg, iVirtualClock);

	if (!vc)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "Virtual Clock Failed!");
		return false;
	}

	//get a pointer to G3D drawing object
	myG3D = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
	if (!myG3D) {
		Report (CS_REPORTER_SEVERITY_ERROR, "No iGraphics3D Plugin!");
		return false;
	}

	//get a pointer to G2D drawing object
	myG2D = CS_QUERY_REGISTRY (object_reg, iGraphics2D);
	if (!myG2D) {
		Report (CS_REPORTER_SEVERITY_ERROR, "No iGraphics2D Plugin!");
		return false;
	}

	myVFS = CS_QUERY_REGISTRY (object_reg, iVFS);
	if (!myVFS)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "No iVFS plugin!");
		return false;
	}

	kbd = CS_QUERY_REGISTRY( object_reg , iKeyboardDriver );
	if (!kbd) {
		Report (CS_REPORTER_SEVERITY_ERROR, "No iKeyboardDriver!");
		return false;
	}


	myMotionMan = CS_QUERY_REGISTRY (object_reg, iMotionManager);
	if (!myMotionMan) {
		Report (CS_REPORTER_SEVERITY_ERROR, "No Motion Manager!");
		return false;
	}

	//start the engine
	iNativeWindow *nw = myG2D->GetNativeWindow();
	if (nw) 
		nw->SetTitle("Chime");

	if (!csInitializer::OpenApplication(object_reg)) {
		Report (CS_REPORTER_SEVERITY_ERROR, "Error opening system!");
		return false;
	}

	FrameWidth= myG2D->GetWidth();
	FrameHeight = myG2D->GetHeight();

	//find the csengine object
	engine = CS_QUERY_REGISTRY( object_reg, iEngine);
	if (!engine) {
		Report (CS_REPORTER_SEVERITY_ERROR, "No iEngine plugin!");
	}


	// Find the level loader plugin
	LevelLoader = CS_QUERY_REGISTRY (object_reg, iLoader);
	if (!LevelLoader)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "No level loader plugin!");
		return false;
	}

	// Find the model converter plugin
	ModelConverter = CS_QUERY_REGISTRY (object_reg, iModelConverter);
	if (!ModelConverter)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "No model converter plugin!");
		return false;
	}

	// Find the model crossbuilder plugin
	CrossBuilder = CS_QUERY_REGISTRY (object_reg, iCrossBuilder);
	if (!CrossBuilder)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "No model crossbuilder plugin!");
		return false;
	}

	//just enable the performance stats for the hell of it
	perf_stats = CS_QUERY_REGISTRY( object_reg, iPerfStats );
	if (!perf_stats) {
		Report (CS_REPORTER_SEVERITY_WARNING, "No perfomance stats will be seen!");
	}


	//Turn of light caching
	engine->SetLightingCacheMode (false);


	//create a view and a camera
	view = new csView (engine, myG3D);

	//Load chime objects and textures
	if (!LoadChimeLib("chimelib.txt")) 
	{
		Report (CS_REPORTER_SEVERITY_WARNING, "Problem with Chime Lib!!");
		return false;
	}


	// Get the collide system plugin.
	iConfigManager* Config = CS_QUERY_REGISTRY (object_reg, iConfigManager);
	const char* p = Config->GetStr ("Walktest.Settings.CollDetPlugIn",
		"crystalspace.collisiondetection.rapid");
	
	collide_system = CS_LOAD_PLUGIN (plugin_mgr, p, iCollideSystem);
	if (!collide_system)
	{
		Report (CS_REPORTER_SEVERITY_ERROR, "No Collision Detection plugin found!\n");
		return false;
	}

	/** Set up communication class **/
	//Comunication thread is initially blocked, until client unblocks it in NextFrame()
	WaitForSingleObject(hMutex,INFINITE);
	
	SetInfoObject();

	main_txtmgr = myG3D->GetTextureManager ();
	main_txtmgr->ResetPalette ();
	main_txtmgr->SetVerbose (true);

	 // Initialize the texture manager
	int r,g,b;
	for (r = 0; r < 8; r++)
		for (g = 0; g < 8; g++)
			for (b = 0; b < 4; b++)
				main_txtmgr->ReserveColor (r * 32, g * 32, b * 64);
	
	main_txtmgr->SetPalette ();

	// Initialise the procedural textures.
	// we do it first because if a level is loaded it will erase everything 
	// previously loaded into the engine.
	//InitProcTextures ();

	curSector = 0;

	engine->Prepare ();

	//create the siena room
	char siena_location[50];
	char username[50];
	char password[50];

	info->GetSienaLocation(siena_location);
	info->GetUsername(username);
	info->GetPassword(password);

	comm_client = new ClientComm(info->GetChatPort(), siena_location, 
					info->GetSienaPort(), username, password, this);
	
	comm.SetChimeCom(comm_client, this);
	info->SetCommObject(comm_client);

	ReadRoom(testRoom);


	// Open the procedural textures after the main texture manager has been prepared
	//if (!OpenProcTextures ())
	//   return false;

	return true;
}


//************************************************************************************
//*
//* Set the contents of the info object for this session
//*
//************************************************************************************
void ChimeSystemDriver::SetInfoObject() {
	char password[50];
	char username[50];
	char siena_location[50];

	info->GetPassword(password);
	info->GetUsername(username);
	info->GetSienaLocation(siena_location);

	if (strcmp(password, "") == 0 || strcmp(username, "") == 0 || strcmp(siena_location, "") == 0) {
			info->SetUsername("denis");
			info->SetPassword("denis");
			info->SetSienaLocation("localhost");
	}

	info->SetSienaPort(1234);
	info->SetChatPort(9999);
}



//**********************************************************************
//*
//* Initialize a procedural texture
//*
//**********************************************************************
/*void ChimeSystemDriver::Init (ProcTexture *pt)
{
  if (!pt->Initialize ())
  {
    Report (CS_REPORTER_SEVERITY_ERROR, "Proc %s failed\n", pt->GetName()); 
    delete pt;
  }
  else
    TexVec.Push (pt);
}
*/
//**********************************************************************
//*
//* Initialize all procedural textures
//*
//**********************************************************************
/*void ChimeSystemDriver::InitProcTextures ()
{
  ProcTexture::System = this;
  ProcTexture::Engine = engine;
  ProcTexture::MainTxtMgr = main_txtmgr;

  ProcTexture *pt = (ProcTexture*) new EngineProcTexture ();
  Init (pt);
}
*/
//**********************************************************************
//*
//* prepare all of our texture animations
//*
//**********************************************************************
/*bool ChimeSystemDriver::OpenProcTextures ()
{
  int i;
  for (i = 0; i < TexVec.Length (); i++)
  {
    ProcTexture *pt = (ProcTexture*)TexVec.Get (i);
    if (!pt->PrepareAnim (GetCurChimeSector()->GetRoom(0)))
      return false;
  }
  return true;
}
*/
//**********************************************************************
//*
//* Animate all of our procedural textures
//*
//**********************************************************************
/*void ChimeSystemDriver::AnimateProcTextures (csTicks current_time, csTicks elapsed_time)
{
  int i;
  for (i = 0; i < TexVec.Length (); i++)
  {
    ProcTexture *pt = (ProcTexture*)TexVec.Get (i);
    pt->Animate (current_time, elapsed_time);
  }
}
*/

//**********************************************************************
//*
//* Refresh 3D Display
//*
//**********************************************************************
void ChimeSystemDriver::Refresh3D ()
{
//	SysSystemDriver::NextFrame ();
	if (!myG3D->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
		return;

	view->Draw ();

	// Start drawing 2D graphics.
	if (!myG3D->BeginDraw (CSDRAW_2DGRAPHICS))
	{
		return;
	}

	
	// Drawing code ends here.
	myG3D->FinishDraw ();
	// Print the final output.
	myG3D->Print (NULL);

}

//**********************************************************************
//*
//* Stop updating 3D view
//*
//**********************************************************************
void ChimeSystemDriver::Stop3D ()
{
	WaitForSingleObject(hMutex,INFINITE);
	active = false;
	ReleaseMutex(hMutex);
}

//**********************************************************************
//*
//* Start updating 3D view
//*
//**********************************************************************
void ChimeSystemDriver::Start3D ()
{
	WaitForSingleObject(hMutex,INFINITE);
	active = true;
	ReleaseMutex(hMutex);
}

//**********************************************************************
//*
//* Function responsible for preparing the next frame.
//* to be displayed.
//*
//**********************************************************************
void ChimeSystemDriver::SetupFrame()
{

  WaitForSingleObject(hMutex,INFINITE);
  //Block communication thread after letting it process atleast one event
  
  csTicks elapsed_time, current_time;
  elapsed_time = vc->GetElapsedTicks();
  current_time = vc->GetCurrentTicks();

  if (perf_stats)
	  timeFPS = perf_stats->GetFPS();
  else
	  timeFPS = 0;

  
  //if the menu shouldn't be drawn then get rid of it
  DeletePopupMenu();

  static csTicks inactive_time = 0;

  // OK do the labeling thing
  //AnimateProcTextures (current_time, elapsed_time);

 // Update the Motion Manager
  if (myMotionMan)
    myMotionMan->UpdateAll ();


  if( active )
  {
	  // Now rotate the camera according to keyboard state
	  float speed = (elapsed_time / 1000.) * (0.03 * 20);

	  if (kbd->GetKeyState (CSKEY_RIGHT))
		  view->GetCamera ()->GetTransform().RotateThis (VEC_ROT_RIGHT, 2.0f*speed);
	  if (kbd->GetKeyState (CSKEY_LEFT))
		  view->GetCamera ()->GetTransform().RotateThis (VEC_ROT_LEFT, 2.0f*speed);
	  if (kbd->GetKeyState (CSKEY_PGUP))
		  view->GetCamera ()->GetTransform().RotateThis (VEC_TILT_UP, 2.0f*speed);
	  if (kbd->GetKeyState (CSKEY_PGDN))
		  view->GetCamera ()->GetTransform().RotateThis (VEC_TILT_DOWN, 2.0f*speed);
	  if (kbd->GetKeyState (CSKEY_UP))
	  {
		  view->GetCamera ()->Move (VEC_FORWARD * 4.0f * speed * SPEED);
		  UserMoved();
	  }
	  if (kbd->GetKeyState (CSKEY_DOWN))
	  {
		  view->GetCamera ()->Move (VEC_BACKWARD * 4.0f * speed * SPEED);
		  UserMoved();
	  }

	  //engine->SetContext (myG3D);

	  // Tell 3D driver we're going to display 3D things.
	  if (!myG3D->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_2DGRAPHICS|CSDRAW_3DGRAPHICS))
		  return;

	  view->Draw ();
	  inactive_time = 0;

  }
  

	//writeMessage();

	// Start drawing 2D graphics.
	if (!myG3D->BeginDraw (CSDRAW_2DGRAPHICS))
	{
		return;
	}

	myG2D->BeginDraw();
}



//Finish drawing
void ChimeSystemDriver::FinishFrame() {
	
	 // Drawing code ends here.
	 myG3D->FinishDraw ();
  
	// Print the final output.
    myG3D->Print (NULL);

	myG2D->FinishDraw ();
	myG2D->Print (NULL);

  //Give communication thread a chance to update any new events
  ReleaseMutex(hMutex);

  //NOTE:
  //Get Function is possibly called by ClientComm during this period.

}

//**********************************************************************
//*
//* Write a message at the bottom of the screen
//*
//**********************************************************************
void ChimeSystemDriver::writeMessage() 
{
	char username[50];
	info->GetUsername(username);

	if (strcmp(username, "") == 0)
		return;

	iTextureManager *tm = myG3D->GetTextureManager ();
	int write_colour = tm->FindRGB (255, 255, 255);
	iFont *courierFont = NULL;
	iFontServer *fs = myG2D->GetFontServer ();
	courierFont = fs->LoadFont (CSFONT_COURIER);
	ChimeSector *curSect = GetCurChimeSector();
	if (curSect != NULL) {
		char displayString[1000];
		if (selectedMesh) {
			sprintf(displayString, "Selected Mesh: %s", selectedMesh->QueryObject()->GetName());
			myG2D->Write(courierFont, 2, myG2D->GetHeight() - 40, write_colour, -1, displayString);
		}
		sprintf(displayString, "Username: %s", username);
		myG2D->Write(courierFont, 2, myG2D->GetHeight() - 30, write_colour, -1, displayString);
		sprintf(displayString, "Room Location: %s", curSect->GetUrl());
		myG2D->Write(courierFont, 2, myG2D->GetHeight() - 20, write_colour, -1, displayString);
		sprintf(displayString, "Number of Users other than You: %d", curSect->GetUserList()->Length());
		myG2D->Write(courierFont, 2, myG2D->GetHeight() - 10, write_colour, -1, displayString);
	}
}

//**********************************************************************
//*
//* Transport user from one sector to a another sector.
//*
//**********************************************************************
bool ChimeSystemDriver::Transport(iSector *room, csVector3 pos, csVector3 lookPos, csVector3 lookUp)
{
	if(!room)
		return false;


	
	view->GetCamera ()->SetSector (room);

	long camera_number = view->GetCamera()->GetCameraNumber();
	iCameraPosition *cam_pos = engine->GetCameraPositions()->Get(camera_number);
	
	if (cam_pos) {
		cam_pos -> Set(room->QueryObject()->GetName(), pos, lookPos, lookUp);
		return true;
	}


	//next line should be commented WARNINGI!!!!!
	 //view->GetCamera ()->SetPerspectiveCenter (FrameWidth / 2, FrameHeight / 2);
	
	 //view->SetRectangle (0, 0, FrameWidth/2, FrameHeight/2);

	return false;
}

//**********************************************************************
//*
//* Returns the current chime sector of a user/camera
//*
//**********************************************************************
ChimeSector * ChimeSystemDriver::GetCurChimeSector()
{
	ChimeSector *chimeSec = NULL;
	iSector *room;
	room = view->GetCamera ()->GetSector();

	int type;
	for(int i = 0; i < NUM_SECT; i++)
	{
		if( sector[i] && (type = sector[i]->IsRoomOf(room)) )
		{
			chimeSec = sector[i];
			break;
		}
	}
	return chimeSec;
}

//**********************************************************************
//*
//* Build and link sec2 to a given hallway-door of sec1
//*
//**********************************************************************
bool ChimeSystemDriver::LinkChimeSectors(ChimeSector *sec1, int doorNum, ChimeSector *&sec2, char *link)
{
	if(!sec1) return false;

	//Don't do anything if sect2 already exists.
	if(sec2) return false;


	csVector3 doorPos;
	iPolygon3D *doorPoly;
	iSector* hallway;

	sec1->GetDoorInfo(doorNum, doorPos, doorPoly, hallway);
	sec2 = new ChimeSector(Sys, engine);
	sec2->BuildDynamicRoom2(link, doorPos, collide_system);
	sec2->Connect(doorPoly, hallway);

    return true;
}

/***************************************************************************
/*
/* Get the graphical engine ready for a menu
/*
/***************************************************************************/
bool ChimeSystemDriver::SetupMenu() {

  //just in case there was a popup menu
  //mark it for deletion
  MarkPopupMenuForDeletion();

  //delete it since this will not be called from HandleEvent of the menu class
  DeletePopupMenu();

  //disable all movement
  Stop3D();

  return true;
}


/****************************************************************************
/*
/* Bring up the menu for the door
/*
/****************************************************************************/
bool ChimeSystemDriver::BringUpDoorMenu(int doorNum, csVector2 screenPoint) {

  csVector3 doorPos;
  iPolygon3D *doorPoly;
  iSector* hallway;
  currentSector->GetDoorInfo(doorNum, doorPos, doorPoly, hallway);
  
  char name[500];

  //do all the preliminary steps needed for menu creation
  SetupMenu(); 

  char *doorUrl;
  reqAtSec = currentSector;	
  reqAtDoor  = doorNum;
  doorUrl = reqAtSec->GetDoorUrl(doorNum);

  if (!doorUrl)
	  strcpy(reqRoomUrl, "<NOWHERE>");

  else
	  strcpy(reqRoomUrl, doorUrl);

  // Create a menu for all test dialogs we implement
  menu = new csMenu (app, csmfs3D, 0);
  csMenu *submenu = new csMenu (NULL);

  if (reqRoomUrl != NULL) {	 
	  strcpy(name, "Link to: ");
	  strcat(name, reqRoomUrl);
	  (void)new csMenuItem (menu, name, -1);
	  submenu = new csMenu (NULL);

  }

  (void)new csMenuItem (menu, "", -1);

  if (doorPoly -> GetAlpha() == 25)
     (void)new csMenuItem (menu, "~Open this link", DOOR_OPEN_LINK);

  (void)new csMenuItem (menu, "~Link this somewhere else", DOOR_LINK_SOMEWHERE_ELSE);

  menu->SetPos (screenPoint.x - 3, FrameHeight - (screenPoint.y - 3));
  menu_drawn = true;
  
  return true;
}


/**************************************************************************
/* 
/*        Update the link to which this door leads
/*	Also uypdates the physical mesh object which represents the door
/*			in the room.
/*
/**************************************************************************/
bool ChimeSystemDriver::UpdateDoorLink(ChimeSector *sec, int doorNum, char *new_door_url) {
	iSector *room;

	csVector3 doorPos;
	iPolygon3D *doorPoly;
	iSector* hallway;
	sec->GetDoorInfo(doorNum, doorPos, doorPoly, hallway);

	char orig_door_url[100];
	
	if (!sec->GetDoorUrl(doorNum))
		strcpy(orig_door_url, "<NOWHERE>");

	else
		strcpy(orig_door_url, sec->GetDoorUrl(doorNum));

	if (!new_door_url)
		return false;

	if (strcmp(new_door_url, orig_door_url) == 0)
		return false;	
		

	iMeshWrapper *object = sec -> FindObject(orig_door_url, room);

	if (!object) {
		csVector3 position(1,0, 4);
		sec->AddMeshObj ("stool", new_door_url, sec->GetRoom(0) , position, 1);
	}

	else 
		object->QueryObject()->SetName(new_door_url);

	sec ->ReplaceDoorUrl(doorNum, new_door_url);
	doorPoly->SetAlpha(25);

	return true;
}


/**************************************************************************
/* 
/*        Open the indicated door.
/*  Note: Will work if reqAtDoor and reqAtSec are set. Otherwise
/*        nothing will happen
/*
/**************************************************************************/
bool ChimeSystemDriver::OpenDoor(char *doorUrl) {

	char username[50];
	info->GetUsername(username);

	UpdateDoorLink(reqAtSec, reqAtDoor, doorUrl);

	if (strcmp(username, "") == 0)
		return false;

	if (reqAtDoor != 0 && reqAtSec != NULL) {
		if (!doorUrl)
			doorUrl = reqAtSec->GetDoorUrl(reqAtDoor);

		strcpy(reqRoomUrl, doorUrl);
		comm.SubscribeRoom(doorUrl, username);
		comm.GetRoom(doorUrl);
		return true;
	}

	else
		return false;
}




//*************************************************************************
//*
//* Function responsible for handling Right mouse button click
//* If User clicks on the door, then user is transported to the new room
//* else User is presented with pull down menu for list availaible links.
//*
//************************************************************************
bool ChimeSystemDriver::HandleRightMouseClick(iEvent &Event)
{

  //Check if user clicked on one of the rooms
  csVector3 v;
  csVector2 p (Event.Mouse.x, FrameHeight-Event.Mouse.y);
  view->GetCamera ()->InvPerspective (p, 1, v);

  //csVector3 vw = view->GetCamera ()->Camera2World (v);
  //csVector3 origin = view->GetCamera ()->GetW2CTranslation ();

  csVector3 vw = view->GetCamera()->GetTransform().This2Other(v);
  csVector3 origin = view->GetCamera()->GetTransform().GetO2TTranslation();

  csVector3 isect;

  ChimeSector *currentSector = GetCurChimeSector();
  int doorNum;
  int result;

  if ((result = currentSector->HallwayHitBeam(origin, origin + (vw-origin) * 20, isect, doorNum)) != DOOR_NOT_FOUND)
  {
	  csVector2   screenPoint;
	  screenPoint.x = Event.Mouse.x;
	  screenPoint.y = FrameHeight - Event.Mouse.y - 1;
	  BringUpDoorMenu(doorNum, screenPoint);
	  //FIXIT Remove this debug code
	 /* if(!strcmp(doorUrl, "www.google.com"))
			ReadRoom(google);
	  else
		  ReadRoom(testRoom);
		  */

  } 
  
  else 
  {
	csVector2   screenPoint;
	screenPoint.x = Event.Mouse.x;
	screenPoint.y = FrameHeight - Event.Mouse.y - 1;
	selectedMesh = SelectMesh(view->GetCamera(), &screenPoint, selectedMeshDist);

	if (selectedMesh) {
		DrawMenu(screenPoint);
		return true;
	}
  }
	
	return false;
}

//*************************************************************************
//*
//* Tell ChimeSystemDriver System where to find the App
//*
//************************************************************************
void ChimeSystemDriver::setCSApp(ChimeApp *app) {
	ChimeSystemDriver::app = app;
}

//*************************************************************************
//*
//* Tell ChimeSystemDriver System where to find the History Window
//*
//************************************************************************
void ChimeSystemDriver::SetHistoryWindow(HistoryWindow *historyWindow) {
	ChimeSystemDriver::historyWindow = historyWindow;
}


//*************************************************************************
//*
//* Draws a menu
//*
//************************************************************************
bool ChimeSystemDriver::DrawMenu(csVector2 screenPoint) {

  char name[50];

  //setup everything to create a new popup menu
  SetupMenu();

  // Create a menu for all test dialogs we implement
  menu = new csMenu (app, csmfs3D, 0);
  csMenu *submenu = new csMenu (NULL);

  if (selectedMesh) {	 
	  strcpy(name, "Name: ");
	  strcat(name, selectedMesh->QueryObject()->GetName());
	  (void)new csMenuItem (menu, name, -1);
	  submenu = new csMenu (NULL);

  }

    (void)new csMenuItem (menu, "~Editing Options", submenu);
    (void)new csMenuItem (submenu, "~Edit with Default App", OBJECT_EDIT_WITH_DEFAULT_APP);
    (void)new csMenuItem (submenu, "~Select App", OBJECT_SELECT_APP);

  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Moving Options", submenu);
    (void)new csMenuItem (submenu, "Carry", OBJECT_CARRY);
    (void)new csMenuItem (submenu, "Drop", OBJECT_DROP);
	 (void)new csMenuItem (submenu, "Delete", OBJECT_DELETE);
    (void)new csMenuItem (submenu, "Undelete", OBJECT_UNDELETE);

  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Security", submenu);
    (void)new csMenuItem (submenu, "Increase", OBJECT_INCREASE_SECURITY);
    (void)new csMenuItem (submenu, "Decrease", OBJECT_DECREASE_SECURITY);
  
  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Properties", OBJECT_PROPERTIES);
   
  csMenuItem *mi = new csMenuItem (menu, "~Quit\tQ", cscmdQuit);

  // Show that a hint can be added to absolutely any component
  app->HintAdd ("Choose this menu item to quit the program", mi);

  menu->SetPos (screenPoint.x - 3, FrameHeight - (screenPoint.y - 3));

  menu_drawn = true;
  return true;
}

//*************************************************************************
//*
//* Because of various stack returns we need to first mark the Popup menu
//* for deletion and it will be deleted either when the user tries to open
//* a new popup menu or when next frame is called and the menu is marked
//* for deletion
//*
//************************************************************************
bool ChimeSystemDriver::MarkPopupMenuForDeletion()
{
	if (menu_drawn) {
		menu_drawn = false;
		Start3D();
		return true;

	} else 
		return false;
	
}


//****************************************************************************
//*
//* Delete the popup menu and free all the memory that was occupied by
//* whichever popup menu was on the screen last
//*
//*****************************************************************************
bool ChimeSystemDriver::DeletePopupMenu() 
{
	if (!menu_drawn && menu != NULL) {
		menu -> Close();
		menu = NULL;
		return true;
	} else
		return false;
}

//*************************************************************************
//*
//* Handle All Events that emerge from the popup menu
//*
//************************************************************************
bool ChimeSystemDriver::HandleMenuEvent(iEvent &Event) 
{
  //if (superclass::HandleEvent (Event))
  // return true;


  switch (Event.Type)
  {
    case csevCommand:
      switch (Event.Command.Code)
      {
	    //Edit with Default App Selected
        case OBJECT_EDIT_WITH_DEFAULT_APP:
		case OBJECT_SELECT_APP:
		case OBJECT_CARRY:
		case OBJECT_DROP:
		case OBJECT_DELETE:
		case OBJECT_UNDELETE:
		case OBJECT_INCREASE_SECURITY:
		case OBJECT_DECREASE_SECURITY:
		case OBJECT_PROPERTIES:
		case DOOR_OPEN_LINK:
			OpenDoor();
			MarkPopupMenuForDeletion();
			return true;

		case DOOR_LINK_SOMEWHERE_ELSE:
			SetupMenu();
			(void)new GetObjectWindow(app);
			return true;
	  }
  }

  return false;
}
//*************************************************************************
//*
//* Function responsible for handling left mouse button click
//* If User clicks on the door, then user is transported to the new room
//* else User is presented with pull down menu for list availaible links.
//*
//************************************************************************
bool ChimeSystemDriver::HandleLeftMouseClick(iEvent &Event)
{

	csVector2   screenPoint;

//	_spawnl(_P_NOWAIT, "c:\\args.txt", NULL);

	screenPoint.x = Event.Mouse.x;
	screenPoint.y = FrameHeight - Event.Mouse.y - 1;
	selectedMesh = SelectMesh(view->GetCamera(), &screenPoint, selectedMeshDist);

	if (selectedMesh)
	{
		meshSelected = true;

		csVector3 v;
		selectedMeshPos = selectedMesh->GetMovable()->GetPosition();
		csVector2 p (Event.Mouse.x, FrameHeight-Event.Mouse.y);
		view->GetCamera ()->InvPerspective (p, 1, v);
		//selectedMeshPos = view->GetCamera ()->Camera2World (v);
		//csVector3 origin = view->GetCamera ()->GetW2CTranslation ();
		selectedMeshPos = view->GetCamera()->GetTransform().This2Other(v);
		csVector3 origin = view->GetCamera()->GetTransform().GetO2TTranslation();


		selectedMeshPos = selectedMeshPos - origin;
		selectedMeshPos.Normalize();

		//FIXIT test
		csBox3 box;
		csReversibleTransform t;
		selectedMesh->GetTransformedBoundingBox(selectedMesh->GetMovable()->GetFullTransform().GetInverse() , box);

	}
	else
	{
		meshSelected = false;
	}

	return true;
}

//*************************************************************************
//*
//* Handle double click on the left mouse button
//*
//*************************************************************************
bool ChimeSystemDriver::HandleLeftMouseDoubleClick(iEvent &Event)
{
	iMeshWrapper *m;
	csVector2   screenPoint;

	screenPoint.x = Event.Mouse.x;
	screenPoint.y = FrameHeight - Event.Mouse.y - 1;
	m = SelectMesh(view->GetCamera(), &screenPoint, selectedMeshDist);

	ChimeSector *curSect = GetCurChimeSector();
	
	if (curSect == NULL) {
		return false; //this should never be the case because we are always somewhere
	}


	//is this a container?
	if (m)  {
		char username[50];
		info->GetUsername(username);

		if (strcmp(username, "") == 0)
			return false;

		if (curSect->findType(selectedMesh->QueryObject()->GetName()) == CONTAINER) {
			comm.SubscribeRoom((char*) selectedMesh->QueryObject()->GetName(), username);
			comm.GetRoom((char*) selectedMesh->QueryObject()->GetName());
		}
	
	//if it isn't then just launch the browser
	else 	
		{
			_spawnl(_P_NOWAIT, browserPath, "browser", selectedMesh->QueryObject()->GetName(), NULL);
		}

	}

	return true;
}

//*************************************************************************
//*
//* Move selected mesh as mouse moves.
//*
//************************************************************************
bool ChimeSystemDriver::MoveSelectedMesh(iEvent &Event)
{
	csVector3 trans;
	csVector3 newMeshPos;
	csVector3 oldMeshPos;
	csVector3 v, offset;
	csVector3 start, end, tol, isect;

	ChimeSector *curSect = GetCurChimeSector();

	if (curSect == NULL) {
		//this should never be the case but what the heck
		return false;
	}

	if (curSect->findType(selectedMesh->QueryObject()->GetName()) == USER) {
		//can't move another User
		return false;
	}


	csVector2 p (Event.Mouse.x, FrameHeight-Event.Mouse.y);
	view->GetCamera ()->InvPerspective (p, 1, v);
	//newMeshPos = view->GetCamera ()->Camera2World (v);
	//csVector3 origin = view->GetCamera ()->GetW2CTranslation ();

	newMeshPos = view->GetCamera()->GetTransform().This2Other(v);
	csVector3 origin = view->GetCamera()->GetTransform().GetO2TTranslation();

	newMeshPos = newMeshPos - origin;
	newMeshPos.Normalize();
	float fact = 1+newMeshPos.y-selectedMeshPos.y;
	newMeshPos.y = 0;
	newMeshPos = origin+selectedMeshDist * fact * 20 * newMeshPos;
	newMeshPos.y = 0;

	oldMeshPos = selectedMesh->GetMovable()->GetPosition();
	offset = newMeshPos - oldMeshPos;

	start = oldMeshPos;
	end = newMeshPos;
	tol = offset;
	tol.Normalize();
	tol *= 0.5;
	end += tol;
	start.y = end.y = 0.1;

	//Check intersection of the move path with walls
	if(!curSect->HitBeam(start, end, isect) )
	{
		//check collision with objects of the room.
		if(!CollisionDetect(selectedMesh, newMeshPos, view->GetCamera()->GetSector()) )
		{
			//csVector3 myPos = view->GetCamera()->GetOrigin();
			csVector3 myPos = selectedMesh->GetMovable()->GetPosition();
			iSector *room = FindSectContainingPoint(myPos, selectedMeshNewSect);

			//selectedMesh->GetMovable().MovePosition(offset);
			selectedMesh->GetMovable()->SetPosition(room, newMeshPos);
			selectedMesh->GetMovable()->UpdateMove();
		}
	}


	return true;
}

//*************************************************************************
//*
//* Find a sector containing a particular point
//*
//************************************************************************
iSector* ChimeSystemDriver::FindSectContainingPoint(csVector3 &pos, ChimeSector *&newSect)
{
	iSector *room = NULL;
	newSect = NULL;

	for(int i = 0; i < NUM_SECT; i++)
	{
		if(sector[i])
		{
			 room = sector[i]->FindRoomContainingPoint(pos);
			 if( room )
			 {
				 newSect = sector[i];
				 return room;
			 }
		}
	}

	return NULL;
}

//**********************************************************************
//*
//* Function responsible for handling keyboard events only
//*
//**********************************************************************
bool ChimeSystemDriver::HandleKeyEvent (iEvent &Event)
{
	//Camera speed

    csTicks elapsed_time;
	elapsed_time = vc->GetElapsedTicks();
   	
	float moveSpeed = 0.1;


	// Now rotate the camera according to keyboard state
	float speed = (elapsed_time / 1000.) * (0.03 * 20);

	switch (Event.Type)
	{
	case csevKeyUp:
		break;
	case csevKeyDown:

		if(Event.Key.Code == 'a')
		{
			view->GetCamera ()->Move (VEC_LEFT *  2.0f * moveSpeed);
			UserMoved();
			return true;
		}
		if(Event.Key.Code == 'd')
		{
			view->GetCamera ()->Move (VEC_RIGHT *  2.0f * moveSpeed);
			UserMoved();
			return true;
		}
		if(Event.Key.Code == 'w')
		{
			view->GetCamera ()->Move (VEC_FORWARD * 2.0f * moveSpeed);
			UserMoved();
			return true;
		}
		if(Event.Key.Code == 's')
		{
			view->GetCamera ()->Move (VEC_BACKWARD * 2.0f * moveSpeed);
			UserMoved();
			return true;
		}

		if(Event.Key.Code == 'r')
		{
			if( meshSelected )
			{
				comm.DeleteObject(selectedMeshSect->GetUrl(), (char*)selectedMesh->QueryObject()->GetName());
				DeleteMeshObj(selectedMesh);
				meshSelected = false;
			}
			return true;
		}
		if(Event.Key.Code == 'f')
		{
			if( freeLook ) freeLook = false;
			else freeLook = true;
			return true;
		}
		if(Event.Key.Code == '=')
		{
			if( SPEED < 40 ) SPEED += 1;
			return true;
		}
		if(Event.Key.Code == '-')
		{
			if( SPEED > 0 ) SPEED -= 1;
			return true;
		}

		if(Event.Key.Code == 'c') {
			ChangeMouseCursor();
		}


//    view->GetCamera ()->Rotate (VEC_ROT_RIGHT, speed);

		break;
	}

	return false;
}


//**********************************************************************
//*
//* Handle all events that are sent to us from external windows
//*
//**********************************************************************
bool ChimeSystemDriver::HandleEventFromOtherWindow(iEvent &Event) {
	
	if (Event.Type == csevCommand && Event.Command.Code == CHIME_EVENT) {
	
		ChimeInfoEvent *info_event = (ChimeInfoEvent*)Event.Command.Info;
		
		switch (info_event->getIdentifier()) {
			case CONNECT_IDENTIFIER:
			
			
			case GET_OBJECT_IDENTIFIER:
					char url[1024];
					if (info_event->getFirstToken(url))
						OpenDoor(url);
					return true;

			case VEM_IDENTIFIER:
			
			case SIENA_WINDOW_IDENTIFIER:
	
			case CHAT_IDENTIFIER:
			
			case HISTORY_IDENTIFIER:
			
			return true;
		}
	}
	
	//just say we can't handle it
	return false;
}

//**********************************************************************
//*
//* Function responsible for handling all the mouse,
//* keyboard and broadcast events.
//*
//**********************************************************************
bool ChimeSystemDriver::HandleEvent (iEvent &Event)
{

	//see if this is an event sent from some other window
	if (HandleEventFromOtherWindow(Event)) 
		goto handled;

	//see if this is a popup menu command and handle it if it is
	if (HandleMenuEvent(Event))
		goto handled;

	switch (Event.Type)
	{
	case csevBroadcast:
		break;
	case csevMouseDoubleClick:
		HandleLeftMouseDoubleClick(Event);
		break;
	case csevMouseDown:
		if(Event.Mouse.Button == 2)
		{
			HandleRightMouseClick(Event);
		}
		else if(Event.Mouse.Button == 1)
		{
			HandleLeftMouseClick(Event);
		}
		break;
	case csevMouseUp:
		if(Event.Mouse.Button == 1)
		{
			UpdateObjPos();
			meshSelected = false;
		} 
		else if (Event.Mouse.Button == 2)
		{
			//mark the popup menu for deletion
			MarkPopupMenuForDeletion();

			//deletes the popup menu if it exists
			DeletePopupMenu();
		}

		break;
	case csevMouseMove:
		{
			if(meshSelected)
			{
				MoveSelectedMesh(Event);
			}

			static bool first_time = true;
			if (freeLook)
			{
				int last_x, last_y;
				last_x = Event.Mouse.x;
				last_y = Event.Mouse.y;

				myG2D->SetMousePosition (FrameWidth / 2, FrameHeight / 2);
				if (!first_time)
				{
					camAngle.x =((float)(last_x - (FrameWidth / 2) )) / (FrameWidth*2);
					camAngle.y =(((float)(last_y - (FrameHeight / 2) )) / (FrameHeight*2)*(1-2*(int)1));

					view->GetCamera ()->GetTransform().RotateThis (VEC_ROT_RIGHT, 3*camAngle.x );
					view->GetCamera ()->GetTransform().RotateThis (VEC_TILT_UP, 3*camAngle.y );

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

		if(Event.Key.Code == 116)
		{
			csVector3 *camLocation;

			curSector = (curSector + 1) % NUM_SECT;
			iSector *room = sector[curSector]->GetRoom(0);

			if( room )
			{
				camLocation = sector[curSector]->GetCamLocation();
				Transport(room, *camLocation, csVector3(0,0, 1), csVector3(0,-1, 0));
			}

			goto handled;
		}
		if(Event.Key.Code == 117)
		{
			iSector *room = sector[curSector]->GetRoom(0);

			Transport(room, csVector3(0,2.5, -3), csVector3(0,0, 1), csVector3(0,-1, 0));

			goto handled;
		}

		if(Event.Key.Code == 118)
		{
			iSector *room = sector[curSector]->GetRoom(0);

			//csPolygon3D *p = room->GetPolygon3D(0);
			//csMaterialWrapper* mat = engine->GetMaterials ()->FindByName ("marble");
			//p->SetMaterial(mat);
			//engine->Prepare();
		}


	}//Switch

	return false;

handled:
	return true;
	
}


//**********************************************************************
//*
//* Main function
//* This is where Chime client starts its execution.
//*
//**********************************************************************
/*
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
*/
//**********************************************************************
//*
//* Load chime objects and textures
//*
//**********************************************************************
bool ChimeSystemDriver::LoadChimeLib(char *fileName)
{
	//Initialize Texture manager
	iTextureManager* txtmgr = myG3D->GetTextureManager ();
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
			if(!LevelLoader->LoadTexture (name, file))
			{
				strcpy(err,"Error Loading Texture: ");
				strcat(err, name);
				Report (CS_REPORTER_SEVERITY_ERROR, err);
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
				Report (CS_REPORTER_SEVERITY_ERROR, err);
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
				Report (CS_REPORTER_SEVERITY_ERROR, err);
				return false;
			}
		}
		else if(!strcmp(command, "BROWSER"))
		{//Read 3D model
			sscanf(buf, "%s %s", command, browserPath);
		}

		fgets(buf, bufSize, fp);
		sscanf(buf, "%s", command);
	}

	txtmgr->PrepareMaterials();
	txtmgr->SetPalette ();
	//engine->PrepareTextures();
	//engine->PrepareMeshes();

	fclose(fp);

	return true;
}



//*********************************************************************************
//*
//* Load a mesh object factory from a general format (3DS, MD2, ...)
//* This creates a mesh object factory which you can then add using add_meshobj ().
//*
//*********************************************************************************

bool ChimeSystemDriver::LoadMeshObj (char *filename, char *templatename, char* txtname)
{

   // First check if the texture exists.
  if (!Sys->view->GetEngine ()->GetMaterialList ()->FindByName (txtname))
  {
    Sys->Report (CS_REPORTER_SEVERITY_NOTIFY,
    	"Can't find material '%s' in memory!", txtname);
    return false;
  }

  // read in the model file
  iDataBuffer *buf = Sys->myVFS->ReadFile (filename);
  if (!buf)
  {
    Sys->Report (CS_REPORTER_SEVERITY_NOTIFY,
  	"There was an error reading the data!");
    return false;
  }

  iModelData *Model = Sys->ModelConverter->Load (buf->GetUint8 (), buf->GetSize ());
  buf->DecRef ();
  if (!Model)
  {
    Sys->Report (CS_REPORTER_SEVERITY_NOTIFY,
  	"There was an error reading the data!");
    return false;
  }


  csModelDataTools::SplitObjectsByMaterial (Model);
  csModelDataTools::MergeObjects (Model, false);
  iMeshFactoryWrapper *wrap =
  CrossBuilder->BuildSpriteFactoryHierarchy (Model, Sys->engine, engine->GetMaterialList ()->FindByName (txtname));
  Model->DecRef ();
  wrap->QueryObject ()->SetName (templatename);

  return true;
}


//*********************************************************************************
//*
//* Add mesh object in a given room at a given location.
//*
//*********************************************************************************
iMeshWrapper* ChimeSystemDriver::AddMeshObj (char* tname, char* sname, iSector* where,
	csVector3 const& pos, float size)
{
 iMeshFactoryWrapper* tmpl = Sys->engine->GetMeshFactories ()->FindByName (tname);
  if (!tmpl)
  {
    Sys->Report (CS_REPORTER_SEVERITY_NOTIFY,
    	"Unknown mesh factory '%s'!", tname);
    return NULL;
  }
  iMeshWrapper* spr = Sys->engine->CreateMeshWrapper (tmpl, sname,
						      where, pos);
  csMatrix3 m; m.Identity (); m = m * (size/10);
  spr->GetMovable ()->SetTransform (m);
  spr->GetMovable ()->UpdateMove ();

  spr->DeferUpdateLighting (CS_NLIGHT_STATIC|CS_NLIGHT_DYNAMIC, 10);
  spr->DecRef (); // its now held by a ref inside the engine
  return spr;
}


//*********************************************************************************
//*
//* Delete mesh object in a given room at a given location.
//*
//*********************************************************************************
bool ChimeSystemDriver::DeleteMeshObj(iMeshWrapper *mesh)
{
    if (mesh)
	{
        //engine->RemoveMesh (mesh);
		return true;
	}
    else
	{
		return false;
    }
}

//*********************************************************************************
//*
//* Remove selected chime sector.
//*
//*********************************************************************************
bool ChimeSystemDriver::RemoveChimeSector(ChimeSector* &sec)
{
	//sec->UnlinkHallwayDoors();
	sec->DisconnectSector();
	delete sec;
	sec = NULL;

	return true;
}

//*********************************************************************************
//*
//* Remove the first sector of the chimeworld
//* and push remaining sectors up.
//*
//*********************************************************************************

bool ChimeSystemDriver::ReshuffleSectors()
{
	sector[1]->DisconnectSector();
	delete sector[0];
	for(int i = 1; i < NUM_SECT; i++)
		sector[i-1] = sector[i];
	sector[NUM_SECT-1] = NULL;
	nextSector = NUM_SECT-1;

	return true;

}

//*********************************************************************************
//*
//* Find closest mesh to the clicked screen coordinate
//*
//*********************************************************************************
iMeshWrapper* ChimeSystemDriver::FindNextClosestMesh (iMeshWrapper *baseMesh,
	iCamera *camera, csVector2 *screenCoord)
{
  int meshIndex;
  float thisZLocation;
  float closestZLocation;
  iMeshWrapper *closestMesh;
  iMeshWrapper *nextMesh;
  csBox2 screenBoundingBox;
  csBox3 bbox3;

  if (baseMesh)
  {
    closestMesh = baseMesh;
    closestZLocation = baseMesh->GetScreenBoundingBox(camera, screenBoundingBox, bbox3);
    // if the baseMesh isn't in front of the camera, return
    if (closestZLocation < 0)
      return NULL;
  }
  else
  {
    closestMesh = NULL;
    closestZLocation = 32000;
  }

  // @@@ This routine ignores 2D meshes for the moment.
  iMeshList *mesh_list = engine->GetMeshes();

  for (meshIndex = 0; meshIndex < mesh_list->GetCount(); meshIndex++)
  {
    nextMesh = (iMeshWrapper*)mesh_list->Get(meshIndex);

    if (nextMesh != baseMesh)
    {
      thisZLocation = nextMesh->GetScreenBoundingBox(camera, screenBoundingBox, bbox3);
      if ((thisZLocation > 0) && (thisZLocation < closestZLocation))
      {
        if (screenBoundingBox.In(screenCoord->x, screenCoord->y))
        {
          closestZLocation = thisZLocation;
          closestMesh = nextMesh;
        }
      }
    }
  }

  return closestMesh;
}

//*********************************************************************************
//*
//* Find closest mesh to the clicked screen coordinate
//*
//*********************************************************************************
iMeshWrapper* ChimeSystemDriver::SelectMesh (iCamera *camera, csVector2 *screenCoord, float &dist)
{

	selectedMeshSect = NULL;
	iMeshWrapper *closestMesh = NULL;
	csVector3 v;
	csVector2 p (screenCoord->x, screenCoord->y);
	camera->InvPerspective (p, 1, v);
	
	//csVector3 vw = camera->Camera2World (v);
	csVector3 vw = view->GetCamera()->GetTransform().This2Other(v);

	iSector* sector = view->GetCamera ()->GetSector ();
	
	//csVector3 origin = camera->GetW2CTranslation ();
	csVector3 origin = view->GetCamera()->GetTransform().GetO2TTranslation();

	csVector3 isect;

	ChimeSector *curSect = GetCurChimeSector();
	closestMesh = curSect->SelectMesh(origin, origin + (vw-origin) * 20, isect, dist);
	if( closestMesh )
	{
		selectedMeshSect = selectedMeshNewSect = curSect;
	}

	return closestMesh;
}

//*********************************************************************************
//*
//* check if there is a collision
//*
//*********************************************************************************
bool ChimeSystemDriver::CollisionDetect(iMeshWrapper *sp, csVector3 pos, iSector *room)
{
/*	bool rc;

	//Check for collision
	csMatrix3 m;
    csOrthoTransform transf1(m, pos);
	csCollider *curCollider = csCollider::GetCollider(*sp);

	collide_system->ResetCollisionPairs();
    collide_system->SetOneHitOnly(true);

	if(curCollider->Collide (*room, &transf1))
		return true;
	 for (int i = 0; i < room->meshes.Length(); i++)
	 {
        iMeshWrapper* spr = (iMeshWrapper*)room->meshes[i];
		csOrthoTransform transf2(m, spr->GetMovable ().GetPosition());

	    if(rc = curCollider->Collide (*spr, &transf1, &transf2))
		{
			return true;
		}
	 }
*/	 return false;
}

//*********************************************************************************
//*
//* Update and signal server about the new pos of the object.
//*
//*********************************************************************************

bool ChimeSystemDriver::UpdateObjPos()
{
	if(!meshSelected) return false;

	if(strcmp(selectedMeshSect->GetUrl(), selectedMeshNewSect->GetUrl()))
	{
		comm.DeleteObject(selectedMeshSect->GetUrl(),(char*)selectedMesh->QueryObject()->GetName());
	}
	csVector3 newPos = selectedMesh->GetMovable()->GetPosition();
	newPos = newPos - selectedMeshNewSect->GetOrigin();
	comm.MoveObject(selectedMeshNewSect->GetUrl(), (char*)selectedMesh->QueryObject()->GetName(), newPos.x, newPos.y, newPos.z);

	return true;
}


//*********************************************************************************
//*
//* handle all nework events which are received by ChimeSystemDriver::GetFunction()
//*
//*********************************************************************************

bool ChimeSystemDriver::HandleNetworkEvent(int method, char *params)
{
	bool result = false;

	switch( method )
	{
	case s_moveObject:
		{
			char roomUrl[MAX_URL];
			char objectUrl[MAX_URL];
			float x, y, z;

			sscanf(params, "%s %s %f %f %f", roomUrl, objectUrl, &x, &y, &z);
			result = MoveObject(roomUrl, objectUrl, x, y, z);
			break;
		}

		
	case s_moveUser:
	case c_moveUser:
		{
			char roomUrl[MAX_URL];
			char username[MAX_URL];
			char ip_address[MAX_URL];
			float x, y, z;

			sscanf(params, "%s %s %s %f %f %f", roomUrl, username, ip_address, &x, &y, &z);
			result = MoveUser(roomUrl, username, ip_address, x, y, z);
			break;
		}
	case s_addObject:
		{
			char roomUrl[MAX_URL];
			char objectUrl[MAX_URL];
			char shape[MAX_URL];
			char Class[MAX_URL];
			char subClass[MAX_URL];
			float x, y, z;

			sscanf(params, "%s %s %s %s %s %f %f %f", roomUrl, objectUrl, shape, Class, subClass, &x, &y, &z);
			result = AddObject(roomUrl, objectUrl, shape, Class, subClass, x, y, z);
			break;
		}

	case s_enteredRoom:
		{
			char newRoomUrl[MAX_URL];
			char username[MAX_URL];
			char ip_address[MAX_URL];
			float x, y, z;

			sscanf(params, "%s %s %s %f %f %f", username, ip_address, newRoomUrl, &x, &y, &z);

			//don't add me as a user
			char my_username[50];
			info->GetUsername(my_username);	

			if (strcmp(username, "") != 0 && strcmp(username, my_username) != 0)
				result = AddUser(newRoomUrl, username, ip_address, "mdl1", 3, 0, 2);  //NEEDS TO BE FIXED - NOT HARDCODED

			break;
		}


	case s_leftRoom:
		{
			char oldRoomUrl[MAX_URL];
			char username[MAX_URL];
			char ip_address[MAX_URL];

			sscanf(params, "%s %s %s", username, ip_address, oldRoomUrl);
			result = DeleteUser(oldRoomUrl, username, ip_address);
			break;
		}

	case s_deleteObject:
		{
			char roomUrl[MAX_URL];
			char objectUrl[MAX_URL];

			sscanf(params, "%s %s", roomUrl, objectUrl);
			result = DeleteObject(roomUrl, objectUrl);  //used to be DeleteUser
			break;
		}

	case s_roomInfo:
		{
			result = ReadRoom(params);
			break;
		}

	case c_talk:
		{
			char ip[20];
			char username[50];
			char msg[1000];

			sscanf(params, "%s %s", ip, username);
			
			char *p = &params[strlen(ip) + strlen(username) + 2];

			if (!p)
				return false;

			strcpy(msg, p);
			result = ReceivedTalkMessage(username, ip, msg);
			break;
		}
	}

	return result;
}



//*********************************************************************************
//*
//* Talk message has been received.
//*
//*********************************************************************************
bool ChimeSystemDriver::ReceivedTalkMessage(const char *username, const char* ip, const char* msg) {
	app->chatWindow->ShowMessage(username, msg);
	return true;
}


//*********************************************************************************
//*
//* Move a specified object.
//*
//*********************************************************************************
bool ChimeSystemDriver::MoveObject(char *roomUrl, char *objectUrl, float x, float y, float z)
{
	ChimeSector *sec = NULL;
	iSector	*room = NULL;

	sec = FindSector( roomUrl );
	if(!sec) return false;

	csVector3 newPos = sec->GetOrigin();
	newPos.x += x;
	newPos.y += y;
	newPos.z += z;

	iMeshWrapper* obj;
	obj = sec->FindObject(objectUrl, room);
	if( !obj ) return false;

	room = sec->FindRoomContainingPoint(newPos);
	if( !room ) return false;

	obj->GetMovable()->SetPosition(room, newPos);
	obj->GetMovable()->UpdateMove();
	return true;
}

//*********************************************************************************
//*
//* Move a specified user.
//*
//*********************************************************************************
bool ChimeSystemDriver::MoveUser(char *roomUrl, char *username, char *ip_address, float x, float y, float z)
{
	
	ChimeSector *sec = NULL;
	iSector	*room = NULL;

	sec = FindSector( roomUrl );
	if(!sec) return false;

	char *userID = sec->MakeUserID(username, ip_address);
	if (!userID)
		return false;

	csVector3 newPos = sec->GetOrigin();
	newPos.x += x;
	newPos.y += y;
	newPos.z += z;

	iMeshWrapper* obj = NULL;
	obj = sec->FindObject(userID, room);
	if( !obj ) return false;

	room = sec->FindRoomContainingPoint(newPos);
	if( !room ) return false;

	obj->GetMovable()->SetPosition(room, newPos);
	obj->GetMovable()->UpdateMove();

	return true;

}

//*********************************************************************************
//*
//* Add a specified object in a given room
//*
//*********************************************************************************
bool ChimeSystemDriver::AddObject(char *roomUrl, char *objectUrl, char *shape, char *Class, char *subClass,
							 float x, float y, float z)
{

	ChimeSector *sec = FindSector( roomUrl );
	if(!sec) return false;

	iSector *room = sec->GetRoom(0);

	if( !room ) return false;

	csVector3 objPos = sec->GetOrigin();
	objPos.x += x;
	objPos.y += y;
	objPos.z += z;

	iMeshWrapper *m = AddMeshObj(shape, objectUrl, room, objPos, 1);
	if( !m ) return false;

	//Add collision detection
	iMeshWrapper *sp = FindObject(room, objectUrl);
	iPolygonMesh* mesh;
	iMeshObject *s = sp->GetMeshObject();
	mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
	if (mesh)
	{
		//(void)new csCollider (*sp, collide_system, mesh);
		//mesh->DecRef ();
	}


	return true;
}


//*********************************************************************************
//*
//* Delete a specified object
//*
//*********************************************************************************
bool ChimeSystemDriver::DeleteObject(char *roomUrl, char *objectUrl)
{
	ChimeSector *sec = FindSector( roomUrl );
	if( !sec ) return false;

	iSector * room;
	room = sec->GetRoom(0);
	if( !room ) return false;

	iMeshWrapper* obj;

	obj = FindObject(room, objectUrl);
	//Check if user is in hallway
	if(!obj)
	{
		room = sec->GetHallway();
		obj = FindObject(room, objectUrl);
	}
	//Check if user is in connector1
	if(!obj)
	{
		room = sec->GetConn1();
		obj = FindObject(room, objectUrl);
	}
	//Check if user is in connector2
	if(!obj)
	{
		room = sec->GetConn2();
		obj = FindObject(room, objectUrl);
	}
	if(!obj) return false;

	DeleteMeshObj(obj);

	return true;
}

//*********************************************************************************
//*
//* Add a specified user in a given room
//*
//*********************************************************************************
bool ChimeSystemDriver::AddUser(char *roomUrl, char *username, char *ip_address, char *shape, float x, float y, float z)
{

	ChimeSector *sec = FindSector( roomUrl );
	if(!sec) return false;

	iSector *room = sec->GetRoom(0);

	if( !room ) return false;

	csVector3 userPos = sec->GetOrigin();
	userPos.x += x;
	userPos.y += y;
	userPos.z += z;

	//Add user to the userList for UDP communication
	if(!sec->AddUser(username, ip_address))
		return true;

	char name[100];

	strcpy(name, username);
	strcat(name, " ");
	strcat(name, ip_address);

	iMeshWrapper *m = AddMeshObj(shape, name, room, userPos, 1);
	if( !m ) return false;


	//Add collision detection
	iMeshWrapper *sp = FindObject(room, name);
	iPolygonMesh* mesh;
	iMeshObject *s = sp->GetMeshObject();
	mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
	if (mesh)
	{
		//(void)new csCollider (*sp, collide_system, mesh);
		//mesh->DecRef ();
	}


	return true;
}

//*********************************************************************************
//*
//* Delete a specified user from a given room
//*
//*********************************************************************************
bool ChimeSystemDriver::DeleteUser(char *roomUrl, char *username, char *ip_address)
{
	ChimeSector *sec = FindSector( roomUrl );
	if( !sec ) return false;

	char *userID = sec->MakeUserID(username, ip_address);

	iSector * room;
	room = sec->GetRoom(0);
	if( !room ) return false;

	iMeshWrapper* user;

	user = FindObject(room, userID);
	//Check if user is in hallway
	if(!user)
	{
		room = sec->GetHallway();
		user = FindObject(room, userID);
	}
	//Check if user is in connector1
	if(!user)
	{
		room = sec->GetConn1();
		user = FindObject(room, userID);
	}
	//Check if user is in connector2
	if(!user)
	{
		room = sec->GetConn2();
		user = FindObject(room, userID);
	}
	if(!user) return false;

	DeleteMeshObj(user);
	sec->deleteUser(userID);

	return true;
}


//*********************************************************************************
//*
//* Change shape of a given 3d object (FUTURE WORK)
//*
//*********************************************************************************
 
bool ChimeSystemDriver::ChangeClass(char *desc)
{
	//Future work

	return true;
}

//*********************************************************************************
//*
//* Read a given room description
//*
//*********************************************************************************
bool ChimeSystemDriver::ReadRoom(char *desc)
{
	char roomUrl[200];
	csVector3 doorPos(0, 0, 0);
	ChimeSector *sec1 = reqAtSec;

	char my_username[50];
	info->GetUsername(my_username);

	if (strcmp(my_username, "") == 0)
		return false;

	if( nextSector == NUM_SECT )
	{
		//Remove the firstSector from the chime world to
		//make room for new ChimeSector;
		ReshuffleSectors();
	}

	ChimeSector *&sec2 = sector[nextSector];

	//If chime sector already exist at next sector location, remove it.
	if( sec2 )
	{
		RemoveChimeSector(sec2);
	}

	sscanf(desc, "%s", roomUrl);

	if( !strcmp(roomUrl, reqRoomUrl) )
	{
		if( sec1 )
		{
			sec1->GetHallwayDoorLoc(reqAtDoor, doorPos);
			//sec1->GetDoorInfo(reqAtDoor, doorPos, doorPoly, hallway);
		}
		else
		{
			nextSector++;	//This is the first chime sector of the chime world.
		}

		sec2 = new ChimeSector(Sys, engine);
		sec2->BuildDynamicRoom2(desc, doorPos, collide_system);

		comm.SubscribeRoom(sec2->GetUrl(), my_username);

		if( sec1 )
		{
			  sec1->ConnectSectors(sec2, reqAtDoor);
		}
		else
		{
			view->GetCamera()->SetSector (sec2->GetRoom(0));
			view->GetCamera ()->Move (*(sec2->GetCamLocation()));
			view->SetRectangle (0, 0, FrameWidth, FrameHeight);

			csVector3 newPos = view->GetCamera()->GetTransform().GetOrigin();
			csVector3 roomOrigin = sec2->GetOrigin();
			newPos -= roomOrigin;
			char my_ip_address[50];
			info->GetMyIPAddress(my_ip_address);

			if (strcmp(my_ip_address, "") == 0)
				return false;

			comm.UserEnteredRoom(my_username, my_ip_address, sec2->GetUrl(), newPos.x, newPos.y, newPos.z);
		}
	}

	return true;
}

//*********************************************************************************
//*
//* get the IP address of this machine
//*
//*********************************************************************************
char* ChimeSystemDriver::getLocalIP()
{

	WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		//just return some bogus IP
        return "129.1.1.1";
    }

    char hostname[80];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        cerr << "Error " << WSAGetLastError() <<
                " when getting local host name." << endl;
		//just return some bogus IP
        return "129.1.1.1";
    }
    cout << "Host name is " << hostname << "." << endl;

    struct hostent *ip_list = gethostbyname(hostname);
    if (ip_list == 0) {
        cerr << "Bad host lookup." << endl;
		//just return some bogus IP
        return "129.1.1.1";
    }

	struct in_addr addr;
    memcpy(&addr, ip_list->h_addr_list[0], sizeof(struct in_addr));
    cout << "Address " << inet_ntoa(addr) << endl;

	WSACleanup();

    return inet_ntoa(addr);
}


//*********************************************************************************
//*
//* print an error using the error box
//*
//*********************************************************************************
void ChimeSystemDriver::ShowError(const char *component, const char* error_msg, const char *variable) {
	char tmp[50];
	sprintf(tmp, "%s\n%s : %s\n", component, error_msg, variable);

	if (component == NULL || error_msg == NULL || variable == NULL)
		Report(CS_REPORTER_SEVERITY_ERROR, "Unknown error has occurred");
	else
		Report(CS_REPORTER_SEVERITY_ERROR, tmp);
}

//*********************************************************************************
//*
//* print an error using the error box
//*
//*********************************************************************************
void ChimeSystemDriver::ShowError(const char *component, const char* error_msg) {
	char tmp[50];
	sprintf(tmp, "%s\n%s\n", component, error_msg);

	if (component == NULL || error_msg == NULL)
		Report(CS_REPORTER_SEVERITY_ERROR, "Unknown error has occurred");
	else
		Report(CS_REPORTER_SEVERITY_ERROR, tmp);

}

//*********************************************************************************
//*
//* Change the mouse cursor
//*
//*********************************************************************************
void ChimeSystemDriver::ChangeMouseCursor() {
	static csMouseCursorID mousecursors [] =
	{
		csmcNone, csmcArrow, csmcLens, csmcCross, csmcPen, csmcMove,
		csmcSizeNWSE, csmcSizeNESW, csmcSizeNS, csmcSizeEW, csmcStop, csmcWait
	};

	if (!mousecursor) 
		mousecursor = 1;

	mousecursor = (mousecursor + 1) % (sizeof (mousecursors) / sizeof (int));
    app->SetMouse (mousecursors [mousecursor]);
}