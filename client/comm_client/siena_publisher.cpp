#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include <iostream.h>
#include <time.h>


//create a publisher object which we can use
SienaPublisher::SienaPublisher(char *_host, short _port, char *_username, char *_password) {

	port = _port;
	host = _host;
	username = _username;
	password = _password;

}


//setup the socket we're going to use
void SienaPublisher::setupSocket() {

	printf("The port we're publishing on is: %d\n", port);
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested)
		printf ("\n\nError: Winsock did not Initialize Properly\n\n");
	
	else
	{
		
		lpht = gethostbyname (host);

		if (lpht == NULL)
			fprintf (stderr, "\n\nWinsock Error: Host Not Found\n\n");

		s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
			
		if (s == INVALID_SOCKET)
			fprintf (stderr, "\n\nWinsock Error: Invalid Socket\n\n");

		saServer.sin_family = AF_INET;
		saServer.sin_addr = *((LPIN_ADDR)*lpht -> h_addr_list);
		saServer.sin_port = htons (port);


	    gethostname(hostname,sizeof(hostname));
		gethostbyname(hostname);
  

		//hostname = inet_ntoa(*((LPIN_ADDR)*lpht -> h_addr_list));
		printf("The hostname is: %s\n", hostname);

		if (connect (s, (LPSOCKADDR) &saServer, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Connect\n\n");
			closesocket (s);
		}

		// Create Request 
		sprintf (headerString, "senp{version=1 method=\"PUB\" ttl=30 ");
	
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
void SienaPublisher::subscribeRoom(char *room, int option) {

		setupSocket();
		
		char subscribeString [1000]; 

		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
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
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket(s);
}

//subscribe all client events
void SienaPublisher::subscribeClient() {
		setupSocket();
		
		char subscribeString [1000]; 


		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
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
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket (s);
}

//subscribe to a particular method originating in some room
void SienaPublisher::subscribeMethod(char *method, char *room, int option) {
		setupSocket();
		
		char subscribeString [1000]; 

		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
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
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket (s);	
}

//unsubscribe from a particular room
void SienaPublisher::unsubscribeRoom(char *room, int option) {
		setupSocket();
		
		char subscribeString [1000]; 

		// Create Header
		sprintf (subscribeString, "senp{method=\"UNS\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
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
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}
	
		closesocket (s);
}

//unsubscribe all events that are addressed to the client
void SienaPublisher::unsubscribeClient() {
		setupSocket();
		
		char subscribeString [1000]; 


		// Create Header
		sprintf (subscribeString, "senp{method=\"UNS\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
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
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket (s);
}


//subscribe to all events of a particular room
void SienaPublisher::subscribeALL(char *room) {
	subscribeMethod("s_moveObject", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_moveUser", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_addObject", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_enteredRoom", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_leftRoom", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_deleteObject", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_changeClass", room, EVENTS_FOR_ANYONE);
	subscribeMethod("s_roomInfo", room, EVENTS_FOR_ANYONE);
}


//unsubscribe from all events originating in some room
void SienaPublisher::unsubscribeALL() {
		setupSocket();
		
		char subscribeString [1000]; 


		// Create Header
		sprintf (subscribeString, "senp{method=\"BYE\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());

			// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket (s);
}


//method to publish an event onto siena bus
void SienaPublisher::publish(int function, char *params, char *address, char *prot) {

	char *method = getFunction(function);

	setupSocket();

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
		fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		closesocket (s);
	}

	closesocket (s);
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
