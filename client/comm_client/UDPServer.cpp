#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"
#include <iostream.h>


UDPServer::UDPServer(int port, NavCallback *_nav) {

   Port = port;
   nav = _nav;
    
   //check for proper DLLs
   RetCode = WSAStartup(0x0101, &wsaData);

   if (RetCode != 0)
   {
        printf ("\nUDPServer: Error in DLL initialization %d", RetCode);
        exit(1);
   }


   // Creating Udp Socket
   if ((sock = socket(AF_INET,									// Address family
					   SOCK_DGRAM,								// Socket type
					   IPPROTO_UDP))== INVALID_SOCKET)			// Protocol)) 
   {
       printf ("\nUDPServer: Error creating socket - %d", WSAGetLastError());
       exit(1);
   }

   printf ("Port: %d\n", Port);

   sock_addr.sin_family = AF_INET;
   sock_addr.sin_addr.s_addr = INADDR_ANY;	
   sock_addr.sin_port = htons(Port);

   // Binding the socket to the address
   if (bind(sock, (LPSOCKADDR) &sock_addr, sizeof(sock_addr))==SOCKET_ERROR)
        {
            printf ("\nUDPServer: Error in binding the socket - %d", WSAGetLastError());
            exit(1);
        }

   printf("\n\nListening...\n\n");

}



//this will start a UDPServer
//with a callback to Nav's Class
int UDPServer::startServer() {
  
	char recvString [1000];
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
				nav->GetSienaFunction(recvString);
				printf ("%s\n\n", recvString);
			}
	}
}

	
