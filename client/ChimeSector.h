// chimeSector.h: interface for the chimeSector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
#define AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "csengine/meshobj.h"
#include "csengine/engine.h"
#include "csengine/collider.h"

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


class csSector;
class csEngine;
struct iCollideSystem;

#define MAX_DOOR	20		//Maximum number of doors in the sector
#define MAX_URL		200		//Maximum length of a url
class chimeSector
{
	char roomUrl[MAX_URL];				//Url represented by this sector
	csSector* roomList[2];				//List of all the rooms in the sector
	csSector *connector1, *connector2;	//Connector room
	csSector* hallway;					//Hallway
	int		  numRooms;					//Number of rooms in th sector.
	int		  numDoors;					//Number of active doors in the sector.
	csEngine* engine;					//Engine that will render this room
	csSystemDriver *System;
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
	csStrVector containerList;				//List of containers

	csPolygon3D *doorList[MAX_DOOR];

	//Polygons representing doors of the hallway
	csPolygon3D **hallRightDoor;
	csPolygon3D **hallLeftDoor;
	csPolygon3D **hallBackDoor;
	csPolygon3D **hallFrontDoor;

	//Polygons representing doors of a focused room
	csPolygon3D **roomFrontDoor;
	csPolygon3D **roomBackDoor;

	//Polygons representing doors of the connector room
	csPolygon3D **conn1FrontDoor;
	csPolygon3D **conn1BackDoor;
	csPolygon3D **conn2FrontDoor;
	csPolygon3D **conn2BackDoor;

	char doorUrl[MAX_DOOR][MAX_URL];
	chimeSector *doorSec[MAX_DOOR];		//sectors represented by the hallway doors

	int linkedDoor;
	chimeSector *linkedSector;

	//Initializes standard vectors.
	bool InitStdVectors();
	//Initialize door vectors
	bool InitDoors();
	//Build a standard room of a given size at given location
	bool BuildStandardRoom(csSector *room, csVector3 const &size, csVector3 const &pos);
	// Build a standard hallway of a given size at given location
	bool BuildHallway(csSector *room, csVector3 const &size, csVector3 const &pos);
	// Build a standard connector that connects a oom with a hallway
	bool BuildStandardConnector(csSector *room, csVector3 const &size, csVector3 const &pos, int type);
	//Build a given wall of a given room
	csPolygon3D * BuildWall(csSector *room, csVector3 const &size, csVector3 const &pos, int type, csMaterialWrapper *txt, csVector3 const &txtSize);

	//Build right wall with doors
	bool buildRightDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset,csPolygon3D *pList[]);
	//Build left wall with doors
	bool buildLeftDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset,csPolygon3D *pList[]);
	//Build Front wall with doors
	bool buildFrontDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset,csPolygon3D *pList[]);
	//Build back wall with doors
	bool buildBackDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset,csPolygon3D *pList[]);
public:

	csSector* GetRoom(int index);
	csSector* GetHallway() { return hallway; };
	csSector* GetConn1() { return connector1; };
	csSector* GetConn2() { return connector2; };

	static int  getStrLine(const char *buf, char *line);
	static int  escapeEndlines(char *buf);
	bool ReadRoom(char *fileName, iCollideSystem* collide_system);
	//Build a new room as specified by the data server.
	bool BuildRoom();
	//Build dynamic room as a function of number of objects in the room
	bool BuildDynamicRoom(char *fileName, const csVector3 &pos, iCollideSystem* collide_system);
	bool BuildDynamicRoom2(char *roomDesc, const csVector3 &pos, iCollideSystem* collide_system);

	//Return Default camera location of this sector
	csVector3* GetCamLocation();
	// Return location and hallway of a given hallway-door of this chime sector
	bool GetDoorInfo(int doorNum, csVector3 &location, csPolygon3D* &door, csSector* &h);

	//Set linked chime sector info
	bool SetLinkedSectorInfo(chimeSector *sect, int doorNum);
	//Get given halway door of this chime sector.
	csPolygon3D*  GetHallwayDoor(int doorNum);
	//Get BackDoor of this chime sector.
	csPolygon3D*  GetBackDoor();
	//Get spatial location of a given hallway door.
	bool GetHallwayDoorLoc(int doorNum, csVector3 & location);
	//connect "otherSect" to the "atDoor" hallway door of this sector.
	bool ConnectSectors(chimeSector *otherSect, int atDoor);
	//diconnect this sector from the linked sector
	bool DisconnectSector();
	//Add user name to the list of users in the sector
	bool AddUser(char *userID);
	//Delete user name from the list of users in the sector
	bool deleteUser(char *userID);

	bool Connect(csPolygon3D *door, csSector *hallway);
	bool SetDoorSector(int doorNum, chimeSector *sec);
	bool Disconnect();
	bool UnlinkHallwayDoors();

	chimeSector* GetDoorSector(int doorNum);
	char* GetDoorUrl(int doorNum);
	//Check If a given room belongs to this chime sector
	int IsRoomOf(csSector *room);
	//Find the room of a sector that contains this point
	csSector* FindRoomContainingPoint(csVector3 p);
	// Check If a given beam intersects any of the walls of the chimesector
	bool HitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect);
	//Check If a given beam intersects any of the hallway doors of this sector
	bool HallwayHitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect, int &doorNum);
	// Find  an object in this sector
	csMeshWrapper* FindObject(char *objectUrl, csSector *&room);
	//check if a given beam hits any of the meshes in this sector.
	csMeshWrapper* SelectMesh(const csVector3 &start, const csVector3 &end, csVector3 &isect, float &dist);

	//Adds a mesh object in the room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, csSector* where,
		csVector3 const& pos, float size);

	//Return origin point of the room
	csVector3 GetOrigin() { return roomOrigin; };
	//Get url used to representthis sector.
	char * GetUrl() { return roomUrl; };
	//Get List of users in this sector.
	csStrVector* GetUserList() { return &userList; };

	//find the type of the link
	int findType(const char *thing); 

	//Add a container to a list of containers
	bool AddContainer(char *location);

	//Remove a container from the list of containers
	bool DeleteContainer(char *location);

	chimeSector();
	chimeSector(csSystemDriver  *Sys, csEngine *e);
	virtual ~chimeSector();

};

#endif // !defined(AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
