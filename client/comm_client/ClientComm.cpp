#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include <iostream.h>

//using namespace std;
DWORD WINAPI StartUDPListener(LPVOID Lparam);
DWORD WINAPI StartSienaSubscriber(LPVOID Lparam);


ClientComm::ClientComm(int port, char *SAddress, int Sport, char *_username, char *_password, NavCallback *nav) {
	password = _password;
	username = _username;

	/*
	cl = new UDPClient(port);
	*/
	
	siena_publisher = new SienaPublisher(SAddress, Sport, username, password);

	/*
	//spawn off a thread for the UDP server
	uds = new UDPServer(port, nav); 

	DWORD nThreadID;
	HANDLE proc;
    proc = CreateThread(NULL,0,StartUDPListener,uds,0,&nThreadID);

	if ( proc == NULL )
	{
       cout << "Error: UDP Listener Thread can not be created.\n";
	   exit(1);
    }
	*/
	 
	//spawn off a thread which will listen for Siena Events
	siena_subscriber = new SienaSubscriber(SAddress, Sport, username, nav);

	DWORD nThreadID2;
	HANDLE siena_proc;
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



bool ClientComm::SendUDPFunction(int function, char *params) {
	cl->sendMess("192.168.1.100", "This is what I am sending");
	return true;
}

bool ClientComm::SendSienaFunction(int function, char *params, char *address, char *prot) {
	siena_publisher->publish("testFunction", "something", "http://www.yahoo.com", "HTTP");
	return true;
}

int main(int argc, char *argv[])
{
	NavCallback *nav = new NavCallback();
	ClientComm *comm = new ClientComm(9999, "astor.psl.cs.columbia.edu", 1234, "denis", "denis", nav);
	while(1) {
		//comm->SendUDPFunction(1, "testing");
		comm->SendSienaFunction(1, "testing", "http://www.yahoo.com", "HTTP");
		Sleep(100000);
	}
	return 0;
}