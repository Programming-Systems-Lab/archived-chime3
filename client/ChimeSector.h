// chimeSector.h: interface for the chimeSector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
#define AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "csengine/meshobj.h"

class csSector;
class csEngine;

class chimeSector  
{
	csSector* mainRoom;					//Main room in the sector
	csSector* roomList[10];				//List of all the rooms in the sector
	int		  numRooms;					//Number of rooms in th sector.
	csEngine* engine;					//Engine that will render this room
	csSystemDriver *System;

public:
	csSector* GetRoom(int index);

	bool ReadRoom(char *fileName);
	//Build a new room as specified by the data server.
	bool BuildRoom();
	//Adds a mesh object in the room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, csSector* where,
		csVector3 const& pos, float size);
	chimeSector();
	chimeSector(csSystemDriver  *Sys, csEngine *e);
	virtual ~chimeSector();
	
};

#endif // !defined(AFX_CHIMESECTOR_H__B2D2C4E8_7AC2_4A3F_AD9A_776822C11E86__INCLUDED_)
