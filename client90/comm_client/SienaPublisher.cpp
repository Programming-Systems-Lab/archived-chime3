#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "ClientComm.h"
#include <iostream.h>
#include <time.h>

//create a publisher object which we can use
SienaPublisher::SienaPublisher(const char *_host, short _port, const char *_username, const char *_password, ChimeSystemDriver *_System) {

	port = _port;
	strcpy(host, _host);
	strcpy(username, _username);
	strcpy(password, _password);
	lpht = NULL;
	s = NULL;
	System = _System;
	strcpy(Component, "Siena Publisher");
}


//setup the socket we're going to use
bool SienaPublisher::setupSocket() {

	printf("The port we're publishing on is: %d\n", port);
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested) {
		System->ShowError(Component, "Winsock did not Initialize Properly\n");
		return false;
	}
	
	else
	{
		
		lpht = gethostbyname (host);

		if (lpht == NULL) {
			System->ShowError(Component, "Host Not Found", host);
			return false;
		}

		s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
			
		if (s == INVALID_SOCKET) {
			System->ShowError(Component, "Invalid Socket");
			return false;
		}

		saServer.sin_family = AF_INET;
		saServer.sin_addr = *((LPIN_ADDR)*lpht -> h_addr_list);
		saServer.sin_port = htons (port);


	    gethostname(hostname,sizeof(hostname));
		gethostbyname(hostname);
  
		//hostname = inet_ntoa(*((LPIN_ADDR)*lpht -> h_addr_list));
		printf("The hostname is: %s\n", hostname);

		if (connect (s, (LPSOCKADDR) &saServer, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Unable to Connect Socket to Server");
			closesocket (s);
			return false;
		}

		// Create Request 
		char uniqueID[50];
		createUniqueID(uniqueID);

		sprintf (headerString, "senp{version=1 method=\"PUB\" ttl=30 id=%s", uniqueID);
	    return true;
	}
}

//a convenience method for converting an int function to a char function
char* SienaPublisher::getFunction(int func) {
	if (func == -1)
		return NULL;

	//client side methods
	if (func == c_connect)
		return "c_connect";
	else if (func == c_getRoom)
		return "c_getRoom";
	else if (func == c_moveObject) 
		return "c_moveObject";
	else if (func == c_moveUser) 
		return "c_moveUser";
	else if (func == c_enteredRoom)
		return "c_enteredRoom";
	else if (func == c_addObject)
		return "c_addObject";
	else if (func == c_deleteObject)
		return "c_deleteObject";
	else if (func == c_disconnect) 
		return "c_disconnect";
	else if (func == c_leftRoom) 
		return "c_leftRoom";
	else if (func == c_subscribeRoom) 
		return "c_subscribeRoom";
	else if (func == c_unsubscribeRoom) 
		return "c_unsubscribeRoom";

	//server side methods
	else if (func == s_moveObject)
		return "s_moveObject";
	else if (func == s_moveUser)
		return "s_moveUser";
	else if (func == s_addObject)
		return "s_addObject";
	else if (func == s_deleteObject) 
		return "s_deleteObject";
	else if (func == s_changeClass)
		return "s_changeClass";
	else if (func == s_roomInfo)
		return "s_roomInfo";
	else if (func == s_enteredRoom)
		return "s_enteredRoom";
	else if (func == s_leftRoom)
		return "s_leftRoom";
	else
		return NULL;
}

//subscribe to a room
bool SienaPublisher::subscribeRoom(char *room, int option) {


	if (!setupSocket()) {
		System->ShowError(Component, "Can't Setup Socket");
		return false;
	}

	char subscribeString [1000]; 
	char uniqueID[50];
	createUniqueID(uniqueID);


		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
	
		if (option == EVENTS_FOR_ME_ONLY)
			sprintf (subscribeString, "%s username=\"%s\"", subscribeString, username);
		else if (option == EVENTS_NOT_FOR_ME)
			sprintf (subscribeString, "%s username !=\"%s\"", subscribeString, username);

		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Subscription Request");
			closesocket(s);
			return false;
		}

		closesocket(s);
		return true;
}

//subscribe all client events
bool SienaPublisher::subscribeClient() {

		if (!setupSocket()) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}
		
		char subscribeString [1000]; 
		char uniqueID[50];
		createUniqueID(uniqueID);


		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s username=\"%s\"}", subscribeString, username); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Client Subscription Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}

//subscribe to a particular method originating in some room
bool SienaPublisher::subscribeMethod(char *method, char *room, int option) {
		if (!setupSocket()) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}
		
		char subscribeString [1000]; 
		char uniqueID[50];
		createUniqueID(uniqueID);


		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);

		if (option == EVENTS_FOR_ME_ONLY)
			sprintf (subscribeString, "%s username=\"%s\"", subscribeString, username);
		else if (option == EVENTS_NOT_FOR_ME)
			sprintf (subscribeString, "%s username !=\"%s\"", subscribeString, username);
		

		sprintf (subscribeString, "%s address=\"%s\"", subscribeString, room); 
		sprintf (subscribeString, "%s chime_method=\"%s\"}", subscribeString, method);
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Method Subscription Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}

//unsubscribe from a particular room
bool SienaPublisher::unsubscribeRoom(char *room, int option) {
		if (!setupSocket()) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}
		
		
		char subscribeString [1000];
		
		char uniqueID[50];
		createUniqueID(uniqueID);

		// Create Header
		sprintf (subscribeString, "senp{method=\"UNS\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);

		if (option == EVENTS_FOR_ME_ONLY)
			sprintf (subscribeString, "%s username=\"%s\"", subscribeString, username);
		else if (option == EVENTS_NOT_FOR_ME)
			sprintf (subscribeString, "%s username !=\"%s\"", subscribeString, username);		

		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send UnSubscription Request");
			closesocket(s);
			return false;
		}
	
		closesocket (s);
		return true;
}

//unsubscribe all events that are addressed to the client
bool SienaPublisher::unsubscribeClient() {
		if (!setupSocket()) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}
		
		char subscribeString [1000]; 
		char uniqueID[50];
		createUniqueID(uniqueID);


		// Create Header
		sprintf (subscribeString, "senp{method=\"UNS\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());

		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s username=\"%s\"}", subscribeString, username); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Unsubscribe Client");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}


//subscribe to all events of a particular room
bool SienaPublisher::subscribeALL(char *room) {
	if (subscribeMethod("s_moveObject", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_moveUser", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_addObject", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_enteredRoom", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_leftRoom", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_deleteObject", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_changeClass", room, EVENTS_FOR_ANYONE) &&
		subscribeMethod("s_roomInfo", room, EVENTS_FOR_ANYONE))
			return true;
	else 
		return false;
}


//unsubscribe from all events originating in some room
bool SienaPublisher::unsubscribeALL() {
		if (!setupSocket()) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}
				
		char subscribeString [1000]; 
		char uniqueID[50];
		createUniqueID(uniqueID);


		// Create Header
		sprintf (subscribeString, "senp{method=\"BYE\" ttl=30 version=1.1 id=\"%s\"", uniqueID);
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Unsubscribe ALL Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}

//get a unique ID for siena messages
void SienaPublisher::createUniqueID(char *uniqueID) {
	time_t ltime;
    time( &ltime );
   
	sprintf(uniqueID, "%ld492", ltime);
	int rand_num = rand() % 100;
	sprintf(uniqueID, "%s.%d.%s", uniqueID, rand_num, hostname);
}


//method to publish an event onto siena bus
bool SienaPublisher::publish(int function, char *params, char *address, char *prot) {

	char *method = getFunction(function);

	if (!setupSocket()) {
		System->ShowError(Component, "Can't Setup Socket");
		return false;
	}
		
	char publishString [1000]; 

	time_t ltime;

	/* Get UNIX-style time and display as number and string. */
    time( &ltime );
    //printf( "Time in seconds since UTC 1/1/70:\t%ld\n", ltime );
   
	sprintf(publishString, "%sid=\"%ld492", headerString, ltime);
	sprintf(publishString, "%s.0.%s\"}", publishString, hostname);
	
	// Create Event
	sprintf (publishString, "%s event{", publishString);
	sprintf (publishString, "%s auth=\"true\"", publishString);
	sprintf (publishString, "%s from_component=\"client\"", publishString);
	sprintf (publishString, "%s data=\"%s\"", publishString, params);
	sprintf (publishString, "%s auth=\"true\"", publishString);
	sprintf (publishString, "%s username=\"%s\"", publishString, username);
	sprintf (publishString, "%s password=\"%s\"", publishString, password);
	sprintf (publishString, "%s prot=\"%s\"", publishString, prot);
	sprintf (publishString, "%s address=\"%s\"", publishString, address);
	sprintf (publishString, "%s chime_method=\"%s\"", publishString, method);
	sprintf (publishString, "%s access=\"\"}", publishString);
	printf("Sending: %s\n\n", publishString);
	
	if (send (s, publishString, strlen(publishString), 0) == SOCKET_ERROR)
	{
		System->ShowError(Component, "Can't Send Publish Request");
		closesocket(s);
		return false;
	}

	closesocket (s);
	return true;
}


//get the IP address of this machine
char* SienaPublisher::getLocalIP()
{
    char hostname[80];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        cerr << "Error " << WSAGetLastError() <<
                " when getting local host name." << endl;
        return NULL;
    }
    cout << "Host name is " << hostname << "." << endl;

    struct hostent *ip_list = gethostbyname(hostname);
    if (ip_list == 0) {
        cerr << "Bad host lookup." << endl;
        return NULL;
    }

	struct in_addr addr;
    memcpy(&addr, ip_list->h_addr_list[0], sizeof(struct in_addr));
    cout << "Address " << inet_ntoa(addr) << endl;
    
    return inet_ntoa(addr);
}
