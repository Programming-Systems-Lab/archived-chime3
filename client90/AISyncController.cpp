// AISyncController.cpp: implementation of the AISyncController class.
//
//////////////////////////////////////////////////////////////////////


#include "AI2TVConstants.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cssysdef.h"
#include "comm_client/ClientComm.h"
#include "AISyncController.h"
#include "ChimeSystemDriver.h"
#include "ChimeApp.h"
/**************************
/*	static function for thread-running the server
/*
/***************************/

DWORD WINAPI RunAI2TVUDPServer(LPVOID lParam){
	AISyncController* sync = (AISyncController*) lParam;
	sync->server->startServer();
	return 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern ChimeSystemDriver *Sys;
AISyncController::AISyncController(ChimeSystemDriver *drv) 
{
	server = new UDPServer(AI2TV_UDP_PORT,drv);
	client = new UDPClient(AI2TV_UDP_PORT,drv);
	num_clients=0;
	listen();

	// the multicast group is not set dynamically
	/* int* ids = new int[3];
	ids[0]=2;ids[1]=5;ids[2]=4;
	char** mcg = new char*[3];
	mcg[1] = "128.59.23.42";
	mcg[2] = "128.59.23.34";
mcg[0] = "160.39.244.111";
	setMultiCastGroup(3,mcg,ids);
	//parseResponse("this is cool");
*/

}

void AISyncController::parseResponse(char* string){
	char msg[1000];
	sprintf(msg,"UDP/SERVER/%s\0",string);
	Sys->myVideoPlayer->debug(1,msg);


	//return 0;
}


void AISyncController::setMultiCastGroup(int n_clients, char **client_addresses, int* client_id_list){
	num_clients=n_clients;
	multicast_group=client_addresses;
	clientIDs=client_id_list;
}

AISyncController::~AISyncController()
{
	if (server) delete server;
	if (client) delete client;
}

void AISyncController::send(char* msg){
	client->sendMess("127.0.0.1",1,msg);
}

void AISyncController::listen(){
	CoInitialize(NULL);
	DWORD threadId;
		Sys->myVideoPlayer->debug(2,"UDPSERVER:Starting to listen");
	CreateThread(NULL,0,RunAI2TVUDPServer,this,0,&threadId);
}

