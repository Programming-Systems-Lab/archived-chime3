#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "ClientComm.h"
#include <iostream.h>


UDPServer::UDPServer(int port, ChimeSystemDriver *_nav) {

   Port = port;
   nav = _nav;
    
   //check for proper DLLs
   RetCode = WSAStartup(0x0101, &wsaData);

   if (RetCode != 0)
   {
        printf ("\nUDPServer: Error in DLL initialization %d", RetCode);
        return;
   }


   // Creating Udp Socket
   if ((sock = socket(AF_INET,									// Address family
					   SOCK_DGRAM,								// Socket type
					   IPPROTO_UDP))== INVALID_SOCKET)			// Protocol)) 
   {
       printf ("\nUDPServer: Error creating socket - %d", WSAGetLastError());
       return;
   }

   printf ("Port: %d\n", Port);

   sock_addr.sin_family = AF_INET;
   sock_addr.sin_addr.s_addr = INADDR_ANY;	
   sock_addr.sin_port = htons(Port);

   // Binding the socket to the address
   if (bind(sock, (LPSOCKADDR) &sock_addr, sizeof(sock_addr))==SOCKET_ERROR)
        {
            printf ("\nUDPServer: Error in binding the socket - %d", WSAGetLastError());
            return;
        }

   printf("\n\nListening...\n\n");

}

//parse the response for Navdeep
void UDPServer::parseResponse(char *string) {
	char *method = strtok(string, "\t");
	char *params = strtok(NULL, "\t");

	if (method != NULL && params != NULL) {
		int meth = getMethod(method);
		if (meth != -1)
			nav->GetFunction(meth, params);
	}
}

//get the method as an integer as specified by Navdeep
int UDPServer::getMethod(char *method) {
	
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
	else if (strstr(method, "c_subscribeRoom") != NULL) 
		return c_disconnect;
	else if (strstr(method, "c_unsubscribeRoom") != NULL) 
		return c_disconnect;
	else if (strstr(method, "c_talk") != NULL) 
		return c_talk;

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
		return s_enteredRoom;
	else if (strstr(method, "s_leftRoom") != NULL)
		return s_leftRoom;
	else
		return -1;
}


//this will start a UDPServer
//with a callback to Nav's Class
int UDPServer::startServer() {
  
	char recvString [5000];
	int nLen;
	nLen = sizeof(SOCKADDR);

	while(1) {

			// Wait for data from the client
	
			SOCKADDR_IN saClient;

			memset(recvString, 0, sizeof(recvString));

			if (recvfrom(sock,				// Bound socket
					recvString,					// Receive buffer
					sizeof(recvString),			// Size of buffer in bytes
					0,						// Flags
					(LPSOCKADDR)&saClient,	// Buffer to receive client address 
					&nLen) == SOCKET_ERROR) {				// Length of client address buffer
					
					fprintf (stderr, "\n\nUDPServer Error: Error in Receiving\n\n");
					closesocket (sock);
			}


			if (recvString != NULL) {
				//nav->getFunction(recvString);
				printf ("%s\n\n", recvString);
				parseResponse(recvString);
			}
	}
}

	
