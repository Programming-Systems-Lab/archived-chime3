//Author: Navdeep S. Tinna
//Date	: 03/05/2001
//File	: Client.cpp

//Copyright 2001 PSL lab, Columbia University



//*********************************************************************************
//*
//*  Client.cpp defines the main Class that is responsible for initializing
//*	the Chime client.
//*
//*********************************************************************************

#ifndef __CLIENT_H__
#define __CLIENT_H__

//Includes from Crystal space
#include "cssys/sysdriv.h"
#include "ChimeSector.h"	// Added by ClassView
#include "csengine/meshobj.h"

//Forward declaration for classes
class csEngine;
class csView;

class Client: public SysSystemDriver
{
  typedef SysSystemDriver superclass;

private:
	chimeSector *curSector;		//Contains all the descrition of the current sector.
    csView      *view;			//View of the curSector.
	csEngine* engine;			//Pointer to the Crystal-Space engine;

	bool freeLook;				//Toggle for turning free look on or off
	csVector3 camAngle;			//Current camera orientation.
public:

	//Load a mesh object from a file.
	bool LoadMeshObj (char *filename, char *templatename, char* txtname);
	//Add mesh object in a given room.
	iMeshWrapper* AddMeshObj (char* tname, char* sname, csSector* where,
		csVector3 const& pos, float size);
	//Load chime objects and textures specified in a given file.
	bool LoadChimeLib(char *fileName);
	//Transport user from a given room to a new room
	bool Transport(csSector *room, csVector3 pos, csVector3 lookPos, csVector3 lookUp);
	//Handles any mouse or keyboard event.
	virtual bool HandleEvent (iEvent &Event);
	//Handles all the keyboard event.
	bool HandleKeyEvent (iEvent &Event);
	//Prepares NextFrame
	virtual void NextFrame();
	//Method to initialize crystal-space engine.
	virtual bool Initialize (int argc, const char* const argv[],const char *iConfigName);
	//Default constructor  
	Client(); 
	//Destructor to clean up and shutdown the engine.
	virtual ~Client();  
};


#endif