// chimeSector.cpp: implementation of the chimeSector class.
//
//////////////////////////////////////////////////////////////////////
#include "cssysdef.h"
#include "cssys/system.h"
#include "chimeSector.h"

#include "csengine/sector.h"
#include "csengine/engine.h"
#include "csengine/texture.h"
#include "csengine/light.h"
#include "csengine/polygon.h"
#include "icollide.h"
#include "icfgnew.h"
#include "ifontsrv.h"
#include "itxtmgr.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

chimeSector::chimeSector()
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


chimeSector::chimeSector(csSystemDriver  *Sys, csEngine *e)
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

chimeSector::~chimeSector()
{

	//FIXIT: Other rooms need to be freed.

}

int chimeSector::escapeEndlines(char *buf)
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

int chimeSector::getStrLine(const char *buf, char *line)
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
bool chimeSector::InitDoors()
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
bool chimeSector::InitStdVectors()
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

//Dummy room
bool chimeSector::ReadRoom(char *fileName, iCollideSystem* collide_system)
{
	if(!engine) return false;

	FILE *fp = fopen(fileName, "r");
	if(!fp) return false;
	int bufSize = 100;
	char buf[100];
	char command[80];

	csSector *room = NULL;
	csPolygon3D* p;
	csMaterialWrapper* txt;
	csVector3 pos;
	csStatLight* light;

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

			pos.Set(0,0,0);
			sscanf(buf, "%s %s %f %f %f %d", command, name, &pos.x, &pos.y, &pos.z,&numPoly);
			roomList[numRooms] = engine->CreateCsSector (name);
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
				txt = engine->GetMaterials ()->FindByName (str);
				p = room->NewPolygon (txt);

				sscanf(curChar, "%d", &numPoints);
				while(*curChar != ',') curChar++; curChar++;

				for(int j = 0; j < numPoints; j++)
				{
					float x, y, z;

					sscanf(curChar, "%f %f %f", &x, &y, &z);
					while(*curChar != ',') curChar++; curChar++;
					x += pos.x; y += pos.y; z += pos.z;
					p->AddVertex (x, y, z);
				}
				int ind1, ind2, size;
				sscanf(curChar, "%d %d %d", &ind1, &ind2, &size);
				p->SetTextureSpace (p->Vobj (ind1), p->Vobj (ind2), size);
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
			AddMeshObj (obj, name, room,location, scale);

		}
		else if(!strcmp(command, "LIGHT"))
		{//Read 3D model
			csVector3 loc, col;
			float radius;
			char type[80];

			sscanf(buf, "%s %s %f %f %f %f %f %f %f", command, type, &loc.x, &loc.y, &loc.z, &col.x, &col.y, &col.z, &radius);
			loc += pos;
			light = new csStatLight (loc.x, loc.y, loc.z, radius, col.x, col.y, col.z, false);
			room->AddLight (light);
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


	room->SetAmbientColor(50,50,50);

	//Prepare the whole room.
	room->Prepare (room);
	room->InitLightMaps (false);
	room->ShineLights ();
	room->CreateLightMaps (System->G3D);

	fclose(fp);
	return true;

}

//Build dynamic room as a function of number of objects in the room
bool chimeSector::BuildDynamicRoom(char *fileName, const csVector3 &pos, iCollideSystem* collide_system)
{
	if(!engine) return false;

	FILE *fp = fopen(fileName, "r");
	if(!fp) return false;
	int bufSize = 100;
	char buf[100];
	char command[80];

	csSector *room;
	csVector3 objStartPos(0,0,0);
	csVector3 objPos(0,0,0);
	int nObj = 0;
	int numObjects = 0;
	bool switched = false;

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


			sscanf(buf, "%s %s %d", command, name, &numObjects);
			roomList[numRooms] = engine->CreateCsSector (name);
			room = roomList[numRooms];
			//numRooms++;
			float length = __max(10, 2*(numObjects+1));
			csVector3 size(10, 5, length);
			csVector3 curPos;

			curPos = pos;
			curPos.z += 1;
			size.x = 2;
			size.y = 3;
			size.z = 2;
			connector1 = engine->CreateCsSector ("connector1");
			BuildStandardConnector(connector1, size, curPos, CONN1);

			size.x = 10;
			size.y = 5;
			size.z = length;
			objStartPos = curPos;
			objStartPos.x -= 3;
			objPos = objStartPos;

			curPos.z += 1+(size.z/2);
			BuildStandardRoom(room, size, curPos);
			csStatLight* light;
			light = new csStatLight (0+curPos.x, 5+curPos.y, 0+curPos.z, 20, 1, 1, 1, false);
			room->AddLight (light);

			curPos.z += size.z/2 + 1;
			size.x = 2;
			size.y = 3;
			size.z = 2;

			connector2 = engine->CreateCsSector ("connector2");
			BuildStandardConnector(connector2, size, curPos, CONN2);
			curPos.z += 1+3;
			size.x = 42;
			size.y = 4;
			size.z = 6;
			hallway = engine->CreateCsSector ("hallway");
			BuildHallway(hallway, size, curPos);

			//Set default camera location
			camLocation = pos;
			camLocation.y = 2;
			camLocation.z += 2;

			firstDoorLocation = curPos;
			firstDoorLocation.z += 3;
			firstDoorLocation.x -= 18;

		}
		else if(!strcmp(command, "OBJ"))
		{//Read 3D object
			char name[80];
			char obj[80];
			csVector3 location;
			float scale;

			if(nObj >= numObjects/2 && !switched)
			{
				objPos = objStartPos;
				objPos.x += 6;
				switched = true;
			}
			nObj++;

			objPos.z += 2;
			sscanf(buf, "%s %s %s %f", command, obj, name, &scale);
			location = objPos;
			AddMeshObj (obj, name, room,location, scale);

		}
		/*else if(!strcmp(command, "MOD"))
		{//Read 3D model
			char name[80], file[100], text[80];
			sscanf(buf, "%s %s %s %s", command, name, file, text);
		}
		*/
		else
		{
			//unkown token
		}
		command[0] = '\0';
		fgets(buf, bufSize, fp);
		sscanf(buf, "%s", command);
	}



	room->SetAmbientColor(50,50,50);

	//Prepare the whole room.
	room->Prepare (room);
	room->InitLightMaps (false);
	room->ShineLights ();
	room->CreateLightMaps (System->G3D);

	connector1->SetAmbientColor(50,50,50);
	connector2->SetAmbientColor(50,50,50);
	hallway->SetAmbientColor(50,50,50);

	connector1->Prepare (connector1);
	connector1->InitLightMaps (false);
	connector1->ShineLights ();
	connector1->CreateLightMaps (System->G3D);

	connector2->Prepare (connector2);
	connector2->InitLightMaps (false);
	connector2->ShineLights ();
	connector2->CreateLightMaps (System->G3D);

	hallway->Prepare (hallway);
	hallway->InitLightMaps (false);
	hallway->ShineLights ();
	hallway->CreateLightMaps (System->G3D);

	//Connect room and connector1 room
	roomBackDoor[0]->SetCSPortal(connector1);
	roomBackDoor[0]->SetAlpha(0);
	conn1FrontDoor[0]->SetCSPortal(room);
	conn1FrontDoor[0]->SetAlpha(0);

	//Connect room and connector2 room
	roomFrontDoor[0]->SetCSPortal(connector2);
	roomFrontDoor[0]->SetAlpha(0);
	conn2BackDoor[0]->SetCSPortal(room);
	conn2BackDoor[0]->SetAlpha(0);

	//Connect hallway and connector2
	conn2FrontDoor[0]->SetCSPortal(hallway);
	conn2FrontDoor[0]->SetAlpha(0);
	hallBackDoor[0]->SetCSPortal(connector2);
	hallBackDoor[0]->SetAlpha(0);

	//Prepare room for collision detection
	iPolygonMesh* mesh;
	mesh = QUERY_INTERFACE (room, iPolygonMesh);
    (void)new csCollider (*room, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (connector1, iPolygonMesh);
    (void)new csCollider (*connector1, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (connector2, iPolygonMesh);
    (void)new csCollider (*connector2, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (hallway, iPolygonMesh);
    (void)new csCollider (*hallway, collide_system, mesh);
    mesh->DecRef ();

	//Add collision detection to all the objects in the room
	for (int i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* sp = (csMeshWrapper*)room->meshes[i];
		iMeshObject *s = sp->GetMeshObject();
		mesh = QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			(void)new csCollider (*sp, collide_system, mesh);
			mesh->DecRef ();
		}
	}

	fclose(fp);
	return true;

}

//Build dynamic room as a function of number of objects in the room
bool chimeSector::BuildDynamicRoom2(char *roomDesc, const csVector3 &pos, iCollideSystem* collide_system)
{
	if(!engine) return false;


	int bufSize = 100;
	int count;
	char *buf = roomDesc;
	char line[500];
	char roomURL[MAX_URL];

	csSector *room;
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
	roomList[numRooms] = engine->CreateCsSector (roomURL);
	room = roomList[numRooms];
	//numRooms++;
	csVector3 curPos;

	curPos = pos;
	curPos.z += 1;

	connSize.x = 2;
	connSize.y = 3;
	connSize.z = 2;
	connector1 = engine->CreateCsSector ("connector1");
	BuildStandardConnector(connector1, connSize, curPos, CONN1);

	roomOrigin.x = curPos.x - (size.x/2);
	roomOrigin.y = 0;
	roomOrigin.z = curPos.z + 1;

	curPos.z += 1+(roomSize.z/2);
	BuildStandardRoom(room, roomSize, curPos);
	csStatLight* light;
	light = new csStatLight (0+curPos.x, 4.9+curPos.y, 0+curPos.z, 20, 1, 1, 1, false);
	room->AddLight (light);


	curPos.z += roomSize.z/2 + 1;



	connector2 = engine->CreateCsSector ("connector2");
	BuildStandardConnector(connector2, connSize, curPos, CONN2);
	curPos.z += 1+3;
	hallwaySize.x = 42;
	hallwaySize.y = 4;
	hallwaySize.z = 6;

	hallway = engine->CreateCsSector ("hallway");
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


		if (!strcmp(Class, "User"))
		{
			AddUser(objUrl);  //add the user to the list of users
		}

		if(!strcmp(Class, "Connector"))
		{
			char *tmp = new char[strlen(objUrl)+1];
			strcpy(tmp, objUrl);
			connList.Push(tmp);
		}

		buf += count;

		AddMeshObj (shape, objUrl, room,location, 1);
	}

	room->SetAmbientColor(50,50,50);
	connector1->SetAmbientColor(50,50,50);
	connector2->SetAmbientColor(50,50,50);
	hallway->SetAmbientColor(50,50,50);

	//Prepare the whole room.
	room->Prepare (room);
	room->InitLightMaps (false);
	room->ShineLights ();
	room->CreateLightMaps (System->G3D);

	connector1->Prepare (connector1);
	connector1->InitLightMaps (false);
	connector1->ShineLights ();
	connector1->CreateLightMaps (System->G3D);

	connector2->Prepare (connector2);
	connector2->InitLightMaps (false);
	connector2->ShineLights ();
	connector2->CreateLightMaps (System->G3D);

	hallway->Prepare (hallway);
	hallway->InitLightMaps (false);
	hallway->ShineLights ();
	hallway->CreateLightMaps (System->G3D);

	//Connect room and connector1 room
	roomBackDoor[0]->SetCSPortal(connector1);
	roomBackDoor[0]->SetAlpha(0);
	conn1FrontDoor[0]->SetCSPortal(room);
	conn1FrontDoor[0]->SetAlpha(0);


	//Connect room and connector2 room
	roomFrontDoor[0]->SetCSPortal(connector2);
	roomFrontDoor[0]->SetAlpha(0);
	conn2BackDoor[0]->SetCSPortal(room);
	conn2BackDoor[0]->SetAlpha(0);

	//Connect hallway and connector2
	conn2FrontDoor[0]->SetCSPortal(hallway);
	conn2FrontDoor[0]->SetAlpha(0);
	hallBackDoor[0]->SetCSPortal(connector2);
	hallBackDoor[0]->SetAlpha(0);

	//Prepare room for collision detection
	iPolygonMesh* mesh;
	mesh = QUERY_INTERFACE (room, iPolygonMesh);
    (void)new csCollider (*room, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (connector1, iPolygonMesh);
    (void)new csCollider (*connector1, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (connector2, iPolygonMesh);
    (void)new csCollider (*connector2, collide_system, mesh);
    mesh->DecRef ();

	mesh = QUERY_INTERFACE (hallway, iPolygonMesh);
    (void)new csCollider (*hallway, collide_system, mesh);
    mesh->DecRef ();

	//Add collision detection to all the objects in the room
	for ( i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* sp = (csMeshWrapper*)room->meshes[i];
		iMeshObject *s = sp->GetMeshObject();
		mesh = QUERY_INTERFACE (s, iPolygonMesh);
		if (mesh)
		{
			(void)new csCollider (*sp, collide_system, mesh);
			mesh->DecRef ();
		}
	}

	int numActiveDoors = __min(connList.Length(), 10);
	for ( i = 0 ; i < numActiveDoors; i++)
	{
		hallFrontDoor[i]->SetAlpha(25);
	}
	for ( i = numActiveDoors ; i < 10; i++)
	{
		hallFrontDoor[i]->SetAlpha(100);
	}

	return true;

}

csSector* chimeSector::GetRoom(int index)
{
	return roomList[index];
}

//*********************************************************************************
//*
//* Add mesh object in a given room at a given location.
//*
//*********************************************************************************
iMeshWrapper* chimeSector::AddMeshObj (char* tname, char* sname, csSector* where,
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

//*********************************************************************************
//*
//* Build a standard room of a given size at given location
//*
//*********************************************************************************
bool chimeSector::BuildStandardRoom(csSector *room, csVector3 const &size, csVector3 const &pos)
{
	csMaterialWrapper* txt;
	csVector3 trans(0,0,0);

	//Load standard texure for walls
	srand( (unsigned)time( NULL ) );
	int dice = rand() % 3;

	if( dice == 0 )
		txt = engine->GetMaterials ()->FindByName ("brownfabric");
	else if( dice == 1)
		txt = engine->GetMaterials ()->FindByName ("glasswall");
	else
		txt = engine->GetMaterials ()->FindByName ("funkywall");


	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	buildFrontDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 4,roomFrontDoor);
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));

	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	//BuildWall(room, size, trans, BACK, txt, csVector3(3,3,3));
	buildBackDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 4,roomBackDoor);

	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(room, size, trans, LEFT, txt, csVector3(3,3,3));

	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(room, size, trans, RIGHT, txt, csVector3(3,3,3));

	//Build Floor wall of the room
	//Load sandard texure for walls
	dice = rand() % 3;
	if( dice == 0 )
		txt = engine->GetMaterials ()->FindByName ("woodfloor");
	else if( dice == 1)
		txt = engine->GetMaterials ()->FindByName ("tilefloor");
	else
		txt = engine->GetMaterials ()->FindByName ("checkerfloor");

	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, FLOOR, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	txt = engine->GetMaterials ()->FindByName ("marble1");
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, ROOF, txt, csVector3(3,3,3));

	return true;
}

//*********************************************************************************
//*
//* Build a standard connector that connects a room with a hallway
//*
//*********************************************************************************
bool chimeSector::BuildStandardConnector(csSector *room, csVector3 const &size, csVector3 const &pos, int type)
{
	csMaterialWrapper* txt;
	csVector3 trans(0,0,0);
	csPolygon3D **connFrontDoor, **connBackDoor;

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
	txt = engine->GetMaterials ()->FindByName ("whitemarble");

	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));
	buildFrontDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 0, connFrontDoor);


	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	//BuildWall(room, size, trans, BACK, txt, csVector3(3,3,3));
	buildBackDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, 0, connBackDoor);
	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(room, size, trans, LEFT, txt, csVector3(3,3,3));

	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(room, size, trans, RIGHT, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, ROOF, txt, csVector3(3,3,3));

	//Build Floor wall of the room
	txt = engine->GetMaterials ()->FindByName ("metalfloor");
	//txt = engine->GetMaterials ()->FindByName ("water");
	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, FLOOR, txt, csVector3(2,2,1));

	return true;
}
//*********************************************************************************
//*
//* Build a standard hallway of a given size at given location
//*
//*********************************************************************************
bool chimeSector::BuildHallway(csSector *room, csVector3 const &size, csVector3 const &pos)
{
	csMaterialWrapper* txt;
	csVector3 trans(0,0,0);
	float doorOff = (size.x - 2)/2;
	//Load sandard texure for walls
	txt = engine->GetMaterials ()->FindByName ("marble1");
	//Build front wall of the room
	trans.Set(-size.x/2.0, 0, size.z/2.0);
	trans += pos;
	buildFrontDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 10, 2, hallFrontDoor);
	//BuildWall(room, size, trans, FRONT, txt, csVector3(3,3,3));
	//Build back wall of the room
	trans.Set(-size.x/2.0, 0, -size.z/2.0);
	trans += pos;
	buildBackDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(2,3,0), 1, doorOff, hallBackDoor);
	//Build Left wall of the room
	trans.Set(-size.x/2, 0, -size.z/2.0);
	trans += pos;
	///buildLeftDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(0,3,2), 5, 2, hallLeftDoor);
	BuildWall(room, size, trans, LEFT, txt, csVector3(3,3,3));
	//Build Right wall of the room
	trans.Set(size.x/2, 0, -size.z/2.0);
	trans += pos;
	BuildWall(room, size, trans, RIGHT, txt, csVector3(3,3,3));
	//buildRightDoorWall(room, size, trans, txt, csVector3(3,3,3), csVector3(0,3,2), 5, 2, hallRightDoor);

	//Build Floor wall of the room
	//Load sandard texure for walls
	txt = engine->GetMaterials ()->FindByName ("hallfloor");
	trans.Set(-size.x/2, 0, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, FLOOR, txt, csVector3(3,3,3));

	//Build Roof wall of the room
	txt = engine->GetMaterials ()->FindByName ("marble1");
	trans.Set(-size.x/2, size.y, -size.z/2);
	trans += pos;
	BuildWall(room, size, trans, ROOF, txt, csVector3(3,3,3));


	return true;
}


//*********************************************************************************
//*
//* Build a given wall of a room
//*
//*********************************************************************************
csPolygon3D * chimeSector::BuildWall(csSector *room, csVector3 const &size, csVector3 const &pos, int type, csMaterialWrapper *txt, csVector3 const &txtSize)
{

	csVector3 v1;
	csVector3 v2;
	csVector3 v3;
	csVector3 v4;

	v1 = stdVector[type+0];
	v2 = stdVector[type+1];
	v3 = stdVector[type+2];
	v4 = stdVector[type+3];

	csPolygon3D *p = NULL;

	v1.x = v1.x * size.x; v1.y = v1.y * size.y; v1.z = v1.z * size.z;
	v2.x = v2.x * size.x; v2.y = v2.y * size.y; v2.z = v2.z * size.z;
	v3.x = v3.x * size.x; v3.y = v3.y * size.y; v3.z = v3.z * size.z;
	v4.x = v4.x * size.x; v4.y = v4.y * size.y; v4.z = v4.z * size.z;

	v1 += pos;
	v2 += pos;
	v3 += pos;
	v4 += pos;

	p = room->NewPolygon (txt);
	p->AddVertex (v1.x, v1.y, v1.z);
	p->AddVertex (v2.x, v2.y, v2.z);
	p->AddVertex (v3.x, v3.y, v3.z);
	p->AddVertex (v4.x, v4.y, v4.z);
	//p->SetTextureSpace (p->Vobj (0), p->Vobj (1), txtSize.x);
	if(type == RIGHT || type == BACK)
	{
		p->SetTextureSpace (p->Vobj (2), p->Vobj (3), txtSize.x, p->Vobj (1), txtSize.y);
	}
	else
	{
		p->SetTextureSpace (p->Vobj (0), p->Vobj (1), txtSize.x, p->Vobj (3), txtSize.y);
	}

	return p;
}
//Build right wall with doors
bool chimeSector::buildRightDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, csPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;

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

	csMaterialWrapper *t = engine->GetMaterials ()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildWall(room, size2, curPos, RIGHT, txt, txtSize);
	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.z > 0)
			BuildWall(room, size1, curPos, RIGHT, txt, txtSize);
		curPos.z += offset;

		pList[i] = BuildWall(room, doorSize, curPos, RIGHT, t, doorTxtSize);
		curPos.z += doorSize.z;
	}
	size1.z = (pos.z + size.z - curPos.z);
	if(size1.z > 0)
		BuildWall(room, size1, curPos, RIGHT, txt, txtSize);

	return true;
}

//Build left wall with doors
bool chimeSector::buildLeftDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, csPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;;

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

	csMaterialWrapper *t = engine->GetMaterials ()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildWall(room, size2, curPos, LEFT, txt, txtSize);
	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.z > 0)
			BuildWall(room, size1, curPos, LEFT, txt, txtSize);
		curPos.z += offset;
		pList[i] = BuildWall(room, doorSize, curPos, LEFT, t, doorTxtSize);
		curPos.z += doorSize.z;
	}
	size1.z = (pos.z + size.z - curPos.z);
	if(size1.z > 0)
		BuildWall(room, size1, curPos, LEFT, txt, txtSize);

	return true;
}


//Build front wall with doors
bool chimeSector::buildFrontDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, csPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;

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

	csMaterialWrapper *t = engine->GetMaterials ()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildWall(room, size2, curPos, FRONT, txt, txtSize);
	curPos = pos;

	for(int i = 0; i < numDoors; i++)
	{
		if(size1.x > 0)
			BuildWall(room, size1, curPos, FRONT, txt, txtSize);
		curPos.x += offset;

		pList[i] = BuildWall(room, doorSize, curPos, FRONT, t, doorTxtSize);
		curPos.x += doorSize.x;
	}
	size1.x = (pos.x + size.x - curPos.x);
	if(size1.x > 0)
		BuildWall(room, size1, curPos, FRONT, txt, txtSize);

	return true;
}


//Build front wall with doors
bool chimeSector::buildBackDoorWall(csSector *room, csVector3 const &size, csVector3 const &pos, csMaterialWrapper *txt, csVector3 const &txtSize,
		                csVector3 const &doorSize, int numDoors, float offset, csPolygon3D *pList[])
{

	csVector3 size1, size2, curPos, doorTxtSize;

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

	csMaterialWrapper *t = engine->GetMaterials ()->FindByName ("halldoor");

	if(size2.y > 0)
		BuildWall(room, size2, curPos, BACK, txt, txtSize);
	curPos = pos;
	for(int i = 0; i < numDoors; i++)
	{
		if(size1.x > 0)
			BuildWall(room, size1, curPos, BACK, txt, txtSize);
		curPos.x += offset;
		pList[i] = BuildWall(room, doorSize, curPos, BACK, t, doorTxtSize);
		curPos.x += doorSize.x;
	}
	size1.x = (pos.x + size.x - curPos.x);
	if(size1.x > 0)
		BuildWall(room, size1, curPos, BACK, txt, txtSize);

	return true;
}


//*********************************************************************************
//*
//* Return Default camera location of this sector
//*
//*********************************************************************************
csVector3* chimeSector::GetCamLocation()
{
	return &camLocation;
}

//*********************************************************************************
//*
//* Return location and hallway of a given hallway-door of this chime sector
//*
//*********************************************************************************
bool chimeSector::GetDoorInfo(int doorNum, csVector3 &location, csPolygon3D* &door, csSector* &h)
{
	location = firstDoorLocation;
	location.x += 4*doorNum;
	door = hallFrontDoor[doorNum];
	h = hallway;
	return true;
}

//Set linked chime sector info
bool chimeSector::SetLinkedSectorInfo(chimeSector *sect, int doorNum)
{
	linkedSector = sect;
	linkedDoor = doorNum;
	return true;
}

//Get given halway door of this chime sector.
csPolygon3D*  chimeSector::GetHallwayDoor(int doorNum)
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
csPolygon3D*  chimeSector::GetBackDoor()
{
	return conn1BackDoor[0];
}

//Get spatial location of a given hallway door.
bool chimeSector::GetHallwayDoorLoc(int doorNum, csVector3 & location)
{
	location = firstDoorLocation;
	location.x += 4*doorNum;
	return true;
}

bool chimeSector::Connect(csPolygon3D *door, csSector *hallway)
{
	door->SetCSPortal(connector1);
	door->SetAlpha(0);
	conn1BackDoor[0]->SetCSPortal(hallway);
	conn1BackDoor[0]->SetAlpha(0);
	return true;
}

//Add user name to the list of users in the sector
bool chimeSector::AddUser(char *userID)
{
	if(!userID) return false;

	char *tmp = new char[strlen(userID)+1];
	strcpy(tmp, userID);
	userList.Push(tmp);
	return true;
}

//Delete user name from the list of users in the sector
bool chimeSector::deleteUser(char *userID)
{
	if(!userID) return false;

	char *user = NULL;
	for(int i = 0; i < userList.Length(); i++)
	{
		if(!strcmp(userList.Get(i), userID))
		{
			userList.Delete(i);
			return true;
		}
	}
	return false;
}

//connect "otherSect" to the "atDoor" hallway door of this sector.
bool chimeSector::ConnectSectors(chimeSector *otherSect, int atDoor)
{
	if( !otherSect ) return false;

	csPolygon3D *door = hallFrontDoor[atDoor];
	csPolygon3D *otherSectorBackDoor = otherSect->GetBackDoor();

	door->SetCSPortal(otherSect->GetConn1());
	door->SetAlpha(0);
	otherSectorBackDoor->SetCSPortal(hallway);
	otherSectorBackDoor->SetAlpha(0);
	doorSec[atDoor] = otherSect;

	otherSect->SetLinkedSectorInfo(this, atDoor);

	return true;
}

//diconnect this sector from the linked sector
bool chimeSector::DisconnectSector()
{
	if(!linkedSector ) return false;

	csPolygon3D *linkedSectorDoor = linkedSector->GetHallwayDoor(linkedDoor);

	linkedSectorDoor->SetCSPortal(NULL);
	linkedSectorDoor->SetAlpha(25);
	conn1BackDoor[0]->SetCSPortal(NULL);
	conn1BackDoor[0]->SetAlpha(25);

	linkedSector->SetDoorSector(linkedDoor, NULL);
	linkedSector = NULL;
	linkedDoor = -1;

	return true;
}

bool chimeSector::Disconnect()
{
	conn1BackDoor[0]->SetCSPortal(NULL);
	conn1BackDoor[0]->SetAlpha(100);
	return true;
}

bool chimeSector::UnlinkHallwayDoors()
{
	chimeSector *sec;

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

bool chimeSector::SetDoorSector(int doorNum, chimeSector *sec)
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

chimeSector* chimeSector::GetDoorSector(int doorNum)
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

char* chimeSector::GetDoorUrl(int doorNum)
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
// Find  an object in this sector
csMeshWrapper* chimeSector::FindObject(char *objectUrl, csSector *&room)
{
	room = NULL;
	int i;

	room = roomList[0];
	csMeshWrapper* obj = NULL;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];

		if( !strcmp(objectUrl, mesh->GetName()) )
		{
			obj = mesh;
			return obj;
		}
	}
	room = hallway;
	obj = NULL;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];

		if( !strcmp(objectUrl, mesh->GetName()) )
		{
			obj = mesh;
			return obj;
		}
	}
	room = connector1;
	obj = NULL;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];

		if( !strcmp(objectUrl, mesh->GetName()) )
		{
			obj = mesh;
			return obj;
		}
	}
	room = connector2;
	obj = NULL;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];

		if( !strcmp(objectUrl, mesh->GetName()) )
		{
			obj = mesh;
			return obj;
		}
	}

	return NULL;
}

//check if a given beam hits any of the meshes in this sector.
csMeshWrapper* chimeSector::SelectMesh (const csVector3 &start, const csVector3 &end, csVector3 &isect, float &dist)
{
	int i;
	csSector *room;

	//check for mesh in  the main room
	room = roomList[0];
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			return mesh;
		}
	}

	//check for mesh in  the hallway
	room = hallway;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			return mesh;
		}
	}

	//check for mesh in  the connector2
	room = connector2;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			return mesh;
		}
	}

	//check for mesh in  the connector1
	room = connector1;
	for (i = 0 ; i < room->meshes.Length(); i++)
	{
		csMeshWrapper* mesh = (csMeshWrapper*)room->meshes[i];
		if(mesh->HitBeam(start, end, isect, &dist) )
		{
			return mesh;
		}
	}

	return NULL;
}

//Find the room of a sector that contains this point
//Note: heuristics and shape of the sector are being used for the test
csSector* chimeSector::FindRoomContainingPoint(csVector3 p)
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
int chimeSector::IsRoomOf(csSector *room)
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
//* Check If a given beam intersects any of the polygons of the chimesector
//*
//*********************************************************************************
bool chimeSector::HitBeam(const csVector3 &start, const csVector3 &end, csVector3 &isect)
{

	csPolygon3D* p = NULL;

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
bool chimeSector::HallwayHitBeam (const csVector3 &start, const csVector3 &end, csVector3 &isect, int &doorNum)
{

	doorNum = -1;
	csPolygon3D* p = NULL;
	p = hallway->HitBeam (start, end, isect);
	int numActiveDoor = connList.Length();

	//Check if intersecting polygon is a door
	if( p )
	{
		for(int i = 0; i < numActiveDoor; i++)
		{
			if(p == hallFrontDoor[i])
			{
				doorNum = i;
				return true;
			}
		}
	}
	return false;
}

