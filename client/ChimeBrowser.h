//Author: Navdeep S. Tinna
//Date	: 03/05/2001
//File	: ChimeBrowser.cpp

//Copyright 2001 PSL lab, Columbia University



//*********************************************************************************
//*
//*  chimeBrowser.cpp defines the main Class that is responsible for browsing and
//*	 displaying 3D view.
//*
//*********************************************************************************

#ifndef __CHIME_BROWSER_H__
#define __CHIME_BROWSER_H__


//Includes from Crystal space
#include "cssys/sysdriv.h"
#include "csengine/meshobj.h"

//Forward declaration for classes
class csEngine;
class csView;
class ClientComm;
class chimeComm;
class chimeBrowser;

#include "ChimeMenu.h"
#include "ChimeComm.h"
#include "comm_client/udp.h"
#include "ChimeSector.h"

#define NUM_SECT 5
#define CONTAINER 0
#define CONNECTOR 1
#define COMPONENT 2
#define USER 3

//menu options
#define POPUP_EDIT_WITH_DEFAULT_APP 88801
#define POPUP_SELECT_APP 88802
#define POPUP_CARRY 88803
#define POPUP_DROP 88804
#define POPUP_DELETE 88805
#define POPUP_UNDELETE 88806
#define POPUP_INCREASE_SECURITY 88807
#define POPUP_DECREASE_SECURITY 88808
#define POPUP_PROPERTIES 88809


class chimeBrowser: public SysSystemDriver
{
  typedef SysSystemDriver superclass;

private:

	csComponent *CoordinateConvertor;		//use this to convert coordinates to Chime World Coordinates
	csMenu *menu;					//the popup menu object
	bool menu_drawn;				//whether the popup menu is currently on the screen

	ChimeMenu *app;					//the app
	char username[80];				//the username of the user using CHIME

    csView      *view;			//View of the curSector.
	csEngine* engine;			//Pointer to the Crystal-Space engine;
	chimeSector *sector[NUM_SECT];	//NumSect sectors that can exist concurrently
	int curSector, nextSector;
	char userID[80];

	bool freeLook;				//Toggle for turning free look on or off
	csVector3 camAngle;			//Current camera orientation.
	float SPEED;				//Camera movement speed factor

	chimeSector *currentSector; //

	csMeshWrapper *selectedMesh;	//Currently selected mesh for movement.
	bool meshSelected;
	csVector3   selectedMeshPos;
	float selectedMeshDist;
	chimeSector *selectedMeshSect;
	chimeSector *selectedMeshNewSect;

	//Currently requested Url
	char reqRoomUrl[MAX_URL];
	chimeSector *reqAtSec;
	int		   reqAtDoor;

	//Collision detection plugin.
	iCollideSystem* collide_system;

	//If 3D view is active or not
	bool active;

	//Path to the internet browser
	char browserPath[400];

	//Remove selected chime sector.
	bool RemoveChimeSector(chimeSector* &sec2);
	//Remove the first sector of the chimeworld
	//and push remaining sectors up.
	bool ReshuffleSectors();
	// Find room corresponding to a given room url
	csSector* FindRoom(char *roomUrl);
	// Find  an object in a given room
	csMeshWrapper* FindObject(csSector *room, char *objectUrl);
	// Find sector corresponding to a given room url
	chimeSector* FindSector(char *roomUrl);
	// Find sector corresponding to a given room
	chimeSector* FindSector(csSector *room);
	//User has moved send notification to all clients.
	void UserMoved();
	//Update and signal server about the new pos of the object.
	bool UpdateObjPos();
	//Move selected mesh as mouse moves.
	bool MoveSelectedMesh(iEvent &Event);
	//Select a mesh at a given screen coordinate.
	csMeshWrapper* SelectMesh (csCamera *camera, csVector2 *screenCoord, float &dist);
	//Check if given mesh intersects with the walls of the room or any of the other
	//objects in the room.
	bool CollisionDetect(csMeshWrapper *sp, csVector3 pos, csSector *room);
	csSector* FindSectContainingPoint(csVector3 &pos, chimeSector *&sect);

	//			**** Recieved info handling functions ***
	bool HandleNetworkEvent(int method, char *params);
	// Move a specified object
	bool MoveObject(char *roomUrl, char *objectUrl, float x, float y, float z);
	// Move a specified user
	bool chimeBrowser::MoveUser(char *roomUrl, char *userID, float x, float y, float z);
	// Add a specified object in a given room
	bool chimeBrowser::AddObject(char *roomUrl, char *objectUrl, char *shape, char *Class, char *subClass,
							 float x, float y, float z);
	// Delete a specified object
	bool chimeBrowser::DeleteObject(char *roomUrl, char *objectUrl);
	// Add a specified user in a given room
	bool chimeBrowser::AddUser(char *roomUrl, char *userID, char *shape, float x, float y, float z);
	// Add a specified user in a given room
	bool chimeBrowser::DeleteUser(char *roomUrl, char *userID);
	// Change shape of a givn 3d object
	bool chimeBrowser::ChangeClass(char *desc);
	// Read a given room description
	bool chimeBrowser::ReadRoom(char *desc);

	//Draw a menu
	bool chimeBrowser::DrawMenu(csVector2 screenPoint);
	//Get rid of Popup menu
	bool chimeBrowser::WipePopupMenu();
	//Handle an event originating from the popup menu
	bool chimeBrowser::HandleMenuEvent(iEvent &Event); 

	char testRoom[500], google[500], google2[500], google3[500];

	///***** Comm section
	volatile HANDLE hMutex; // Create a Mutex object
	
	//communication helper class
	chimeComm comm;
	//this is the class that we will use for all communication
	ClientComm *comm_client;


	//Convert the Coordinates from Local To Global Coordinates
	bool chimeBrowser::ConvertCoordinates(csVector2 *screenCoord);

public:

	//transport to some room
	bool chimeBrowser::TransportToRoom(char *name); 

	//a pointer so we can access the history Window
	HistoryWindow *historyWindow;

	//set a pointer to the history window so the system can update it as necessary
	void SetHistoryWindow(HistoryWindow *historyWindow);
	
	//Show an error
	void ShowError(const char *component, const char* error_msg);
	void ShowError(const char *component, const char* error_msg, const char *variable);

	// Get the view.
	csView* GetView () { return view; }
	
	//tell ChimeBrowser where to find the app
	void chimeBrowser::setCSApp(ChimeMenu *app);

	//set the coordinate convertor
	void chimeBrowser::setCoordinateConvertor(csComponent *Parent);

	//Load a mesh object from a file.
	bool LoadMeshObj (char *filename, char *templatename, char* txtname);
	//Add mesh object in a given room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, csSector* where,
		csVector3 const& pos, float size);
	//Delete mesh object in a given room at a given location.
	bool DeleteMeshObj(csMeshWrapper *mesh);
	//Find closest mesh to the clicked screen coordinate
	csMeshWrapper* FindNextClosestMesh (csMeshWrapper *baseMesh,
		csCamera *camera, csVector2 *screenCoord);
	//Load chime objects and textures specified in a given file.
	bool LoadChimeLib(char *fileName);
	//Transport user from a given room to a new room
	bool Transport(csSector *room, csVector3 pos, csVector3 lookPos, csVector3 lookUp);
	//Build and link sec2 to a given hallway-door of sec1
	bool LinkChimeSectors(chimeSector *sec1, int door, chimeSector *&sec2, char *link);
	// Returns the current chime sector of a user/camera
	chimeSector* GetCurChimeSector();
	//Handles any mouse or keyboard event.
	virtual bool HandleEvent (iEvent &Event);
	//Handles all the keyboard event.
	bool HandleKeyEvent (iEvent &Event);
	// Function responsible for hndling right mouse button click
	bool HandleRightMouseClick(iEvent &Event);
	// Function responsible for hndling left mouse button click
	bool HandleLeftMouseClick(iEvent &Event);
	//Handle double click on the left mouse button
	bool HandleLeftMouseDoubleClick(iEvent &Event);
	//Prepares NextFrame
	virtual void NextFrame();
	//Refresh 3D Display
	void Refresh3D ();
	//Stop updating 3D view
	void Stop3D ();
	//Start updating 3D view
	void Start3D ();
	//Method to initialize crystal-space engine.
	virtual bool Initialize (int argc, const char* const argv[],const char *iConfigName);
	//Default constructor
	chimeBrowser();
	//write a small message in the bottom left corner
	void writeMessage();

	//get the IP of this machine
	char* getLocalIP();


	//Destructor to clean up and shutdown the engine.
	virtual ~chimeBrowser();


	///Comm section
	void GetFunction(int method, char *received);
};


#endif
