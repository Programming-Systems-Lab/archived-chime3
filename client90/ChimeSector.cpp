// ChimeSector.cpp: implementation of the ChimeSector class.
//
//////////////////////////////////////////////////////////////////////
#include "cssysdef.h"
#include "cssys/system.h"
#include "ChimeSystemDriver.h"
#include "ChimeSector.h"


#include "csengine/sector.h"
#include "csengine/engine.h"
#include "csengine/texture.h"
#include "csengine/light.h"
#include "csengine/polygon.h"
#include "cstool/collider.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"

#include "ChimeApp.h"

extern ChimeSystemDriver *Sys;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChimeSector::ChimeSector()
{
	connector1= connector2 = NULL;
	hallway = NULL;
	engine = NULL;
	System = NULL;
	numRooms = 0;	//FIXIT: This should not be hardcoded.
	memset(roomList, 0, 2*sizeof(csSector*));
	InitStdVectors();
	camLocation.Set(0,0,0);

	linkedDoor = -1;
	linkedSector = NULL;

	InitDoors();
}


ChimeSector::ChimeSector(ChimeSystemDriver  *Sys, iEngine *e)
{
	connector1= connector2 = NULL;
	hallway = NULL;
	System = Sys;
	engine = e;
	numRooms = 0;	//FIXIT: This should not be hardcoded.
	memset(roomList, 0, 2*sizeof(csSector*));
	InitStdVectors();
	camLocation.Set(0,0,0);

	linkedDoor = -1;
	linkedSector = NULL;
	InitDoors();
}

ChimeSector::~ChimeSector()
{

	//FIXIT: Other rooms need to be freed.

}

//*******************************************************************************
//*
//* Convert all newlines to \\n due to Siena problem
//*
//*******************************************************************************
int ChimeSector::escapeEndlines(char *buf)
{

	int len = strlen(buf);
	int i, j = 0;
	for(i = 0; i < len; i++)
	{
		if(buf[i] == '\\' && buf[i+1] == 'n')
		{
			buf[j] = '\n';
			i++;
		}
		else
		{
			buf[j] = buf[i];
		}
		j++;
	}
	buf[j] = '\0';

	return 1;
}

int ChimeSector::getStrLine(const char *buf, char *line)
{
	int i = 0;
	int len = strlen(buf);

	while(i < len && buf[i] != '\n')
	{
		line[i] = buf[i];
		i++;
	}
	line[i] = '\0';
	i++;

	return i;
}

//*********************************************************************************
//*
//* Initializes door pointers
//*
//*********************************************************************************
bool ChimeSector::InitDoors()
{
	numDoors = 0;

	//Polygons representing doors of the hallway
	hallRightDoor = &doorList[numDoors];
	numDoors += 1;

	hallLeftDoor = &doorList[numDoors];
	numDoors += 1;

	hallBackDoor = &doorList[numDoors];
	numDoors += 1;

	hallFrontDoor = &doorList[numDoors];
	numDoors += 10;

	//Polygons representing doors of main room
	roomFrontDoor = &doorList[numDoors];
	numDoors += 1;

	roomBackDoor = &doorList[numDoors];
	numDoors += 1;

	//Polygons representing doors of the connector room
	conn1FrontDoor = &doorList[numDoors];
	numDoors += 1;

	conn1BackDoor = &doorList[numDoors];
	numDoors += 1;

	conn2FrontDoor = &doorList[numDoors];
	numDoors += 1;

	conn2BackDoor = &doorList[numDoors];
	numDoors += 1;

	for(int i = 0; i < MAX_DOOR; i++)
	{
		doorSec[i] = NULL;
		strcpy(doorUrl[i], "");
	}
	return true;
}

//*********************************************************************************
//*
//* Initializes standard vectors.
//*
//*********************************************************************************
bool ChimeSector::InitStdVectors()
{
	stdVector[LEFT+0].Set(0,1,0);
	stdVector[LEFT+1].Set(0,1,1);
	stdVector[LEFT+2].Set(0,0,1);
	stdVector[LEFT+3].Set(0,0,0);

	stdVector[RIGHT+2].Set(0,1,1);
	stdVector[RIGHT+3].Set(0,1,0);
	stdVector[RIGHT+0].Set(0,0,0);
	stdVector[RIGHT+1].Set(0,0,1);


	stdVector[FRONT+0].Set(0,1,0);
	stdVector[FRONT+1].Set(1,1,0);
	stdVector[FRONT+2].Set(1,0,0);
	stdVector[FRONT+3].Set(0,0,0);

	stdVector[BACK+2].Set(1,1,0);
	stdVector[BACK+3].Set(0,1,0);
	stdVector[BACK+0].Set(0,0,0);
	stdVector[BACK+1].Set(1,0,0);

	stdVector[FLOOR+0].Set(0,0,1);
	stdVector[FLOOR+1].Set(1,0,1);
	stdVector[FLOOR+2].Set(1,0,0);
	stdVector[FLOOR+3].Set(0,0,0);

	stdVector[ROOF+0].Set(0,0,0);
	stdVector[ROOF+1].Set(1,0,0);
	stdVector[ROOF+2].Set(1,0,1);
	stdVector[ROOF+3].Set(0,0,1);


	return true;
}

//*******************************************************************************
//*
//* Create Dummy room
//*
//*******************************************************************************
bool ChimeSector::ReadRoom(char *fileName, iCollideSystem* collide_system)
{
	if(!engine) return false;

	FILE *fp = fopen(fileName, "r");
	if(!fp) return false;
	int bufSize = 100;
	char buf[100];
	char command[80];

	iSector *room = NULL;
	iPolygon3D* p;
	iMaterialWrapper* txt;
	csVector3 pos;
	iStatLight* light;
	iLightList* light_list;
	iMaterialWrapper *iMW;

	fgets(buf, bufSize, fp);
	sscanf(buf, "%s", command);

	while( !feof(fp) && strcmp(command, "END"))
	{
		if(command[0] == ';')
		{//Skip comments
		}
		else if(!strcmp(command, "ROOM"))
		{//Read texture
			char name[80];
			int numPoly;

			//create new constructs for walls
		    iMeshWrapper *wallmesh = engine -> CreateSectorWallsMesh(room, "walls");
			iThingState *walls = SCF_QUERY_INTERFACE(wallmesh->GetMeshObject(), iThingState);

			pos.Set(0,0,0);
			sscanf(buf, "%s %s %f %f %f %d", command, name, &pos.x, &pos.y, &pos.z,&numPoly);
			roomList[numRooms] = engine->CreateSector (name);
			room = roomList[numRooms];
			numRooms++;

			for(int i = 0; i < numPoly; i++)
			{
				int numPoints;
				char str[100];
				char *curChar = buf;

				command[0] = '\0';
				fgets(buf, bufSize, fp);
				sscanf(buf, "%s", command);
				while(command[0] == ';' || command[0] == '\0')
				{
					fgets(buf, bufSize, fp);
					sscanf(buf, "%s", command);
				}

				sscanf(curChar, " %[^ ,] ", command);
				while(*curChar != ',') curChar++; curChar++;

				sscanf(curChar, " %[^ ,] ", str);
				while(*curChar != ',') curChar++; curChar++;

				txt = engine->GetMaterialList()->FindByName (str);
				iMW = SCF_QUERY_INTERFACE(txt, iMaterialWrapper);
				
				p = walls->CreatePolygon ();
				p -> SetMaterial(iMW);

				sscanf(curChar, "%d", &numPoints);
				while(*curChar != ',') curChar++; curChar++;

				for(int j = 0; j < numPoints; j++)
				{
					float x, y, z;

					sscanf(curChar, "%f %f %f", &x, &y, &z);
					while(*curChar != ',') curChar++; curChar++;
					x += pos.x; y += pos.y; z += pos.z;
					p->CreateVertex (csVector3(x, y, z));
				}
				int ind1, ind2, size;
				sscanf(curChar, "%d %d %d", &ind1, &ind2, &size);
				p->SetTextureSpace (p->GetVertex (ind1), p->GetVertex (ind2), size);
			}
		}
		else if(!strcmp(command, "OBJ"))
		{//Read 3D object
			char name[80];
			char obj[80];
			csVector3 location;
			float scale;

			sscanf(buf, "%s %s %s %f %f %f %f", command, obj, name, &location.x, &location.y, &location.z, &scale);
			location += pos;
			AddMeshObj (obj, name, room, location, scale);

		}
		else if(!strcmp(command, "LIGHT"))
		{//Read 3D model
			csVector3 loc, col;
			float radius;
			char type[80];

			sscanf(buf, "%s %s %f %f %f %f %f %f %f", command, type, &loc.x, &loc.y, &loc.z, &col.x, &col.y, &col.z, &radius);
			loc += pos;

			light_list = room->GetLights();
			light = engine -> CreateLight (NULL, csVector3(loc.x, loc.y, loc.z), radius, csColor(col.x, col.y, col.z), false);
			light_list->Add (light->QueryLight());
			light->DecRef();
		}
		else if(!strcmp(command, "MOD"))
		{//Read 3D model
			char name[80], file[100], text[80];
			sscanf(buf, "%s %s %s %s", command, name, file, text);
		}
		else
		{//Read 3D model
			//unkown token
		}
		command[0] = '\0';
		fgets(buf, bufSize, fp);
		sscanf(buf, "%s", command);
	}


	//room->SetAmbientColor(50,50,50);

	//Prepare the whole room.
	//room->Prepare (room);
	//room->InitLightMaps (false);
	//room->ShineLights ();
	//room->CreateLightMaps (System->G3D);

	fclose(fp);
	return true;

}



//*******************************************************************************
//*
//* Build dynamic room as a function of number of objects in the room
//*
//*******************************************************************************
bool ChimeSector::BuildDynamicRoom2(char *roomDesc, const csVector3 &pos, iCollideSystem* collide_system)
{
	if(!engine) return false;


	int bufSize = 100;
	int count;
	char *buf = roomDesc;
	char line[500];
	char roomURL[MAX_URL];

	iSector *room;
	iLightList *light_list;

	csVector3 size(10, 5, 10);
	csVector3 objStartPos(0,0,0);
	csVector3 objPos(0,0,0);


	int nObj = 0;
	int numObjects = 0;
	bool switched = false;

	//Make \n characters to endline character.
	escapeEndlines(roomDesc);

	count = getStrLine(buf, line);
	sscanf(line, "%s %f %f %f %d", roomURL, &roomSize.x, &roomSize.y, &roomSize.z, &numObjects);
	buf += count;

	strcpy(roomUrl, roomURL);
	roomList[numRooms] = engine->CreateSector (roomURL);
	room = roomList[numRooms];
	csVector3 curPos;

	curPos = pos;
	curPos.z += 1;

	connSize.x = 2;
	connSize.y = 3;
	connSize.z = 2;
	connector1 = engine->CreateSector ("connector1");
	BuildStandardConnector(connector1, connSize, curPos, CONN1);

	roomOrigin.x = curPos.x - (size.x/2);
	roomOrigin.y = 0;
	roomOrigin.z = curPos.z + 1;

	curPos.z += 1+(roomSize.z/2);
	BuildStandardRoom(room, roomSize, curPos);

	//setup lighting for the room
	iStatLight* light;
	light_list = room->GetLights();
	light = engine -> CreateLight (NULL, csVector3(0+curPos.x, 4.9+curPos.y, 0+curPos.z), 20, csColor(1, 1, 1), false);
	light_list->Add (light->QueryLight());
	light->DecRef();
	
	curPos.z += roomSize.z/2 + 1;

	connector2 = engine->CreateSector ("connector2");
	BuildStandardConnector(connector2, connSize, curPos, CONN2);
	curPos.z += 1+3;
	hallwaySize.x = 42;
	hallwaySize.y = 4;
	hallwaySize.z = 6;

	hallway = engine->CreateSector ("hallway");
	BuildHallway(hallway, hallwaySize, curPos);

	//Set default camera location
	camLocation = pos;
	camLocation.y = 2;
	camLocation.z += 2;

	firstDoorLocation = curPos;
	firstDoorLocation.z += 3;
	firstDoorLocation.x -= 18;


	objPos.x = roomOrigin.x + 2;
	objPos.y = 0;
	objPos.z = roomOrigin.z + 1;

	for(int i = 0; i < numObjects; i++)
	{//Read 3D object
		bool proceed = true;
		char objUrl[MAX_URL];
		char shape[80];
		char Class[80];
		char subClass[80];
		int defaultLoc;

		csVector3 location;

		if(nObj >= numObjects/2 && !switched)
		{
			objPos.x = roomOrigin.x + roomSize.x - 2;
			objPos.y = 0;
			objPos.z = roomOrigin.z - 1;
			switched = true;
		}
		nObj++;

		objPos.z += 2;

		count = getStrLine(buf, line);
		//sscanf(line, "%s %s %s %s %d", objUrl, shape, Class, subClass, &defaultLoc);
		sscanf(line, "%s %s %s %s %d %f %f %f", objUrl, shape, Class, subClass, &defaultLoc, &location.x, &location.y, &location.z);
		if( !defaultLoc )
		{
			//sscanf(line, "%s %s %s %s %d %f %f %f", objUrl, shape, Class, subClass, &defaultLoc, &location.x, &location.y, &location.z);
			location += roomOrigin;
		}
		else
		{
			location = objPos;
		}

		if (!strcmp(Class, "Container")) {
			AddContainer(objUrl);	//add the container to the list of containers
		}

		else if (!strcmp(Class, "User"))
		{
			proceed = AddUser(objUrl, subClass);  //add the user to the list of users - subClass is the IP for users
			strcat(objUrl, " ");
			strcat(objUrl, subClass);
		}

		else if(!strcmp(Class, "Connector"))
		{
			char *tmp = new char[strlen(objUrl)+1];
			strcpy(tmp, objUrl);
			connList.Push(tmp);
		}

		if (proceed) {

			buf += count;
			location.y = 1;
			AddMeshObj (shape, objUrl, room,location, 1);
		}
	}


	/*	
	room->SetAmbientColor(50,50,50);
	connector1->SetAmbientColor(50,50,50);
	connector2->SetAmbientColor(50,50,50);
	hallway->SetAmbientColor(50,50,50);
	*/
	
	//Prepare the whole room.
	//room->Prepare (room);
	//room->InitLightMaps (false);
	room->ShineLights ();
	//room->CreateLightMaps (System->G3D);

	//connector1->Prepare (connector1);
	//connector1->InitLightMaps (false);
	connector1->ShineLights ();
	//connector1->CreateLightMaps (System->G3D);

	//connector2->Prepare (connector2);
	//connector2->InitLightMaps (false);
	connector2->ShineLights ();
	//connector2->CreateLightMaps (System->G3D);

	//hallway->Prepare (hallway);
	//hallway->InitLightMaps (false);
	hallway->ShineLights ();
	//hallway->CreateLightMaps (System->G3D);


	//Connect room and connector1 room
	roomBackDoor[0]->CreatePortal(connector1);
	roomBackDoor[0]->SetAlpha(0);
	conn1FrontDoor[0]->CreatePortal(room);
	conn1FrontDoor[0]->SetAlpha(0);


	//Connect room and connector2 room
	roomFrontDoor[0]->CreatePortal(connector2);
	roomFrontDoor[0]->SetAlpha(0);
	conn2BackDoor[0]->CreatePortal(room);
	conn2BackDoor[0]->SetAlpha(0);

	//Connect hallway and connector2
	conn2FrontDoor[0]->CreatePortal(hallway);
	conn2FrontDoor[0]->SetAlpha(0);
	hallBackDoor[0]->CreatePortal(connector2);
	hallBackDoor[0]->SetAlpha(0);

	//Prepare room for collision detection
	iPolygonMesh* mesh;
	iObject *obj;
	csColliderWrapper *collider;

	//mesh = SCF_QUERY_INTERFACE (walls->GetMeshObject(), iPolygonMesh);
	//obj = SCF_QUERY_INTERFACE(walls, iObject);
    //collider = new csColliderWrapper (obj, collide_system, mesh);
	//mesh->DecRef ();
	//obj->DecRef();

/*
	mesh = SCF_QUERY_INTERFACE (connector1, iPolygonMesh);
    collider = new csColliderWrapper (*connector1, collide_system, mesh);
    mesh->DecRef ();

	mesh = SCF_QUERY_INTERFACE (connector2, iPolygonMesh);
    collider = new csColliderWrapper (*connector2, collide_system, mesh);
    mesh->DecRef ();

	mesh = SCF_QUERY_INTERFACE (hallway, iPolygonMesh);
    collider = new csColliderWrapper (*hallway, collide_system, mesh);
    mesh->DecRef ();
*/	

	//Add collision detection to all the objects in the room
	for ( i = 0 ; i < room->GetMeshes()->GetCount(); i++)
	{
		iMeshWrapper* sp = room->GetMeshes()->Get(i);
		iMeshObject *s = sp->GetMeshObject();
		mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			collider = new csColliderWrapper (sp->QueryObject(), collide_system, mesh);
			mesh->DecRef ();
		}
	}

		//Add collision detection to all the objects in the room
	for ( i = 0 ; i < hallway->GetMeshes()->GetCount(); i++)
	{
		iMeshWrapper* sp = hallway->GetMeshes()->Get(i);
		iMeshObject *s = sp->GetMeshObject();
		mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			collider = new csColliderWrapper (sp->QueryObject(), collide_system, mesh);
			mesh->DecRef ();
		}
	}

		//Add collision detection to all the objects in the room
	for ( i = 0 ; i < connector1->GetMeshes()->GetCount(); i++)
	{
		iMeshWrapper* sp = connector1->GetMeshes()->Get(i);
		iMeshObject *s = sp->GetMeshObject();
		mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			collider = new csColliderWrapper (sp->QueryObject(), collide_system, mesh);
			mesh->DecRef ();
		}
	}

		//Add collision detection to all the objects in the room
	for ( i = 0 ; i < connector2->GetMeshes()->GetCount(); i++)
	{
		iMeshWrapper* sp = connector2->GetMeshes()->Get(i);
		iMeshObject *s = sp->GetMeshObject();
		mesh = SCF_QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			collider = new csColliderWrapper (sp->QueryObject(), collide_system, mesh);
			mesh->DecRef ();
		}
	}
	collider -> DecRef();

	int numActiveDoors = __min(connList.Length(), 10);
	for ( i = 0 ; i < numActiveDoors; i++)
	{
		hallFrontDoor[i]->SetAlpha(ACTIVE_DOOR_ALPHA);
	}
	for ( i = numActiveDoors ; i < 10; i++)
	{
		hallFrontDoor[i]->SetAlpha(INACTIVE_DOOR_ALPHA);
	}

	return true;

}


//*******************************************************************************
//*
//* Add Collision Detection to a room
//*
//*******************************************************************************
bool ChimeSector::AddCollisionDetection(iSector *room, iCollideSystem* collide_system) {
	iPolygonMesh* mesh;
	mesh = SCF_QUERY_INTERFACE (room, iPolygonMesh);
	/*
    (void)new csCollider (*room, collide_system, mesh);
	*/
    mesh->DecRef ();
	return true;
}

//*******************************************************************************
//*
//* Get the ith room
//*
//*******************************************************************************
iSector* ChimeSector::GetRoom(int index)
{
	return roomList[index];
}

//*********************************************************************************
//*
//* Add mesh object in a given room at a given location.
//*
//*********************************************************************************
iMeshWrapper* ChimeSector::AddMeshObj (char* tname, char* sname, iSector* where,
	csVector3 const& pos, float size)
{

 iMeshFactoryWrapper* tmpl = engine->GetMeshFactories ()->FindByName (tname);
  if (!tmpl)
  {
    Sys->Report (0,
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
//* Build a standard room of a given size at given location
//*
//*********************************************************************************
bool ChimeSector::BuildStandardRoom(iSector *room, csVector3 const &size, csVector3 const &pos)
{
	iMaterialWrapper* txt;
	csVector3 trans(0,0,0);

	iMeshWrapper *wallmesh = engine -> CreateSectorWallsMesh(room, "walls");
	iThingState *walls = SCF_QUERY_INTERFACE(wallmesh->GetMeshObject(), iThingState);

	//Load standard texure for walls
	srand( (unsigned)time( NULL ) );
	int dice = rand() % 3;

	if( dice == 0 )
		txt = engine->GetMaterialList()->FindByName ("brownfabric");
	else if( dice == 1)
		txt = engine->GetMaterialList()->FindByName ("glasswall");
	else
		txt = engine->GetMaterialList()->FindByName ("funkywall");


	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	buildFrontDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 4,roomFrontDoor);
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));

	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	
	//BuildWall(room, size, trans, BACK, txt, csVector3(3,3,3));
	buildBackDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 4,roomBackDoor);

	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(walls, "left_wall",  size, trans, LEFT, txt, csVector3(3,3,3));

	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(walls, "right_wall", size, trans, RIGHT, txt, csVector3(3,3,3));

	//Build Floor wall of the room
	//Load sandard texure for walls
	dice = rand() % 3;
	if( dice == 0 )
		txt = engine->GetMaterialList()->FindByName ("woodfloor");
	else if( dice == 1)
		txt = engine->GetMaterialList()->FindByName ("tilefloor");
	else
		txt = engine->GetMaterialList()->FindByName ("checkerfloor");

	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(walls, "floor", size, trans, FLOOR, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	txt = engine->GetMaterialList()->FindByName ("marble1");
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(walls, "ceiling", size, trans, ROOF, txt, csVector3(3,3,3));

	walls->DecRef();
	return true;
}

//*********************************************************************************
//*
//* Build a standard connector that connects a room with a hallway
//*
//*********************************************************************************
bool ChimeSector::BuildStandardConnector(iSector *room, csVector3 const &size, csVector3 const &pos, int type)
{
	iMaterialWrapper* txt;
	csVector3 trans(0,0,0);
	iPolygon3D **connFrontDoor, **connBackDoor;
	
	iMeshWrapper *wallmesh = engine -> CreateSectorWallsMesh(room, "walls");
	iThingState *walls = SCF_QUERY_INTERFACE(wallmesh->GetMeshObject(), iThingState);

	if(type == CONN1)
	{
		connFrontDoor = conn1FrontDoor;
		connBackDoor = conn1BackDoor;
	}
	else
	{
		connFrontDoor = conn2FrontDoor;
		connBackDoor = conn2BackDoor;
	}

	//Load sandard texure for walls
	txt = engine->GetMaterialList()->FindByName ("whitemarble");

	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));
	buildFrontDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 0, connFrontDoor);


	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	//BuildWall(room, size, trans, BACK, txt, csVector3(3,3,3));
	buildBackDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 0, connBackDoor);
	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(walls, "left wall", size, trans, LEFT, txt, csVector3(3,3,3));

	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(walls, "right_wall", size, trans, RIGHT, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(walls, "ceiling", size, trans, ROOF, txt, csVector3(3,3,3));

	//Build Floor wall of the room
	txt = engine->GetMaterialList()->FindByName ("metalfloor");
	//txt = engine->GetMaterialList()->FindByName ("water");
	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(walls, "floor", size, trans, FLOOR, txt, csVector3(2,2,1));

	walls->DecRef();

	return true;
}
//*********************************************************************************
//*
//* Build a standard hallway of a given size at given location
//*
//*********************************************************************************
bool ChimeSector::BuildHallway(iSector *room, csVector3 const &size, csVector3 const &pos)
{
	iMaterialWrapper* txt;
	csVector3 trans(0,0,0);
	float doorOff = (size.x - 2)/2;
	//Load sandard texure for walls
	txt = engine->GetMaterialList()->FindByName ("marble1");
	
	iMeshWrapper *wallmesh = engine -> CreateSectorWallsMesh(room, "walls");
	iThingState *walls = SCF_QUERY_INTERFACE(wallmesh->GetMeshObject(), iThingState);
	
	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	buildFrontDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 10, 2, hallFrontDoor);
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));
	
	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	buildBackDoorWall(walls, room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, doorOff, hallBackDoor);

	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	///buildLeftDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(0,3,2), 5, 2, hallLeftDoor);
	BuildWall(walls, "left_wall", size, trans, LEFT, txt, csVector3(3,3,3));

	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(walls, "right_wall", size, trans, RIGHT, txt, csVector3(3,3,3));
	//buildRightDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(0,3,2), 5, 2, hallRightDoor);

	//Build Floor wall of the room
	//Load sandard texure for walls
	txt = engine->GetMaterialList()->FindByName ("hallfloor");
	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(walls, "floor", size, trans, FLOOR, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	txt = engine->GetMaterialList()->FindByName ("marble1");
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(walls, "roof", size, trans, ROOF, txt, csVector3(3,3,3));

	walls->DecRef();

	return true;
}


//*********************************************************************************
//*
//* Build a given wall of a room
//*
//*********************************************************************************
iPolygon3D * ChimeSector::BuildWall(iThingState *walls, char *wall_name, csVector3 const &size, csVector3 const &pos, 
									 int type, iMaterialWrapper *txt, csVector3 const &txtSize)
{

	//iMaterialWrapper *iMW = SCF_QUERY_INTERFACE(txt, iMaterialWrapper);

	csVector3 v1;
	csVector3 v2;
	csVector3 v3;
	csVector3 v4;

	v1 = stdVector[type+0];
	v2 = stdVector[type+1];
	v3 = stdVector[type+2];
	v4 = stdVector[type+3];

	iPolygon3D *p = NULL;

	v1.x = v1.x * size.x; v1.y = v1.y * size.y; v1.z = v1.z * size.z;
	v2.x = v2.x * size.x; v2.y = v2.y * size.y; v2.z = v2.z * size.z;
	v3.x = v3.x * size.x; v3.y = v3.y * size.y; v3.z = v3.z * size.z;
	v4.x = v4.x * size.x; v4.y = v4.y * size.y; v4.z = v4.z * size.z;

	v1 += pos;
	v2 += pos;
	v3 += pos;
	v4 += pos;

	p = walls->CreatePolygon (wall_name);
	p->SetMaterial(txt);
	p->CreateVertex (csVector3(v1.x, v1.y, v1.z));
	p->CreateVertex (csVector3(v2.x, v2.y, v2.z));
	p->CreateVertex (csVector3(v3.x, v3.y, v3.z));
	p->CreateVertex (csVector3(v4.x, v4.y, v4.z));
	//p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), txtSize.x);

	if(type == RIGHT || type == BACK)
	{
		p->SetTextureSpace (p->GetVertex (2), p->GetVertex (3), txtSize.x, p->GetVertex (1), txtSize.y);
	}
	else
	{
		p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), txtSize.x, p->GetVertex (3), txtSize.y);
	}

	//iMW->DecRef();
	return p;
}

bool ChimeSector::CreateLabel(iThingState *walls, char *label_name, csVector3 const &size, csVector3 const &pos, 
									 int type, iMaterialWrapper *txt, csVector3 const &txtSize)
{
	csVector3 v1, v2, v3, v4, new_pos;

	v1 = stdVector[type+0];
	v2 = stdVector[type+1];
	v3 = stdVector[type+2];
	v4 = stdVector[type+3];

	iPolygon3D *p = NULL;

	v1.x = v1.x * size.x * 2; v1.y = (v1.y * size.y) ; v1.z = v1.z * size.z;  //divide y/3
	v2.x = v2.x * size.x * 2; v2.y = (v2.y * size.y) ; v2.z = v2.z * size.z;
	v3.x = v3.x * size.x * 2; v3.y = (v3.y * size.y) ; v3.z = v3.z * size.z;
	v4.x = v4.x * size.x * 2; v4.y = (v4.y * size.y) ; v4.z = v4.z * size.z;

	new_pos.x = pos.x - (size.x / 2);
	new_pos.y = size.y + pos.y;
	new_pos.z = pos.z;

	v1 += new_pos;
	v2 += new_pos;
	v3 += new_pos;
	v4 += new_pos;

	//get all the materials
	iMaterialWrapper *pt_engine = engine->GetMaterialList()->FindByName ("pt_engine");
	iMaterialWrapper *label = engine->GetMaterialList()->FindByName ("woodfloor");
	iMaterialWrapper *iMW = SCF_QUERY_INTERFACE(label, iMaterialWrapper);

	p = walls->CreatePolygon (label_name);
	if (pt_engine)
		p->SetMaterial (pt_engine);

	p->CreateVertex (csVector3(v1.x, v1.y, v1.z));
	p->CreateVertex (csVector3(v2.x, v2.y, v2.z));
	p->CreateVertex (csVector3(v3.x, v3.y, v3.z));
	p->CreateVertex (csVector3(v4.x, v4.y, v4.z));

	

	if(type == RIGHT || type == BACK)
	{
		p->SetTextureSpace (p->GetVertex (2), p->GetVertex (3), txtSize.x * 2, p->GetVertex (1), txtSize.y);
	}
	else
	{
		p->SetTextureSpace (p->GetVertex (0), p->GetVertex (1), txtSize.x * 2, p->GetVertex (3), txtSize.y);
	}

	return true;
}


//Build right wall with doors
bool ChimeSector::buildRightDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, 
									 iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;
	iMeshWrapper *doormesh = engine->CreateSectorWallsMesh(room, "right_door_wall");
	iThingState *doors = SCF_QUERY_INTERFACE(doormesh->GetMeshObject(), iThingState);

	doorTxtSize.x = doorSize.z;
	doorTxtSize.y = doorSize.y;
	doorTxtSize.z = 0;				//Its ignored by BuildWall() anyway.

	curPos = pos;
	size1.x = size.x;
	size1.y = doorSize.y;
	size1.z = offset;

	size2.x = size.x;
	size2.y = size.y - doorSize.y;
	size2.z = size.z;

	curPos = pos;
	curPos.y += doorSize.y;
	int door_num = 0;

	//added this
	iMaterialWrapper *t = engine->GetMaterialList()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildDoor(doors, &door_num, size2, curPos, RIGHT, txt, txtSize);
	
	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.z > 0)
			BuildDoor(doors, &door_num, size1, curPos, RIGHT, txt, txtSize);
		
		curPos.z += offset;

		pList[i] = BuildDoor(doors, &door_num, doorSize, curPos, RIGHT, t, doorTxtSize);
		curPos.z += doorSize.z;

	}
	size1.z = (pos.z + size.z - curPos.z);
	if(size1.z > 0)
		BuildDoor(doors, &door_num, size1, curPos, RIGHT, txt, txtSize);
	

	doors -> DecRef();

	return true;
}

//Build left wall with doors
bool ChimeSector::buildLeftDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;
	iMeshWrapper *doormesh = engine->CreateSectorWallsMesh(room, "left_door_wall");
	iThingState *doors = SCF_QUERY_INTERFACE(doormesh->GetMeshObject(), iThingState);
	int door_num = 0;

	doorTxtSize.x = doorSize.z;
	doorTxtSize.y = doorSize.y;
	doorTxtSize.z = 0;				//Its ignored by BuildWall() anyway.

	curPos = pos;
	size1.x = size.x;
	size1.y = doorSize.y;
	size1.z = offset;

	size2.x = size.x;
	size2.y = size.y - doorSize.y;
	size2.z = size.z;

	curPos = pos;
	curPos.y += doorSize.y;

	iMaterialWrapper *t = engine->GetMaterialList()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildDoor(doors, &door_num, size2, curPos, LEFT, txt, txtSize);
	
	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.z > 0)
			BuildDoor(doors, &door_num, size1, curPos, LEFT, txt, txtSize);
		
		curPos.z += offset;
		
		pList[i] = BuildDoor(doors, &door_num, doorSize, curPos, LEFT, t, doorTxtSize);
		curPos.z += doorSize.z;
	}
	size1.z = (pos.z + size.z - curPos.z);
	if(size1.z > 0)
		BuildDoor(doors, &door_num, size1, curPos, LEFT, txt, txtSize);
	

	doors->DecRef();
	return true;
}


//Build front wall with doors
bool ChimeSector::buildFrontDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[])
{

	int door_num = 0;
	csVector3 size1, size2, curPos, doorTxtSize, label_size;

	doorTxtSize.x = doorSize.x;
	doorTxtSize.y = doorSize.y;
	doorTxtSize.z = 0;				//Its ignored by BuildWall() anyway.

	label_size.x = doorSize.x;
	label_size.y = doorSize.y;
	label_size.z = doorSize.z;

	curPos = pos;
	size1.x = offset;
	size1.y = doorSize.y;
	size1.z = size.z;

	size2.x = size.x;
	size2.y = size.y - doorSize.y;
	size2.z = size.z;

	curPos = pos;
	curPos.y += doorSize.y;

	iMaterialWrapper *t = engine->GetMaterialList()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildDoor(walls, &door_num, size2, curPos, FRONT, txt, txtSize);

	curPos = pos;

	for(int i = 0; i < numDoors; i++)
	{
		if(size1.x > 0)
			BuildDoor(walls, &door_num, size1, curPos, FRONT, txt, txtSize);
		

		curPos.x += offset;

		pList[i] = BuildDoor(walls, &door_num, doorSize, curPos, FRONT, t, doorTxtSize);

		//create a label on the wall above the door
		//CreateLabel(room, label_size, curPos, FRONT, csVector3(3,3,3));


		curPos.x += doorSize.x;
	}
	size1.x = (pos.x + size.x - curPos.x);
	if(size1.x > 0) 
		BuildDoor(walls, &door_num, size1, curPos, FRONT, txt, txtSize);

	
	return true;
}

//convenience method to make the nth door on some wall
iPolygon3D* ChimeSector::BuildDoor(iThingState *walls, int *door_num, csVector3 const &size, csVector3 const &pos, 									 
							int type, iMaterialWrapper *txt, csVector3 const &txtSize)
{
	char door_name[1024];
	sprintf(door_name, "door_%d", *door_num);
	*door_num = *door_num + 1;
	return BuildWall(walls, door_name, size, pos, type, txt, txtSize);
}


//Build front wall with doors
bool ChimeSector::buildBackDoorWall(iThingState *walls, iSector *room, csVector3 const &size, csVector3 const &pos, iMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, iPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize, label_size;
	int door_num = 0;


	doorTxtSize.x = doorSize.x;
	doorTxtSize.y = doorSize.y;
	doorTxtSize.z = 0;				//Its ignored by BuildWall() anyway.


	curPos = pos;
	size1.x = offset;
	size1.y = doorSize.y;
	size1.z = size.z;

	size2.x = size.x;
	size2.y = size.y - doorSize.y;
	size2.z = size.z;

	curPos = pos;
	curPos.y += doorSize.y;

	iMaterialWrapper *t = engine->GetMaterialList()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildDoor(walls, &door_num, size2, curPos, BACK, txt, txtSize);
	

	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.x > 0)
			BuildDoor(walls, &door_num, size1, curPos, BACK, txt, txtSize);

		curPos.x += offset;
		pList[i] = BuildDoor(walls, &door_num, doorSize, curPos, BACK, t, doorTxtSize);

		curPos.x += doorSize.x;
	}
	size1.x = (pos.x + size.x - curPos.x);
	if(size1.x > 0)
		BuildDoor(walls, &door_num, size1, curPos, BACK, txt, txtSize);

	return true;
}


//*********************************************************************************
//*
//* Return Default camera location of this sector
//*
//*********************************************************************************
csVector3* ChimeSector::GetCamLocation()
{
	return &camLocation;
}

//*********************************************************************************
//*
//* Return location and hallway of a given hallway-door of this chime sector
//*
//*********************************************************************************
bool ChimeSector::GetDoorInfo(int doorNum, csVector3 &location, iPolygon3D* &door, iSector* &h)
{
	location = firstDoorLocation;
	location.x += 4*doorNum;
	door = hallFrontDoor[doorNum];
	h = hallway;
	return true;
}

//Set linked chime sector info
bool ChimeSector::SetLinkedSectorInfo(ChimeSector *sect, int doorNum)
{
	linkedSector = sect;
	linkedDoor = doorNum;
	return true;
}

//Get given halway door of this chime sector.
iPolygon3D*  ChimeSector::GetHallwayDoor(int doorNum)
{
	if( doorNum >= 0 && doorNum < 10)
	{
		return hallFrontDoor[doorNum];
	}
	else
	{
		return NULL;
	}
}

//Get BackDoor of this chime sector.
iPolygon3D* ChimeSector::GetBackDoor()
{
	return conn1BackDoor[0];
}

//Get spatial location of a given hallway door.
bool ChimeSector::GetHallwayDoorLoc(int doorNum, csVector3 & location)
{
	location = firstDoorLocation;
	location.x += 4*doorNum;
	return true;
}

bool ChimeSector::Connect(iPolygon3D *door, iSector *hallway)
{
	door->CreatePortal (connector1);
	door->SetAlpha(0);
	conn1BackDoor[0]->CreatePortal (hallway);
	conn1BackDoor[0]->SetAlpha(0);
	return true;
}

//find the type of the link
int ChimeSector::findType(const char *thing) 
{
	for(int i = 0; i < containerList.Length(); i++)
	{
		if(!strcmp(containerList.Get(i), thing))
		{
			return CONTAINER;
		}
	}

	for (i = 0; i < connList.Length(); i++)
	{
		if(!strcmp(connList.Get(i), thing))
			{
				return CONNECTOR;
			}
	}

	for (i = 0; i < userList.Length(); i++)
	{
		if(!strcmp(userList.Get(i), thing))
			{
				return USER;
			}
	}

	return -1;
}


//Add user name to the list of users in the sector
bool ChimeSector::AddUser(char *username, char *ip_address)
{
	char my_username[50];

	((ChimeSystemDriver *)System)->GetApp()->GetInfo()->GetUsername(my_username);

	//don't do anything if the user is already there
	for (int i = 0; i < userList.Length(); i++) {
		char in_username[100];
		char in_ip_address[100];
		sscanf(userList.Get(i), "%s %s", in_username, in_ip_address);
		if (strcmp(username, in_username) == 0 || //already on the list
			strcmp(username, my_username) == 0) //me
			return false;
	}


	UserIDType userID = MakeUserID(username, ip_address);

	if(!userID) return false;

	if (System && ((ChimeSystemDriver*)System)->GetApp() && 
			 ((ChimeSystemDriver*)System)->GetApp()->chatWindow) {
		 ((ChimeSystemDriver*)System)->GetApp()->chatWindow->AddLocalUser(username, ip_address);
	}

	userList.Push(userID);
	return true;
}

//Delete user name from the list of users in the sector. Comparison based on username only
//CAREFUL - This is not the same as UserID
bool ChimeSector::deleteUser(char *username)
{
	if(!username) return false;

	if (System && ((ChimeSystemDriver*)System)->GetApp() 
			&&  ((ChimeSystemDriver*)System)->GetApp()->chatWindow) {
		 ((ChimeSystemDriver*)System)->GetApp()->chatWindow->DeleteLocalUser(username);
	}

	for(int i = 0; i < userList.Length(); i++)
	{
		char cur_username[100];
		char *temp = userList.Get(i);
		sscanf(temp, "%s ", cur_username);
		if(!strcmp(cur_username, username))
		{
			userList.Delete(i);
			return true;
		}
	}
	return false;
}


//Delete user name from the list of users in the sector. Comparison based on username and ip_address
bool ChimeSector::deleteUser(char *username, char *ip_address)
{
	UserIDType userID = MakeUserID(username, ip_address);
	
	if(!userID) return false;

	if (System && ((ChimeSystemDriver*)System)->GetApp() 
			&&  ((ChimeSystemDriver*)System)->GetApp()->chatWindow) {
		 ((ChimeSystemDriver*)System)->GetApp()->chatWindow->DeleteLocalUser(username, ip_address);
	}

	for(int i = 0; i < userList.Length(); i++)
	{
		if(!strcmp(userID, userList.Get(i)))
		{
			userList.Delete(i);
			return true;
		}
	}
	return false;
}


//make the user ID out of a username and ip_address
char* ChimeSector::MakeUserID(const char *username, const char* ip_address) {
	if (!username) return NULL;
	if (!ip_address) return NULL;

	char* userID = new char[strlen(username) + strlen(ip_address) + 2];
	strcpy(userID, username);
	strcat(userID, " ");
	strcat(userID, ip_address);
	return userID;
}



//Add a container to a list of containers
bool ChimeSector::AddContainer(char *location)
{
	if(!location) return false;

	char *tmp = new char[strlen(location)+1];
	strcpy(tmp, location);
	containerList.Push(tmp);
	return true;
}


//Remove a container from the list of containers
bool ChimeSector::DeleteContainer(char *location) 
{
	if(!location) return false;

	char *loc = NULL;
	for(int i = 0; i < containerList.Length(); i++)
	{
		if(!strcmp(containerList.Get(i), location))
		{
			containerList.Delete(i);
			return true;
		}
	}
	return false;
}

//connect "otherSect" to the "atDoor" hallway door of this sector.
bool ChimeSector::ConnectSectors(ChimeSector *otherSect, int atDoor)
{
	if( !otherSect ) return false;

	iPolygon3D *door = hallFrontDoor[atDoor];
	iPolygon3D *otherSectorBackDoor = otherSect->GetBackDoor();

	door->CreatePortal (otherSect->GetConn1());
	door->SetAlpha(0);
	otherSectorBackDoor->CreatePortal (hallway);
	otherSectorBackDoor->SetAlpha(0);
	doorSec[atDoor] = otherSect;

	otherSect->SetLinkedSectorInfo(this, atDoor);

	return true;
}

//diconnect this sector from the linked sector
bool ChimeSector::DisconnectSector()
{
	if(!linkedSector ) return false;

	iPolygon3D *linkedSectorDoor = linkedSector->GetHallwayDoor(linkedDoor);

	linkedSectorDoor->CreatePortal (NULL);
	linkedSectorDoor->SetAlpha(ACTIVE_DOOR_ALPHA);
	conn1BackDoor[0]->CreatePortal (NULL);
	conn1BackDoor[0]->SetAlpha(ACTIVE_DOOR_ALPHA);

	linkedSector->SetDoorSector(linkedDoor, NULL);
	linkedSector = NULL;
	linkedDoor = -1;

	return true;
}

bool ChimeSector::Disconnect()
{
	conn1BackDoor[0]->CreatePortal (NULL);
	conn1BackDoor[0]->SetAlpha(INACTIVE_DOOR_ALPHA);
	return true;
}

bool ChimeSector::UnlinkHallwayDoors()
{
	ChimeSector *sec;

	for(int i = 0; i < 10; i++)
	{
		sec = doorSec[i];
		if( sec )
		{
			sec->Disconnect();
		}
	}
	return true;
}

bool ChimeSector::SetDoorSector(int doorNum, ChimeSector *sec)
{
	if( doorNum >= 0 && doorNum < MAX_DOOR)
	{
		doorSec[doorNum] = sec;
		return true;
	}
	else
	{
		return false;
	}
}

ChimeSector* ChimeSector::GetDoorSector(int doorNum)
{
	if( doorNum >= 0 && doorNum < MAX_DOOR)
	{
		return doorSec[doorNum];
	}
	else
	{
		return NULL;
	}
}

//get the door url associated with this doornum
char* ChimeSector::GetDoorUrl(int doorNum)
{
	int numActive = connList.Length();

	if( doorNum >= 0 && doorNum < numActive)
	{
		return connList.Get(doorNum);
	}
	else
	{
		return NULL;
	}
}

//replace the door url with the one provided as an argument
bool ChimeSector::ReplaceDoorUrl(int doorNum, char *doorUrl) {
	
	if (!doorUrl) 
		return false;

	char *tmp = new char[strlen(doorUrl)+1];
	strcpy(tmp, doorUrl);

	if( doorNum >= 0 && doorNum < connList.Length()) {
		connList.Replace(doorNum, tmp);
		return true;
	
	} else {
		return false;
	}
}

// Find  an object in this sector
iMeshWrapper* ChimeSector::FindObject(char *objectUrl, iSector *&room)
{
	room = NULL;
	int i;

	room = roomList[0];
	iMeshWrapper* obj = NULL;
	iMeshList *mesh_list = room->GetMeshes();

	if (!mesh_list)
		goto out;

	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);

		if( !strcmp(objectUrl, mesh->QueryObject()->GetName()) )
		{
			obj = mesh;
			goto out;
		}
	}
	room = hallway;
	mesh_list = room->GetMeshes();
	obj = NULL;
	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);

		if( !strcmp(objectUrl, mesh->QueryObject()->GetName()) )
		{
			obj = mesh;
			goto out;
		}
	}
	room = connector1;
	mesh_list = room->GetMeshes();
	obj = NULL;
	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);
		if( !strcmp(objectUrl, mesh->QueryObject()->GetName()) )
		{
			obj = mesh;
			goto out;
		}
	}
	room = connector2;
	mesh_list = room->GetMeshes();
	obj = NULL;
	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);

		if( !strcmp(objectUrl, mesh->QueryObject()->GetName()) )
		{
			obj = mesh;
			goto out;
		}
	}


out:
	mesh_list->DecRef();
	return obj;
}

//check if a given beam hits any of the meshes in this sector.
iMeshWrapper* ChimeSector::SelectMesh (const csVector3 &start, const csVector3 &end, csVector3 &isect, float &dist)
{
	int i;
	iSector *room;
	iMeshWrapper *mesh = NULL;

	//check for mesh in  the main room
	room = roomList[0];
	iMeshList *mesh_list = room->GetMeshes();


	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		mesh = mesh_list->Get(i);
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			goto out;
		}
	}

	mesh = NULL;

	//check for mesh in  the hallway
	room = hallway;
	mesh_list = room->GetMeshes();

	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			goto out;
		}
	}

	//check for mesh in  the connector2
	mesh = NULL;
	room = connector2;
	mesh_list = room->GetMeshes();

	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			goto out;
		}
	}

	//check for mesh in  the connector1
	mesh = NULL;
	room = connector1;
	mesh_list = room->GetMeshes();

	for (i = 0 ; i < mesh_list->GetCount(); i++)
	{
		iMeshWrapper* mesh = mesh_list->Get(i);
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			goto out;
		}
	}


out:
	//mesh_list->DecRef();
	return mesh;
}

//Find the room of a sector that contains this point
//Note: heuristics and shape of the sector are being used for the test
iSector* ChimeSector::FindRoomContainingPoint(csVector3 p)
{
	float start = roomOrigin.z - connSize.z;
	float end = roomOrigin.z;

	if(p.z >= start && p.z < end)
	{
		return connector1;
	}

	start = end;
	end += roomSize.z;
	if(p.z >= start && p.z < end)
	{
		return roomList[0];
	}

	start = end;
	end += connSize.z;
	if(p.z >= start && p.z < end)
	{
		return connector2;
	}
	start = end;
	end += hallwaySize.z;
	if(p.z >= start && p.z < end)
	{
		return hallway;
	}

	return NULL;
}

//*********************************************************************************
//*
//* Check If a given room belongs to this chime sector
//*
//*********************************************************************************
int ChimeSector::IsRoomOf(iSector *room)
{
	int type = UNKNOWN;

	if(room == roomList[0])
		type = ROOM;
	else if(room == connector1)
		type = CONN1;
	else if(room == connector2)
		type = CONN2;
	else if(room == hallway)
		type = HALLWAY;

	return type;
}


//*********************************************************************************
//*
//* Check If a given beam intersects any of the polygons of the ChimeSector
//*
//*********************************************************************************
bool ChimeSector::HitBeam(const csVector3 &start, const csVector3 &end, csVector3 &isect)
{

	iPolygon3D* p = NULL;

	p = roomList[0]->HitBeam (start, end, isect);
	if( p ) return true;
	p = hallway->HitBeam (start, end, isect);
	if( p ) return true;
	p = connector1->HitBeam (start, end, isect);
	if( p ) return true;
	p = connector2->HitBeam (start, end, isect);
	if( p ) return true;

	return false;
}

//*********************************************************************************
//*
//* Check If a given beam intersects any of the active hallway doors of this sector
//*
//*********************************************************************************
int ChimeSector::HallwayHitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect, int &doorNum)
{

	doorNum = -1;
	iPolygon3D* p = NULL;
	p = hallway->HitBeam (start, end, isect);
	int numDoors = 10;


	//Check if intersecting polygon is an active door
	if( p )
	{
		for(int i = 0; i < numDoors; i++)
		{
			if(p == hallFrontDoor[i])
			{
				doorNum = i;
				if (p -> GetAlpha() == ACTIVE_DOOR_ALPHA)
					return ACTIVE_DOOR;
				else 
					return INACTIVE_DOOR;
			}

		}
	}

	return DOOR_NOT_FOUND;
}


