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





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

chimeSector::chimeSector()
{
	mainRoom = NULL;
	engine = NULL;
	System = NULL;
	numRooms = 0;	//FIXIT: This should not be hardcoded.
	memset(roomList, 0, 10*sizeof(csSector*));
}


chimeSector::chimeSector(csSystemDriver  *Sys, csEngine *e)
{
	mainRoom = NULL;
	System = Sys;
	engine = e;
	numRooms = 0;	//FIXIT: This should not be hardcoded.
	memset(roomList, 0, 10*sizeof(csSector*));
}

chimeSector::~chimeSector()
{
	if(mainRoom)
	{
		delete mainRoom;
		mainRoom = NULL;
		//FIXIT: Other rooms need to be freed.
	}
	
}


//Build a new room as specified by the data server.
bool chimeSector::BuildRoom()
{
	if(!engine) return false;
	

  csMaterialWrapper* tm = engine->GetMaterials ()->FindByName ("stone");   

  roomList[0] = engine->CreateCsSector ("Room0");
  mainRoom = roomList[0];

  csPolygon3D* p;
  p = mainRoom->NewPolygon (tm);
  p->AddVertex (-5, 0, 5);
  p->AddVertex (5, 0, 5);
  p->AddVertex (5, 0, -5);
  p->AddVertex (-5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = mainRoom->NewPolygon (tm);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (5, 20, -5);
  p->AddVertex (5, 20, 5);
  p->AddVertex (-5, 20, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = mainRoom->NewPolygon (tm);
  p->AddVertex (-5, 20, 5);
  p->AddVertex (5, 20, 5);
  p->AddVertex (5, 0, 5);
  p->AddVertex (-5, 0, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = mainRoom->NewPolygon (tm);
  p->AddVertex (5, 20, 5);
  p->AddVertex (5, 20, -5);
  p->AddVertex (5, 0, -5);
  p->AddVertex (5, 0, 5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = mainRoom->NewPolygon (tm);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (-5, 20, 5);
  p->AddVertex (-5, 0, 5);
  p->AddVertex (-5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  p = mainRoom->NewPolygon (tm);
  p->AddVertex (5, 20, -5);
  p->AddVertex (-5, 20, -5);
  p->AddVertex (-5, 0, -5);
  p->AddVertex (5, 0, -5);
  p->SetTextureSpace (p->Vobj (0), p->Vobj (1), 3);

  csStatLight* light;
  light = new csStatLight (-3, 5, 0, 10, 1, 0, 0, false);
  mainRoom->AddLight (light);
  light = new csStatLight (3, 5, 0, 10, 0, 0, 1, false);
  mainRoom->AddLight (light);
  light = new csStatLight (0, 5, -3, 10, 0, 1, 0, false);
  mainRoom->AddLight (light);

  AddMeshObj ("balloons", "balloon", mainRoom,csVector3(0,1,-1), 0.2);
  return true;
	
}

//Dummy room 
bool chimeSector::ReadRoom(char *fileName)
{
	if(!engine) return false;

	FILE *fp = fopen(fileName, "r");
	if(!fp) return false;
	int bufSize = 100;
	char buf[100];
	char command[80];

	csSector *room;
	csPolygon3D* p;	
	csMaterialWrapper* txt;

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
			csVector3 pos;

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
			char name[80], file[100], text[80];
			sscanf(buf, "%s %s %s %s", command, name, file, text);
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

		fgets(buf, bufSize, fp);
		sscanf(buf, "%s", command);		
	}


	csStatLight* light;
	light = new csStatLight (3, 3, 3, 10, 1, 1,1, false);
/*	room->AddLight (light);
	light = new csStatLight (3, 5, 0, 10, 0, 0, 1, false);
	room->AddLight (light);
	light = new csStatLight (0, 5, -10, 10, 0, 1, 0, false);
	room->AddLight (light);
*/
	room->SetAmbientColor(60,60,60);
	//  AddMeshObj ("balloons", "balloon", room,csVector3(0,3,-1), 0.1);
	
	//Prepare the whole room.
	room->Prepare (room);
	room->InitLightMaps (false);
	room->ShineLights ();
	room->CreateLightMaps (System->G3D);

	fclose(fp);
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
