#include <string>
#include <exception>
#include <winsock.h>
 
int main (int argc, char* argv [])
{
    WORD wVersionRequested = MAKEWORD (2,2); // Version of Winsock that is used <2.2>
	WSADATA wsaData;
	int nRet;

	nRet = WSAStartup (wVersionRequested, &wsaData); // Initializes Winsock

	// Checks for the correct version of winsock
	if (wsaData.wVersion != wVersionRequested)
		printf ("\n\nError: Winsock did not Initialize Properly\n\n");
	else
	{
		LPHOSTENT lpht;
		SOCKET	r, s;
		SOCKADDR_IN saR, saS;

		lpht = gethostbyname ("localhost");

		if (lpht == NULL)
			fprintf (stderr, "\n\nWinsock Error: Host Not Found\n\n");

		s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
		r = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if ((s == INVALID_SOCKET) || (r == INVALID_SOCKET))
			fprintf (stderr, "\n\nWinsock Error: Invalid Socket\n\n");

		saS.sin_family = AF_INET;
		saS.sin_addr   = *((LPIN_ADDR)*lpht -> h_addr_list);
		saS.sin_port   = htons (1111);

		saR.sin_family      = AF_INET;
		saR.sin_addr.s_addr = INADDR_ANY;	
		saR.sin_port        = htons (5000);

		if (bind (r, (LPSOCKADDR) &saR, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Bind\n\n");
			closesocket (r);
		}

		if (connect (s, (LPSOCKADDR) &saS, sizeof (struct sockaddr)) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Connect\n\n");
			closesocket (s);
		}

		char subscribeString [500]; 

		// Create Request 
		sprintf (subscribeString, "senp{method=\"SUB\" ttl=30 version=1.1 id=\"randomnum.0.dez\" ");
		sprintf (subscribeString, "%sto=\"senp://", subscribeString);
		sprintf (subscribeString, "%slocalhost", subscribeString);
		sprintf (subscribeString, "%s:", subscribeString);
		sprintf (subscribeString, "%s1111\" ", subscribeString);
		sprintf (subscribeString, "%shandler=\"senp://localhost:5000\"}", subscribeString);

		// Create Filter
		sprintf (subscribeString, "%s filter{", subscribeString);
		sprintf (subscribeString, "%scomponent=\"VEM\"}", subscribeString);
		
		// Subscribes
		if (send (s, subscribeString, 500, 0) == SOCKET_ERROR)
		{
			fprintf (stderr, "\n\nWinsock Error: Unable to Send\n\n");
			closesocket (s);
		}

		closesocket (s);

		// Infinite loop to keep receiving events of type "VEM" 
		for (;;)
		{
			if (listen (r, SOMAXCONN) == SOCKET_ERROR)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Listen\n\n");
				closesocket (r);
			}

			SOCKET	remoteSocket;

			remoteSocket = accept (r, NULL, NULL);

			if (remoteSocket == INVALID_SOCKET)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Accept\n\n");
				closesocket (r);
			}

			char recvString [500]; 
			
			if (recv (remoteSocket, recvString, 500, 0) == SOCKET_ERROR)
			{
				fprintf (stderr, "\n\nWinsock Error: Unable to Recv\n\n");
				closesocket (r);
				closesocket (remoteSocket);
			}

			if (recvString != NULL) 
				printf ("%s\n\n", recvString);
		}
	}

	WSACleanup ();

	return 0;
}