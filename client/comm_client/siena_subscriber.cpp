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

	System = _nav;
	username = _username;
	host = _host;
	port = _port;
	strcpy(Component, "Siena Subscriber");


	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested) {
		System->ShowError(Component, "Winsock did not Initialize Properly\n");
		return;
	}

	else
	{
		LPHOSTENT lpht = gethostbyname (host);

		if (lpht == NULL) {
			System->ShowError(Component, "Host Not Found", host);
			return;
		}

		r = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (r == INVALID_SOCKET) {
			System->ShowError(Component, "Invalid Socket");
			return;
		}

		SOCKADDR_IN saR;

		saR.sin_family      = AF_INET;
		saR.sin_addr.s_addr = INADDR_ANY;
		saR.sin_port        = htons (5000);

		if (bind (r, (LPSOCKADDR) &saR, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Unable to Connect Socket to Server");
			closesocket (r);
			return;
		}

	}

}

bool SienaSubscriber::subscribeRoom(char *room) {

		SOCKET s;
		if ((s = createSendSocket()) != INVALID_SOCKET) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}

		// Create Header
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%s%s", subscribeString, host);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s%d\" ", subscribeString, port);
		sprintf (subscribeString, "%shandler=\"senp://%s:5000\"}", subscribeString, getLocalIP());


		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%s username !=\"%s\"", subscribeString, username);
		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room);
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Subscribe Room Request");
			closesocket(s);
			return false;
		}

		closesocket(s);
		return true;
}

bool SienaSubscriber::subscribeClient() {
		SOCKET s;
		if ((s = createSendSocket()) != INVALID_SOCKET) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}


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
			System->ShowError(Component, "Can't Send Client Subscription Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}


bool SienaSubscriber::subscribeMethod(char *method, bool include_myself) {
		SOCKET s;
		if ((s = createSendSocket()) != INVALID_SOCKET) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}

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

		if (include_myself)
			sprintf (subscribeString, "%s username=\"%s\"", subscribeString, username);
		else
			sprintf (subscribeString, "%s username !=\"%s\"", subscribeString, username);

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

bool SienaSubscriber::unsubscribeRoom(char *room) {
		SOCKET s;
		if ((s = createSendSocket()) != INVALID_SOCKET) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}

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
		sprintf (subscribeString, "%s address=\"%s\"}", subscribeString, room);
		printf("Sending filter request: %s\n\n", subscribeString);

		// Subscribes
		if (send (s, subscribeString, strlen(subscribeString), 0) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Can't Send Room Subscription Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}

bool SienaSubscriber::unsubscribeClient() {
		SOCKET s;
		if ((s = createSendSocket()) != INVALID_SOCKET) {
			System->ShowError(Component, "Can't Setup Socket");
			return false;
		}

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
			System->ShowError(Component, "Can't Send Client Unsubscribe Request");
			closesocket(s);
			return false;
		}

		closesocket (s);
		return true;
}


SOCKET SienaSubscriber::createSendSocket() {
	WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested) {
		System->ShowError(Component, "Winsock did not Initialize Properly");
		return INVALID_SOCKET;
	}

	else
	{
		LPHOSTENT lpht = gethostbyname (host);

		if (lpht == NULL) {
			System->ShowError(Component, "Host Not Found", host);
			return INVALID_SOCKET;
		}


		SOCKET s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (s == INVALID_SOCKET) {
			System->ShowError(Component, "Invalid Socket");
			return INVALID_SOCKET;
		}

		SOCKADDR_IN saS;
		saS.sin_family		= AF_INET;
		saS.sin_addr		= *((LPIN_ADDR)*lpht -> h_addr_list);
		saS.sin_port		= htons (port);

		if (connect (s, (LPSOCKADDR) &saS, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			System->ShowError(Component, "Unable to Connect Socket to Server");
			closesocket (s);
			return INVALID_SOCKET;
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
		System->GetFunction(method, data);
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
	else if (strstr(method, "c_leftRoom") != NULL)
		return c_leftRoom;
	else if (strstr(method, "c_unsubscribeRoom") != NULL)
		return c_unsubscribeRoom;
	else if (strstr(method, "c_subscribeRoom") != NULL)
		return c_subscribeRoom;

	//server side methods
	else if (strstr(method, "s_moveObject") != NULL)
		return s_moveObject;
	else if (strstr(method, "s_moveUser") != NULL)
		return s_moveUser;
	else if (strstr(method, "s_addObject") != NULL)
		return s_addObject;
	else if (strstr(method, "s_deleteObject") != NULL)
		return s_deleteObject;
	else if (strstr(method, "s_changeClass") != NULL)
		return s_changeClass;
	else if (strstr(method, "s_roomInfo") != NULL)
		return s_roomInfo;
	else if (strstr(method, "s_enteredRoom") != NULL)
		return s_roomInfo;
	else if (strstr(method, "s_leftRoom") != NULL)
		return s_roomInfo;
	else
		return -1;
}


bool SienaSubscriber::startServer() {

	//make sure to subscribe the client
	//don't think this is really necessary
	//subscribeClient();

	//let's just subscribe the client to all the methods that he wants
	/*
	subscribeMethod("s_moveObject", true);
	subscribeMethod("s_moveUser", true);
	subscribeMethod("s_addObject", true);
	subscribeMethod("s_enteredRoom", true);
	subscribeMethod("s_leftRoom", true);
	subscribeMethod("s_deleteObject", true);
	subscribeMethod("s_changeClass", true);
	subscribeMethod("s_roomInfo", true);
	*/

	// Infinite loop to keep receiving events for the client

	if (listen (r, SOMAXCONN) == SOCKET_ERROR)
	{
		System->ShowError(Component, "Unable to Listen");
		closesocket (r);
		return false;
	}


	char finalString [999999];
	char recvString [5000];

	while(1)
	{

	// Infinite loop to keep receiving events for the client
	if (listen (r, SOMAXCONN) == SOCKET_ERROR)
	{
		System->ShowError(Component, "Unable to Listen");
		closesocket (r);
		return false;
	}

			SOCKET remoteSocket;

			printf("\n\nListening...\n\n");
			remoteSocket = accept (r, NULL, NULL);

			if (remoteSocket == INVALID_SOCKET)
			{
				System->ShowError(Component, "Unable to Accept Connection");
				closesocket (r);
				return false;
			}


			int length = 1;
			memset(finalString, 0, sizeof(finalString));

		while(length != 0)
		{
				memset(recvString, 0, sizeof(recvString));


				if ((length = recv (remoteSocket, recvString, sizeof(recvString), 0)) == SOCKET_ERROR)
				{
					System->ShowError(Component, "Unable to Receive");
					closesocket (r);
					closesocket (remoteSocket);
					return false;
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

	return true;
}

//get the Local IP of this machine
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
