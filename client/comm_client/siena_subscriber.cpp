#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include <iostream.h>


//create a subscriber that will create a filter for all packets going to client and having the
//client's username
SienaSubscriber::SienaSubscriber(char *_host, short _port, char *_username, chimeBrowser *_nav) {

	nav = _nav;
	username = _username;
	host = _host;
	port = _port;

	
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested)
		printf ("\n\nError: Winsock did not Initialize Properly\n\n");
	
	else
	{
		LPHOSTENT lpht = gethostbyname (host);

		if (lpht == NULL)
			fprintf (stderr, "\n\nWinsock Error: Host Not Found\n\n");

		r = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (r == INVALID_SOCKET)
			fprintf (stderr, "\n\nWinsock Error: Invalid Socket\n\n");

		SOCKADDR_IN saR;

		saR.sin_family      = AF_INET;
		saR.sin_addr.s_addr = INADDR_ANY;	
		saR.sin_port        = htons (5000);

		if (bind (r, (LPSOCKADDR) &saR, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Bind\n\n");
			closesocket (r);
		}

	}

}

void SienaSubscriber::subscribeRoom(char *room) {

		SOCKET s = createSendSocket();	
		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket(s);
}

void SienaSubscriber::subscribeClient() {
		SOCKET s = createSendSocket();
		


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


void SienaSubscriber::subscribeMethod(char *method) {
		SOCKET s = createSendSocket();
		
		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s username=\"%s\"", subscribeString, username);
		sprintf (subscribeString, "%s chime_method=\"%s\"}", subscribeString, method);
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}

		closesocket (s);	
}

void SienaSubscriber::unsubscribeRoom(char *room) {
		SOCKET s = createSendSocket();

		// Create Header
		sprintf (subscribeString, "senp{method=\"UNS\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room); 
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		}
	
		closesocket (s);
}

void SienaSubscriber::unsubscribeClient() {
		SOCKET s = createSendSocket();

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


SOCKET SienaSubscriber::createSendSocket() {
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested) {
		printf ("\n\nError: Winsock did not Initialize Properly\n\n");
		return NULL;
	}
	
	else
	{
		LPHOSTENT lpht = gethostbyname (host);

		if (lpht == NULL)
			fprintf (stderr, "\n\nWinsock Error: Host Not Found\n\n");

		SOCKET s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (s == INVALID_SOCKET)
			fprintf (stderr, "\n\nWinsock Error: Invalid Socket\n\n");

		SOCKADDR_IN saS;
		saS.sin_family		= AF_INET;
		saS.sin_addr		= *((LPIN_ADDR)*lpht -> h_addr_list);
		saS.sin_port		= htons (port);

		if (connect (s, (LPSOCKADDR) &saS, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Connect\n\n");
			closesocket (s);
		}

		return s;
	}
}

//format the response as something which Navdeep is expecting
void SienaSubscriber::formatResponse(char *string) {

	//char *data = new char[strlen(string)+1];
	char *data = getField("data", string);

	int method = getMethod(string);

	if (data != NULL && method != -1) { 
		nav->GetFunction(method, data);
	} else {
		printf("Response couldn't be formatted properly\n Ignoring....\n");
	}

	//delete[] data;
}


//get a particualr field from the siena string
char *SienaSubscriber::getField(char *field, char *string) {
	char *buf = new char[strlen(string)+1];
	char *data = buf;

	strcpy(data, string);
	data = strstr(data, field);
	
	if (data != NULL)
	{
		data = strstr(data, "\"");
		data = &data[1];
		
		char *end = strstr(data, "\" ");
		if (end == NULL)
			return NULL;

		//printf("The end is: %s", end);
		//printf("The difference is: %d\n", end - data);
		char *final = new char[(end - data) + 1];
		strncpy(final, data, end - data);
		final[(end - data)] = NULL;
		printf("The field \"%s\" is: %s\n", field, final);

		delete [] buf;

		return final;
		//return data;
	}
	else 
	{
		printf("Siena Subscriber Error: Incorrectly formatted string - can't get field\n");
		delete [] data;
		return NULL;
	}
}

//get the method from the siena string
int SienaSubscriber::getMethod(char *string) {

	char *method = getField("chime_method", string);

	if (method == NULL)
		return -1;

	//client side methods
	if (strstr(method, "c_connect") != NULL)
		return c_connect;
	else if (strstr(method, "c_getRoom") != NULL)
		return c_getRoom;
	else if (strstr(method, "c_moveObject") != NULL)
		return c_moveObject;
	else if (strstr(method, "c_moveUser") != NULL) 
		return c_moveUser;
	else if (strstr(method, "c_enteredRoom") != NULL)
		return c_enteredRoom;
	else if (strstr(method, "c_addObject") != NULL)
		return c_addObject;
	else if (strstr(method, "c_deleteObject") != NULL)
		return c_deleteObject;
	else if (strstr(method, "c_disconnect") != NULL) 
		return c_disconnect;

	//server side methods
	else if (strstr(method, "s_moveObject") != NULL)
		return s_moveObject;
	else if (strstr(method, "s_moveUser") != NULL)
		return s_moveUser;
	else if (strstr(method, "s_AddObject") != NULL)
		return s_AddObject;
	else if (strstr(method, "s_deleteObject") != NULL) 
		return s_deleteObject;
	else if (strstr(method, "s_changeClass") != NULL)
		return s_changeClass;
	else if (strstr(method, "s_roomInfo") != NULL)
		return s_roomInfo;
	else
		return -1;
}


void SienaSubscriber::startServer() {

	//make sure to subscribe the client
	//don't think this is really necessary
	//subscribeClient();

	//let's just subscribe the client to all the methods that he wants
	subscribeMethod("s_moveObject");
	subscribeMethod("s_moveUser");
	subscribeMethod("s_addObject");
	subscribeMethod("s_enteredRoom");
	subscribeMethod("s_leftRoom");
	subscribeMethod("s_deleteObject");
	subscribeMethod("s_changeClass");
	subscribeMethod("s_roomInfo");

	// Infinite loop to keep receiving events for the client 
	
	if (listen (r, SOMAXCONN) == SOCKET_ERROR)
	{
		fprintf (stderr, "\n\nWinsock Error: Unable to Listen\n\n");
		closesocket (r);
	}

		
	char finalString [100000];
	char recvString [5000]; 

	while(1)
	{

	// Infinite loop to keep receiving events for the client 
	if (listen (r, SOMAXCONN) == SOCKET_ERROR)
	{
		fprintf (stderr, "\n\nWinsock Error: Unable to Listen\n\n");
		closesocket (r);
	}

			SOCKET remoteSocket;

			printf("\n\nListening...\n\n");
			remoteSocket = accept (r, NULL, NULL);

			if (remoteSocket == INVALID_SOCKET)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Accept\n\n");
				closesocket (r);
			}

		
			int length = 1;
			memset(finalString, 0, sizeof(finalString));
	
		while(length != 0) 
		{
				memset(recvString, 0, sizeof(recvString));

			
				if ((length = recv (remoteSocket, recvString, sizeof(recvString), 0)) == SOCKET_ERROR)
				{
					fprintf (stderr, "\n\nWinsock Error: Unable to Recv\n\n");
					closesocket (r);
					closesocket (remoteSocket);
				}
			

				//concatenate all accepts
				//because Siena never fills out the buffer completely
				//would have done blocking until buffer is full but can't find a siena message
				//terminator to signify message end
				
				
				if (recvString != NULL) 
				{
					recvString[length] = NULL;
					//printf("The receive String is: %s\n", recvString);
					//printf("The final String is: %s\n", finalString);

					//make sure the string received fits in the buffer
					if (strlen(finalString) + strlen(recvString) > sizeof(finalString)) {
						printf("Subscriber Error: Siena Message too long - ignoring");
						*finalString = NULL;
						break;
					}
					else  {
						strncat(finalString, recvString, length);
						printf("The length of this packet is %d", length);
					}
				}
				
		}

		closesocket(remoteSocket);


		if (finalString != NULL) 
		{
			//recvString[length] = NULL;
			//printf("The string is: %s\n", recvString);
			formatResponse(finalString);
			printf ("%s\n\n", finalString);
		}
		
	}
		
	WSACleanup ();

	return;
}


char* SienaSubscriber::getLocalIP()
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