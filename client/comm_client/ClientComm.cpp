#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "ClientComm.h"
#include <iostream.h>

//using namespace std;
DWORD WINAPI StartUDPListener(LPVOID Lparam);
DWORD WINAPI StartSienaSubscriber(LPVOID Lparam);

ClientComm::~ClientComm() {
	delete siena_publisher;
	delete cl;

	//now stop the threads
	CloseHandle(proc);
	CloseHandle(siena_proc);

	//now destroy their object
	delete uds;
	delete siena_subscriber;
}

ClientComm::ClientComm(int port, const char *SAddress, int Sport, const char *_username, const char *_password, ChimeSystemDriver *_nav) {
	password = _password;
	username = _username;
	nav = _nav;

	cl = new UDPClient(port, nav);
	
	siena_publisher = new SienaPublisher(SAddress, Sport, username, password, nav); 
	
	//spawn off a thread for the UDP server
	uds = new UDPServer(port, nav); 

	DWORD nThreadID;
	//HANDLE proc;
	proc = CreateThread(NULL,0,StartUDPListener,uds,0,&nThreadID);

	if ( proc == NULL )
	{
		cout << "Error: UDP Listener Thread can not be created.\n";
		exit(1);
	}
	
	 
	
	//spawn off a thread which will listen for Siena Events
	siena_subscriber = new SienaSubscriber(SAddress, Sport, username, nav);

	DWORD nThreadID2;
	//HANDLE siena_proc;
	siena_proc = CreateThread(NULL,0,StartSienaSubscriber, siena_subscriber,0,&nThreadID2);

	if ( siena_proc == NULL )
	{
		cout << "Error: Siena Server Thread can not be created.\n";
		exit(1);
	}
	

}


DWORD WINAPI StartUDPListener(LPVOID lParam)
{ 
	UDPServer *uds = (UDPServer*) lParam;

	uds->startServer();
	return(1);
}

DWORD WINAPI StartSienaSubscriber(LPVOID lParam)
{ 
	SienaSubscriber *ss = (SienaSubscriber*) lParam;

	ss->startServer();
	return(1);
}


//subscribe to a events in a room
bool ClientComm::subscribeRoom(char *room, int option) {
	siena_publisher->subscribeRoom(room, option);
	return true;
}

//subscribe to all events published by client
bool ClientComm::subscribeClient() {
	siena_publisher->subscribeClient();
	return true;
}

//subscribe to all events of some method
bool ClientComm::subscribeMethod(char *method, char *room, int option) {
	siena_publisher->subscribeMethod(method, room, option);
	return true;
}

//unsubscribe from all events of some room
bool ClientComm::unsubscribeRoom(char *room, int option) {
	siena_publisher->unsubscribeRoom(room, option);
	return true;
}


//subscribe to ALL Events of that room
bool ClientComm::subscribeALL(char *room) {
	siena_publisher->subscribeALL(room);
	return true;
}


//unsubscribe from ALL events
bool ClientComm::unsubscribeALL() {
	siena_publisher->unsubscribeALL();
	return true;
}


//unsubscribe the client
bool ClientComm::unsubscribeClient() {
	siena_publisher->unsubscribeClient();
	return true;
}

bool ClientComm::SendUDPFunction(const char *ip_addr, int function, char *params) {
	cl->sendMess(ip_addr, function, params);
	return true;
}

bool ClientComm::SendSienaFunction(int function, char *params, char *address, char *prot) {
	siena_publisher->publish(function, params, address, prot);
	return true;
}

/*int main(int argc, char *argv[])
{
	ChimeSystemDriver *nav = new ChimeSystemDriver();
	ClientComm *comm = new ClientComm(9999, "localhost", 1234, "denis", "denis", nav);
	while(1) {
		comm->SendUDPFunction("192.168.1.100", 2, "room something others");
		comm->SendSienaFunction(c_connect, "testing", "http://www.cs.brandeis.edu", "HTTP");
		Sleep(10000);
	}
	return 0;
}
*/