//Author: Navdeep S. Tinna
//Date	: 04/28/2001
//File	: Client.cpp

//Copyright 2001 PSL lab, Columbia University



//*********************************************************************************
//*
//*  ChimeComm.cpp  responsible for all comunication with
//*	 Chime Server and all other clients.
//*
//*********************************************************************************

#ifndef __CHIME_COMM_H__
#define __CHIME_COMM_H__

#include "csutil/csstrvec.h"
#include "InfoStorer.h"

class ClientComm;
class ChimeSystemDriver;

class chimeComm
{

private:
   char *userID;
   char command[500];
   ClientComm *client_comm;
   ChimeSystemDriver *System;

   //Append given token to a string.
	bool AppendToken(char *command, char *token);
public:
	//Default constructor
	chimeComm();
	//Constructor
	chimeComm(ClientComm *client_comm, ChimeSystemDriver *System);
	//Destructor to clean up and shutdown the engine.
	~chimeComm();
	void SetChimeCom(ClientComm *client_comm, ChimeSystemDriver *System);

	//Connect to a given Chime Server
	bool connect(char *server, char *userID, char *password);
	//Connect to a given CHIME Server
	bool Connect(char *server, char *userID, char *password);
	//Request info about a given room
	bool GetRoom(char *url);
	//Send updated position of a moved object
	bool MoveObject(char *roomUrl, char *objectUrl, float x, float y, float z);
	//Send updated position of the user
	bool MoveUser(char *roomUrl, char *username, char *ip_address, float x, float y, float z, const csStrVector *userList);
	//Broadcast entrance of a user in a given room
	bool UserEnteredRoom(char *username, char *ip_address, char *newRoomUrl, float x, float y, float z);
	//Broadcast that a user has left a given room
	bool UserLeftRoom(char *username, char *ip_address, char *oldRoomUrl);
	//Broadcast addition of new object in a given room
	bool AddObject(char *roomUrl, char *objectUrl, float x, float y, float z);
	//Broadcast deletion of new object in a given room
	bool DeleteObject(char *roomUrl, char *objectUrl);
	//Subscribe for events of this room
	bool SubscribeRoom(char *roomUrl, char *username);
	//Unsubscribe for events of this room
	bool UnsubscribeRoom(char *roomUrl, char *username);
	//Disconnect user from CHIME server
	bool Disconnect(char *username);

	void UpdateChimeComm();

	//Utility function to convert float to string
	static char* ftoa(float num);

};


#endif
