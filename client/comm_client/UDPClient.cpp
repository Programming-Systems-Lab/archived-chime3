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

//this will send a message using this UDPClient
void UDPClient::sendMess(char *host, char *function, char *params) {

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
	
	char msg [1000];
	sprintf(msg, "%s(%s)", function, params); 
	if (2 + strlen(msg)>500)
	{
          printf("\nCan't send full messg: Truncating");
	}

	//sprintf(buf, "%s", msg);

	printf("Sending: %s\n", msg);

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
