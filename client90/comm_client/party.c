/*++

Copyright (c) 1993 Microsoft Corporation

Module Name:

    party.c

Abstract:
    IP Multicast party line to demo the usage of IP Multicast on Microsoft TCP/IP.
    Compile this as a QuickWin application using Large memory model.


Author:

    Azfar Moazzam (AzfarM) 22-May-1994

Environment:

    User

Revision History:

    dd-mmm-yyy <email>

--*/

#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>
#include <string.h>

#define  PARTY_PORT 9999
#define  PARTY_LINE "233.1.1.1"

VOID usage();

main(int argc, char *argv[])

{

   long     RetCode = 0;
   WSADATA  wsaData;
   SOCKET   sock;
   struct   sockaddr_in sock_addr, dst_sock_addr, new_sock_addr;
   int      new_sock_addr_len;
   long     bytes;
   char     *buf;
   char     *Text;

   struct  ip_mreq mreq;
   struct  in_addr MCastAddr;

   char    MCastAddress[16];
   char    LocalAddress[16];
   int     Port;

   char    *UserName;
   char    *MachineName;


   buf = (char *) malloc (300*sizeof(char));
   Text = (char *) malloc (255*sizeof(char));
   UserName = (char *) malloc (255*sizeof(char));
   MachineName = (char *) malloc (16*sizeof(char));

   if (argc<2)
   {
        usage();
        exit(1);
   }

   RetCode = WSAStartup(0x0101, &wsaData);

   if (RetCode != 0)
   {
        printf ("\nError in DLL initialization %d", RetCode);
        return (1);
   }


   // Creating Udp Socket
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
   {
       printf ("\nError creating socket - %d", WSAGetLastError());
       return (1);
   }


   printf ("\nIP Multicast Address [Enter 0 for default]: ");
   scanf ("%s",MCastAddress);

   if (*MCastAddress == '0')
       strcpy (MCastAddress, PARTY_LINE);

   printf ("IP Multicast Address: %s", MCastAddress);

   printf ("\nPort [Enter 0 for default]: ");
   scanf("%d", &Port);

   if (!Port)
       Port = PARTY_PORT;

   printf ("Port: %d", Port);

   printf("\nLocal IP interface: ");
   scanf ("%s",LocalAddress);

   printf ("Local IP Interface: %s", LocalAddress);

   switch (argv[1][1])
   {
       case ('C'):
       case ('c'):

		printf("\nEnter User Name: ");
		scanf ("%s", UserName);

        printf("\nEnter your message (no longer than 255 characters)");
        printf("\nType !EXIT to quit client and !KILL to quit server\n");

        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr (LocalAddress);
        sock_addr.sin_port = 0;

        // Binding the socket to the address
        if (bind(sock, (LPSOCKADDR) &sock_addr, sizeof(sock_addr))==SOCKET_ERROR)
        {
            printf ("\nError in binding the socket - %d", WSAGetLastError());
            WSACleanup();
            return(1);
        }


        // Setting default IP interface for multicast
        MCastAddr.s_addr = inet_addr(LocalAddress);

        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,
                    (char *) &MCastAddr, sizeof(struct in_addr))==-1)
            printf("\nWarning: Could not set Default MCast Interface - %d",
                            WSAGetLastError());

        dst_sock_addr.sin_family = AF_INET;
        dst_sock_addr.sin_addr.s_addr = inet_addr(MCastAddress);
        dst_sock_addr.sin_port = htons(Port);

        while (TRUE)
        {

           printf("\nText: ");
           gets(Text);

           if (!strcmp(Text, "!EXIT"))
               break;

           if (strlen(Text)+strlen(UserName)>300)
           {
               printf("\nPlease enter a smaller message");
               continue;
           }

           if (!strcmp(Text, "!KILL"))
                strcpy(buf, Text);
           else
                sprintf(buf,"%s: %s", UserName, Text);

           if (sendto(sock, buf, strlen(buf), MSG_DONTROUTE,
                       (LPSOCKADDR) &dst_sock_addr,
                       sizeof(dst_sock_addr))==INVALID_SOCKET)
           {
               printf ("\nError in Sending data on the socket - %d",
               WSAGetLastError());
               WSACleanup();
               return(1);
           }
        }

        printf("\nExiting...");

        break;

       case ('S'):
       case ('s'):
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = inet_addr(LocalAddress);
        sock_addr.sin_port = htons(Port);

        // Binding the socket to the address
        if (bind(sock, (LPSOCKADDR) &sock_addr, sizeof(sock_addr))==SOCKET_ERROR)
        {
            printf ("\nError in binding the socket - %d", WSAGetLastError());
            return(1);
        }

        printf("\nWelcome to the IP Multicast Party Line");
        printf("\nParty On... \n\n");

        // Joining a multicast group
        mreq.imr_multiaddr.s_addr = inet_addr(MCastAddress);
        mreq.imr_interface.s_addr = inet_addr (LocalAddress);

        if (setsockopt(sock,
                   IPPROTO_IP,
                   5,
                   (char *) &mreq,
                   sizeof(struct ip_mreq)) == -1)
        {
             printf("\nError %d in joining Multicast Group", WSAGetLastError());
             exit(1);
        }

        while(1)
        {
             buf = (char *) calloc (300,sizeof (char));

             buf[0]='\0';

             new_sock_addr_len = sizeof(new_sock_addr);

             if (((bytes=recvfrom(sock, buf, 300, 0,
                    (LPSOCKADDR) &new_sock_addr,
                    (int *) &new_sock_addr_len)) == INVALID_SOCKET))
             {
                   printf ("\nError in receiving data - %d",
                                           WSAGetLastError());
                   return(1);
             }

             if (!strcmp(buf, "!KILL"))
             {
                printf ("\nSignal received from client. Exiting..");

                // Dropping membership from the Mcast group
                setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                                (char *) &mreq, sizeof(mreq));

                break;
             }

             printf ("%s\n", buf);

             free(buf);

        }

        break;

     default:
        usage();
        WSACleanup();
        exit(1);
   }

   closesocket(sock);

   free (buf);
   free (Text);
   free (MachineName);
   free (UserName);

   WSACleanup();

}



VOID usage()
{
        printf("\nUsage: PARTY -s | -c ");


}

