// ChimeSector.h: interface for the ChimeSector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChimeSector_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
#define AFX_ChimeSector_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "csengine/meshobj.h"
#include "csengine/engine.h"
#include "cstool/collider.h"


#define LEFT	0
#define RIGHT	4
#define FRONT	8
#define BACK	12
#define FLOOR	16
#define ROOF	20

#define UNKNOWN	0
#define CONN1	1
#define CONN2	2
#define ROOM	3
#define HALLWAY	4

#define CONTAINER 0
#define CONNECTOR 1
#define COMPONENT 2
#define USER 3

#define ACTIVE_DOOR_ALPHA 25
#define INACTIVE_DOOR_ALPHA 100

struct iSector;
struct iEngine;
class ChimeSystemDriver;

struct iCollideSystem;

#define MAX_DOOR	20		//Maximum number of doors in the sector
#define MAX_URL		200		//Maximum length of a url

#define DOOR_NOT_FOUND -1
#define INACTIVE_DOOR  1
#define ACTIVE_DOOR 2

class ChimeSector
{
	typedef char* UserIDType;

	char roomUrl[MAX_URL];				//Url represented by this sector
	iSector* roomList[2];				//List of all the rooms in the sector
	iSector *connector1, *connector2;	//Connector room
	iSector* hallway;					//Hallway
	int		  numRooms;					//Number of rooms in th sector.
	int		  numDoors;					//Number of active doors in the sector.

	iEngine* engine;					//Engine that will render this room
	ChimeSystemDriver *System;
	csVector3 stdVector[24];			//Standard vector used to build walls


	csVector3 camLocation;				//Default camera location
	csVector3 firstDoorLocation;
	csVector3 roomOrigin;				//one of the corners of the room from which
										//object positions are offset.
	csVector3 roomSize;
	csVector3 connSize;
	csVector3 hallwaySize;

	csStrVector	connList;				//List of connectors
	csStrVector userList;				//List of users in the room
	csStrVector containerList;			//List of containers

	iPolygon3D *doorList[MAX_DOOR];

	//Polygons representing doors of the hallway
	iPolygon3D **hallRightDoor;
	iPolygon3D **hallLeftDoor;
	iPolygon3D **hallBackDoor;
	iPolygon3D **hallFrontDoor;

	//Polygons representing doors of a focused room
	iPolygon3D **roomFrontDoor;
	iPolygon3D **roomBackDoor;

	//Polygons representing doors of the connector room
	iPolygon3D **conn1FrontDoor;
	iPolygon3D **conn1BackDoor;
	iPolygon3D **conn2FrontDoor;
	iPolygon3D **conn2BackDoor;

	char doorUrl[MAX_DOOR][MAX_URL];
	ChimeSector *doorSec[MAX_DOOR];		//sectors represented by the hallway doors

	int linkedDoor;
	ChimeSector *linkedSector;

	//Initializes standard vectors.
	bool InitStdVectors();
	//Initialize door vectors
	bool InitDoors();
	//Build a standard room of a given size at given location
	bool BuildStandardRoom(iSector *room, csVector3 const &size, csVector3 const &pos);
	// Build a standard hallway of a given size at given location
	bool BuildHallway(iSector *room, csVector3 const &size, csVector3 const &pos);
	// Build a standard connector that connects a oom with a hallway
	bool BuildStandardConnector(iSector *room, csVector3 const &size, csVector3 const &pos, int type);
	//Build a given wall of a given room
	
	iPolygon3D *BuildWall(iThingState *walls, char *wall_name, csVector3 const &size, csVector3 const &pos, 
									 int type, iMaterialWrapper *txt, csVector3 const &txtSize);

	//convenience method to build a wall
	iPolygon3D* BuildDoor(iThingState *walls, int *door_num, csVector3 const &size, csVector3 const &pos, 									 int type, iMaterialWrapper *txt, csVector3 const &txtSize);

	//Build right wall with doors
	bool buildRightDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, 
									 iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[]);

	
	//Build left wall with doors
	bool buildLeftDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[]);

	//Build Front wall with doors
	bool buildFrontDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[]);
	
	//Build back wall with doors
	bool buildBackDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[]);
public:

	iSector* GetRoom(int index);
	iSector* GetHallway() { return hallway; };
	iSector* GetConn1() { return connector1; };
	iSector* GetConn2() { return connector2; };

	//make a UserID
	UserIDType MakeUserID(const char *username, const char* ip_address);

	static int  getStrLine(const char *buf, char *line);
	static int  escapeEndlines(char *buf);
	bool ReadRoom(char *fileName, iCollideSystem* collide_system);
	//Build a new room as specified by the data server.
	bool BuildRoom();
	//Build dynamic room as a function of number of objects in the room
	bool BuildDynamicRoom2(char *roomDesc, const csVector3 &pos, iCollideSystem* collide_system);

	//Return Default camera location of this sector
	csVector3* GetCamLocation();
	// Return location and hallway of a given hallway-door of this chime sector
	bool GetDoorInfo(int doorNum, csVector3 &location, iPolygon3D* &door, iSector* &h);

	//Set linked chime sector info
	bool SetLinkedSectorInfo(ChimeSector *sect, int doorNum);
	//Get given halway door of this chime sector.
	iPolygon3D*  GetHallwayDoor(int doorNum);
	//Get BackDoor of this chime sector.
	iPolygon3D*  GetBackDoor();
	//Get spatial location of a given hallway door.
	bool GetHallwayDoorLoc(int doorNum, csVector3 & location);
	//connect "otherSect" to the "atDoor" hallway door of this sector.
	bool ConnectSectors(ChimeSector *otherSect, int atDoor);
	//diconnect this sector from the linked sector
	bool DisconnectSector();
	//Add user name to the list of users in the sector
	bool AddUser(char *username, char *IP_address);
	//Delete user name from the list of users in the sector
	bool deleteUser(char *username);
	bool deleteUser(char *username, char *ip_address);

	bool Connect(iPolygon3D *door, iSector *hallway);
	bool SetDoorSector(int doorNum, ChimeSector *sec);
	bool Disconnect();
	bool UnlinkHallwayDoors();

	ChimeSector* GetDoorSector(int doorNum);
	char* GetDoorUrl(int doorNum);
	//Check If a given room belongs to this chime sector
	int IsRoomOf(iSector *room);
	//Find the room of a sector that contains this point
	iSector* FindRoomContainingPoint(csVector3 p);
	// Check If a given beam intersects any of the walls of the ChimeSector
	bool HitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect);
	//Check If a given beam intersects any of the hallway doors of this sector
	int HallwayHitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect, int &doorNum);
	// Find  an object in this sector
	iMeshWrapper* FindObject(char *objectUrl, iSector *&room);
	//check if a given beam hits any of the meshes in this sector.
	iMeshWrapper* SelectMesh(const csVector3 &start, const csVector3 &end, csVector3 &isect, float &dist);

	//Adds a mesh object in the room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, iSector* where,
		csVector3 const& pos, float size);

	//Return origin point of the room
	csVector3 GetOrigin() { return roomOrigin; };
	//Get url used to representthis sector.
	char * GetUrl() { return roomUrl; };
	//Get List of users in this sector.
	csStrVector* GetUserList() { return &userList; };

	//find the type of the link
	int findType(const char *thing); 

	//add collision detection to a room
	bool AddCollisionDetection(iSector *room, iCollideSystem* collide_system);

	//Add a container to a list of containers
	bool AddContainer(char *location);

	//Remove a container from the list of containers
	bool DeleteContainer(char *location);

	ChimeSector();
	ChimeSector(ChimeSystemDriver  *Sys, iEngine *e);
	virtual ~ChimeSector();

	//create label
	bool CreateLabel(iThingState *walls, char *label_name, csVector3 const &size, csVector3 const &pos, 
									 int type, iMaterialWrapper *txt, csVector3 const &txtSize);
	//replace a door url with something else
	bool ReplaceDoorUrl(int doorNum, char *string);

};


#endif // !defined(AFX_ChimeSector_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
