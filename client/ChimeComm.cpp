//*******************************************************************
//*
//* Implementation of ChimeComm communication class.
//*
//*******************************************************************

#include <stdlib.h>
#include <string.h>

#include "cssysdef.h"
#include "chimeComm.h"
#include "comm_client/udp.h"

chimeComm::chimeComm()
{
	
}

chimeComm::chimeComm(ClientComm *client_comm)
{

	chimeComm::client_comm = client_comm;
}

chimeComm::~chimeComm()
{

}

void chimeComm::SetChimeCom(ClientComm *client_comm)
{

	chimeComm::client_comm = client_comm;
}


//Utility function to convert from float to string
char * chimeComm::ftoa(float num)
{
	int i, dec, sign, size;
	char str[15], tempstr[15];

	strcpy(tempstr, fcvt(num, 4, &dec, &sign));
	size = strlen(tempstr);

	for (i = 0; i < dec; i=i+1)
		str[i] = tempstr[i];

	str[dec] = '.'; //put the decimal point

	for (i = dec; i <=size; i=i+1)
		str[i+1] = tempstr[i];
	
	return strdup(str);
}

//Append given token to a string.
bool chimeComm::AppendToken(char *command, char *token)
{
	strcat(command, " ");
	strcat(command, token);

	return true;
}

//Connect to a given CHIME Server
bool chimeComm::Connect(char *server, char *userID, char *password)
{
	strcpy(command, server);	
	AppendToken(command, userID);
	AppendToken(command, password);
	
	//client_comm->
	//SendFunction(c_connect, command);
	return true;
}

//Request info about a given room
bool chimeComm::GetRoom(char *url)
{
	strcpy(command, url);

	client_comm->SendSienaFunction(c_getRoom, command, url,"HTTP");
	return true;
}


//Send updated position of a moved object
bool chimeComm::MoveObject(char *roomUrl, char *objectUrl, float x, float y, float z)
{
	strcpy(command, roomUrl);
	AppendToken(command, objectUrl);
	AppendToken(command, ftoa(x));
	AppendToken(command, ftoa(y));
	AppendToken(command, ftoa(z));

	client_comm->SendSienaFunction(c_moveObject, command, roomUrl, "HTTP");
	return true;
}

//Send updated position of the user
bool chimeComm::MoveUser(char *roomUrl, char *userID, float x, float y, float z, const csStrVector *userList)
{
	
	strcpy(command, roomUrl);
	AppendToken(command, userID);
	AppendToken(command, ftoa(x));
	AppendToken(command, ftoa(y));
	AppendToken(command, ftoa(z));
	 
	//Send signal to all the user in this room
	for(int i = 0 ; i < userList->Length(); i++)
	{
		client_comm->SendUDPFunction(userList->Get(i), c_moveUser, command);
	}
	return true;
}

//Broadcast entrance of a user in a given room
bool chimeComm::UserEnteredRoom(char *userID, char *newRoomUrl, float x, float y, float z)
{
	
	strcpy(command, userID);
	AppendToken(command, newRoomUrl);
	AppendToken(command, ftoa(x));
	AppendToken(command, ftoa(y));
	AppendToken(command, ftoa(z));

	client_comm->SendSienaFunction(c_enteredRoom, command, newRoomUrl,"HTTP");
	return true;
}

//Broadcast that a user has lift a given room
bool chimeComm::UserLeftRoom(char *userID, char *oldRoomUrl)
{
	
	strcpy(command, userID);
	AppendToken(command, oldRoomUrl);	

	client_comm->SendSienaFunction(c_leftRoom, command, oldRoomUrl,"HTTP");
	return true;
}

//Broadcast addition of new object in a given room
bool chimeComm::AddObject(char *roomUrl, char *objectUrl, float x, float y, float z)
{
	strcpy(command, roomUrl);
	AppendToken(command, objectUrl);
	AppendToken(command, ftoa(x));
	AppendToken(command, ftoa(y));
	AppendToken(command, ftoa(z));

	client_comm->SendSienaFunction(c_addObject, command, roomUrl,"HTTP");
	return true;
}

//Broadcast deletion of new object in a given room
bool chimeComm::DeleteObject(char *roomUrl, char *objectUrl)
{
	strcpy(command, roomUrl);
	AppendToken(command, objectUrl);

	client_comm->SendSienaFunction(c_deleteObject, command, roomUrl,"HTTP");
	return true;
}

//Subscribe for events of this room
bool chimeComm::SubscribeRoom(char *roomUrl, char *userID)
{
	strcpy(command, roomUrl);
	AppendToken(command, userID);

	client_comm->subscribeALL(roomUrl);
	return true;
}
//Unsubscribe for events of this room
bool chimeComm::UnsubscribeRoom(char *roomUrl, char *userID)
{
	strcpy(command, roomUrl);
	AppendToken(command, userID);

	client_comm->unsubscribeRoom(roomUrl, EVENTS_FOR_ANYONE);
	//client_comm->SendSienaFunction(c_unsubscribeRoom, command, roomUrl,"HTTP");
	return true;
}

//Disconnect user from CHIME server
bool chimeComm::Disconnect(char *userID)
{
	strcpy(command, userID);

//	client_comm->SendSienaFunction(c_disconnect, command, roomUrl,"HTTP");
	return true;
}
