//Author: Navdeep S. Tinna
//Date	: 03/05/2001
//File	: ChimeSystemDriver.cpp

//Copyright 2001 PSL lab, Columbia University



//*********************************************************************************
//*
//*  ChimeSystemDriver.cpp defines the main Class that is responsible for browsing and
//*	 displaying 3D view.
//*
//*********************************************************************************

#ifndef __CHIME_BROWSER_H__
#define __CHIME_BROWSER_H__


//Includes from Crystal space
#include "cssys/sysdriv.h"
#include "csengine/meshobj.h"

//Forward declaration for classes
struct iEngine;
struct iVirtualClock;
struct iCollideSystem;
struct iPerfStats;
struct iModelConverter;
struct iCrossBuilder;
struct iVFS;
struct iLoader;
struct iMotionManager;
class csView;

class ClientComm;
class chimeComm;
class ChimeSystemDriver;
class ChimeSector;
class InfoStorer;
class ChimeApp;

//#include "ChimeApp.h"
#include "ChimeWindow.h"
#include "ChimeComm.h"
#include "comm_client/ClientComm.h"
#include "ChimeLabeling.h"
//#include "ChimeSector.h"

//changes the number of sectors that need to be opened to delete a sector
#define NUM_SECT 100

//just types which we use
#define CONTAINER 0
#define CONNECTOR 1
#define COMPONENT 2
#define USER 3

#ifndef MAX_URL
#define MAX_URL		200		//Maximum length of a url
#endif

//popup menu options
#define OBJECT_EDIT_WITH_DEFAULT_APP 88801
#define OBJECT_SELECT_APP 88802
#define OBJECT_CARRY 88803
#define OBJECT_DROP 88804
#define OBJECT_DELETE 88805
#define OBJECT_UNDELETE 88806
#define OBJECT_INCREASE_SECURITY 88807
#define OBJECT_DECREASE_SECURITY 88808
#define OBJECT_PROPERTIES 88809


//define popup menu options for the door
#define DOOR_OPEN_LINK 88810
#define DOOR_LINK_SOMEWHERE_ELSE 88811

class ChimeSystemDriver
{
  //typedef SysSystemDriver superclass;
  //DECLARE_TYPED_VECTOR (ptTexVec, ProcTexture);
  //ptTexVec TexVec;

private:
	int FrameWidth;
	int FrameHeight;

	/// Timing.
	float timeFPS;


	/**
	 * The main engine interface
	 * (when interface will be complete, iEngine will not be needed anymore)
	 */

	 // The main renderers texture manager
	iTextureManager *main_txtmgr;

	InfoStorer *info;					//pointer to all the info about this session
	csComponent *CoordinateConvertor;		//use this to convert coordinates to Chime World Coordinates
	csMenu *menu;					//the popup menu object
	bool menu_drawn;				//whether the popup menu is currently on the screen

	ChimeApp *app;					//the app

	ChimeSector *sector[NUM_SECT];	//NumSect sectors that can exist concurrently
	int curSector, nextSector;

	bool freeLook;				//Toggle for turning free look on or off
	csVector3 camAngle;			//Current camera orientation.
	float SPEED;				//Camera movement speed factor

	ChimeSector *currentSector;		//The current Sector being looked at

	iMeshWrapper *selectedMesh;	//Currently selected mesh for movement.
	bool meshSelected;
	csVector3   selectedMeshPos;
	float selectedMeshDist;
	ChimeSector *selectedMeshSect;
	ChimeSector *selectedMeshNewSect;

	//Currently requested Url
	char reqRoomUrl[MAX_URL];
	ChimeSector *reqAtSec;
	int		   reqAtDoor;


	//Collision detection plugin.
	iCollideSystem* collide_system;

	//If 3D view is active or not
	bool active;

	//Path to the internet browser
	char browserPath[400];

	//Texture stuff
	void ChimeSystemDriver::InitProcTextures ();
	bool ChimeSystemDriver::OpenProcTextures ();
	void ChimeSystemDriver::AnimateProcTextures (csTicks current_time, csTicks elapsed_time);
	void ChimeSystemDriver::Init (ProcTexture *pt);


	//Remove selected chime sector.
	bool RemoveChimeSector(ChimeSector* &sec2);
	//Remove the first sector of the chimeworld
	//and push remaining sectors up.
	bool ReshuffleSectors();
	// Find room corresponding to a given room url
	iSector* FindRoom(char *roomUrl);
	// Find  an object in a given room
	iMeshWrapper* FindObject(iSector *room, char *objectUrl);
	// Find sector corresponding to a given room url
	ChimeSector* FindSector(char *roomUrl);
	// Find sector corresponding to a given room
	ChimeSector* FindSector(iSector *room);
	//User has moved send notification to all clients.
	void UserMoved();
	//Update and signal server about the new pos of the object.
	bool UpdateObjPos();
	//Move selected mesh as mouse moves.
	bool MoveSelectedMesh(iEvent &Event);
	//Select a mesh at a given screen coordinate.
	iMeshWrapper* SelectMesh (iCamera *camera, csVector2 *screenCoord, float &dist);
	//Check if given mesh intersects with the walls of the room or any of the other
	//objects in the room.
	bool CollisionDetect(iMeshWrapper *sp, csVector3 pos, iSector *room);
	iSector* FindSectContainingPoint(csVector3 &pos, ChimeSector *&sect);

	//			**** Recieved info handling functions ***
	bool HandleNetworkEvent(int method, char *params);
	// Move a specified object
	bool MoveObject(char *roomUrl, char *objectUrl, float x, float y, float z);
	// Move a specified user
	bool ChimeSystemDriver::MoveUser(char *roomUrl, char *username, char* ip_address, float x, float y, float z);
	// Add a specified object in a given room
	bool ChimeSystemDriver::AddObject(char *roomUrl, char *objectUrl, char *shape, char *Class, char *subClass,
							 float x, float y, float z);
	// Delete a specified object
	bool ChimeSystemDriver::DeleteObject(char *roomUrl, char *objectUrl);
	// Add a specified user in a given room
	bool ChimeSystemDriver::AddUser(char *roomUrl, char *username, char *ip_address, char *shape, float x, float y, float z);
	// Add a specified user in a given room
	bool ChimeSystemDriver::DeleteUser(char *roomUrl, char *username, char*ip_address);
	// Change shape of a givn 3d object
	bool ChimeSystemDriver::ChangeClass(char *desc);
	// Read a given room description
	bool ChimeSystemDriver::ReadRoom(char *desc);

	//Draw a menu
	bool ChimeSystemDriver::DrawMenu(csVector2 screenPoint);

	char testRoom[500], google[500], google2[500], google3[500];

	///***** Comm section
	volatile HANDLE hMutex; // Create a Mutex object
	
	//communication helper class
	chimeComm comm;
	//this is the class that we will use for all communication
	ClientComm *comm_client;

	//Convert the Coordinates from Local To Global Coordinates
	bool ChimeSystemDriver::ConvertCoordinates(csVector2 *screenCoord);

	//Mouse cursor stuff
	int mousecursor;
	void ChangeMouseCursor();

	//when a talk message has been received
	bool ReceivedTalkMessage(const char *username, const char* ip, const char* msg);

	//bring up the door menu
	bool BringUpDoorMenu(int doorNum, csVector2 screenpoint);
	
	//open the door
	//will work if reqAtDoor has been set otherwise will not do anything
	bool OpenDoor(char *doorUrl=NULL);

	//do all the necessary steps to setup the menu
	bool SetupMenu();

	//mark the menu to be deleted
	bool MarkPopupMenuForDeletion();

	//delete the popup menu
	bool DeletePopupMenu();

	//set the contents of the info object
	void SetInfoObject();

	//update the link on the door
	bool UpdateDoorLink(ChimeSector *sec, int doorNum, char *new_door_url);

public:

	iEngine *engine;

	iGraphics2D *myG2D;               //pointer to graphics 2D
	iGraphics3D *myG3D;				  //pointer to 3D graphics
	csView       *view;				  //View of the curSector.
	iObjectRegistry* object_reg;
	iPluginManager* plugin_mgr;
	iKeyboardDriver* kbd;
	iVirtualClock* vc;
	iPerfStats* perf_stats;
	iModelConverter* ModelConverter;
	iCrossBuilder* CrossBuilder;
	iVFS* myVFS;
	iLoader* LevelLoader;
	iMotionManager *myMotionMan;

	void Report (int severity, const char* msg, ...);

	//handle an event from some window
	bool HandleEventFromOtherWindow(iEvent &Event);
	
	//reset the local chat buddies list
	void ResetLocalChatBuddies(ChimeSector *cur_sec);

	//transport to some room
	bool ChimeSystemDriver::TransportToRoom(char *name); 

	//a pointer so we can access the history Window
	HistoryWindow *historyWindow;

	//set a pointer to the history window so the system can update it as necessary
	void SetHistoryWindow(HistoryWindow *historyWindow);
	
	//Show an error
	void ShowError(const char *component, const char* error_msg);
	void ShowError(const char *component, const char* error_msg, const char *variable);

	// Get the view.
	csView* GetView () { return view; }
	
	//tell ChimeSystemDriver where to find the app
	void ChimeSystemDriver::setCSApp(ChimeApp *app);

	//get the App associated with this system
	ChimeApp* ChimeSystemDriver::GetApp() { return app; };

	//set the coordinate convertor
	void ChimeSystemDriver::setCoordinateConvertor(csComponent *Parent);

	//Load a mesh object from a file.
	bool LoadMeshObj (char *filename, char *templatename, char* txtname);
	//Add mesh object in a given room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, iSector* where,
		csVector3 const& pos, float size);
	//Delete mesh object in a given room at a given location.
	bool DeleteMeshObj(iMeshWrapper *mesh);
	//Find closest mesh to the clicked screen coordinate
	iMeshWrapper* FindNextClosestMesh (iMeshWrapper *baseMesh,
		iCamera *camera, csVector2 *screenCoord);
	//Load chime objects and textures specified in a given file.
	bool LoadChimeLib(char *fileName);
	//Transport user from a given room to a new room
	bool Transport(iSector *room, csVector3 pos, csVector3 lookPos, csVector3 lookUp);
	//Build and link sec2 to a given hallway-door of sec1
	bool LinkChimeSectors(ChimeSector *sec1, int door, ChimeSector *&sec2, char *link);
	// Returns the current chime sector of a user/camera
	ChimeSector* GetCurChimeSector();
	//Handles any mouse or keyboard event.
	bool HandleEvent (iEvent &Event);
	
	//Handle an event originating from the popup menu
	bool ChimeSystemDriver::HandleMenuEvent(iEvent &Event); 

	//Handles all the keyboard event.
	bool HandleKeyEvent (iEvent &Event);
	// Function responsible for hndling right mouse button click
	bool HandleRightMouseClick(iEvent &Event);
	// Function responsible for hndling left mouse button click
	bool HandleLeftMouseClick(iEvent &Event);
	//Handle double click on the left mouse button
	bool HandleLeftMouseDoubleClick(iEvent &Event);
	
	//Prepares the Frame
	void SetupFrame();

	//finishes the frame
	void FinishFrame();

	//Refresh 3D Display
	void Refresh3D ();
	//Stop updating 3D view
	void Stop3D ();
	//Start updating 3D view
	void Start3D ();
	//Method to initialize crystal-space engine.
	virtual bool Initialize (int argc, const char* const argv[],const char *iConfigName, InfoStorer* info, iObjectRegistry *object_reg);
	//Default constructor
	ChimeSystemDriver();
	//write a small message in the bottom left corner
	void writeMessage();

	//get the IP of this machine
	char* getLocalIP();


	//Destructor to clean up and shutdown the engine.
	virtual ~ChimeSystemDriver();


	///Comm section
	void GetFunction(int method, char *received);
};


#endif
