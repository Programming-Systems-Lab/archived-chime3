
#include "cssysdef.h"
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include <iostream.h>


//this will just setup the socket for communication
UDPClient::UDPClient(int _port) {
 
   Port = _port;
 
}


//a convenience method for converting an int function to a char function
char* UDPClient::getFunction(int func) {
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

	//server side methods
	else if (func == s_moveObject)
		return "s_moveObject";
	else if (func == s_moveUser)
		return "s_moveUser";
	else if (func == s_AddObject)
		return "s_AddObject";
	else if (func == s_deleteObject) 
		return "s_deleteObject";
	else if (func == s_changeClass)
		return "s_changeClass";
	else if (func == s_roomInfo)
		return "s_roomInfo";
	else
		return NULL;
}

//this will send a message using this UDPClient
void UDPClient::sendMess(char *host, int func, char *params) {

	char *function = getFunction(func);

	//check for proper DLLs
   RetCode = WSAStartup(0x0101, &wsaData);

   if (RetCode != 0)
   {
        printf ("\nError in DLL initialization %d", RetCode);
        return;
   }


   lpht = gethostbyname (host);

   // Creating Udp Socket
   if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
   {
       printf ("\nUDPClient:Error creating socket - %d", WSAGetLastError());
       return;
   }

   printf ("Port: %d\n", Port);
 
   sock_addr.sin_family = AF_INET;
   sock_addr.sin_addr = *((LPIN_ADDR)*lpht -> h_addr_list);
   sock_addr.sin_port = htons (Port);


	if (connect (sock, (LPSOCKADDR) &sock_addr, sizeof (struct sockaddr)) == SOCKET_ERROR)
	{
		fprintf (stderr, "\n\nWinsock Error: Unable to Connect\n\n");
		closesocket (sock);
	}
	
	char msg [5000];

	//form the message
	sprintf(msg, "%s\t%s\t", function, params); 

	if (2 + strlen(msg)>500)
	{
          printf("\nCan't send full messg: Truncating");
	}

	//sprintf(buf, "%s", msg);

	printf("Sending message: %s\n", msg);

	if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
		fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
		closesocket (sock);
	}

	//free(msg);
}

UDPClient::~UDPClient() {
	closesocket(sock);
	WSACleanup();
}
