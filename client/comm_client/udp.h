#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>

////////////////CLIENT SIDE METHODS///////////////////////
#define c_connect 0				// "server userID password"
#define c_getRoom 1				//	"url"
#define c_moveObject 2			//	"roomUrl objectUrl x y z"
#define c_moveUser 3			//	"roomUrl userID x y z"
#define c_enteredRoom 4			//	"userID newRoomUrl"
#define c_addObject 5			//	"roomUrl objectUrl x y z"
#define c_deleteObject 6		//	"roomUrl objectUrl"
#define c_disconnect 7			//	"userID"


////////////////SERVER SIDE METHODS/////////////////////////
#define s_moveObject 8			//	"roomUrl objectUrl x y z"
#define s_moveUser 9			//	"roomUrl userID	x y z"
#define s_AddObject 10			//	"roomUrl objectUrl x y z"
#define s_deleteObject 11		//	"roomUrl objectUrl"
#define s_changeClass 12		//	"roomUrl objectUrl newClassType newSubClassType new3Dfile new2Dfile"
#define s_roomInfo	13			//	"roomUrl Width Height Length #ofObjects\n
								//	ObjectUrl_1 shape class subClass default x y x \n
								//	ObjectUrl_2 shape class subClass default x y x \n
								//	ObjectUrl_3 shape class subClass default x y x \n
								//	.
								//	.
								//	.
								//	ObjectUrl_n shape class subClass default x y x \n"

//this is an equivalent of an interface you need to override
class NavCallback {
	volatile HANDLE hMutex; // Create a Mutex object

public:
	//just something to test
	NavCallback();
	void GetFunction(int method, char *received);
};


class UDPServer {
   long     RetCode;
   WSADATA  wsaData;
   SOCKET   sock;
   struct   sockaddr_in sock_addr, dst_sock_addr, new_sock_addr;
   int      new_sock_addr_len;
   long     bytes;

   struct  ip_mreq mreq;
   struct  in_addr MCastAddr;

   int     Port;

   char    *UserName;
   char    *MachineName;
   char		*buf;
   char		*Text;
   NavCallback *nav;
   char		*recvString;
   SOCKET	remoteSocket;

   private:
	   void parseResponse(char *string);
	   int getMethod(char *method);

   public:
	   UDPServer(int port, NavCallback *_nav);
	   //start the server
	   int startServer();
	   
};

class UDPClient {
   long     RetCode;
   WSADATA  wsaData;
   SOCKET   sock;
   struct   sockaddr_in sock_addr, dst_sock_addr, new_sock_addr;
   int      new_sock_addr_len;
   long     bytes;

   int     Port;
   LPHOSTENT lpht;

   private:
	   char* getFunction(int method);

   public:
	   UDPClient(int _port);
	   void sendMess(char *IP_addr, int function, char *params);

		//destructor
	   ~UDPClient();
};


class SienaSubscriber {
	char subscribeString [1000]; 
	NavCallback *nav;
	char *username;
	char *host;
	int port;

	SOCKET	r;
	//SOCKADDR_IN saR, saS;

private:
	void formatResponse(char *string);
	char *getField(char *method, char *string);
	int getMethod(char *method);

public:
	SienaSubscriber(char *host, short port, char *_username, NavCallback *_nav);
	void subscribeRoom(char *room);
	void subscribeClient();
	void subscribeMethod(char *method);
	void unsubscribeClient();
	void unsubscribeRoom(char *room);
	void startServer();

private:
	SOCKET createSendSocket();
};

class SienaPublisher {
	LPHOSTENT lpht;
	SOCKET	s;
	SOCKADDR_IN saServer;

	char headerString [1000]; 
	char *username;
	char *password;
	char hostname[1000];
	int port;
	char *host;

private:
	void setupSocket();

public:
	SienaPublisher(char *host, short port, char *username, char *password);
	void publish(int function, char *params, char *address, char *prot);
	char* getFunction(int func);
};


class ClientComm {

   UDPClient *cl;
   UDPServer *uds;
   SienaSubscriber *siena_subscriber;
   SienaPublisher *siena_publisher;
   char *username;
   char *password;

   public:
	   ClientComm(int port, char *SAddress, int Sport, char *_username, char *_password, NavCallback *nav);
	   bool SendUDPFunction(char *ip_addr, int function, char *params) ;
	   bool SendSienaFunction(int function, char *params, char *address, char *prot);

   private:
	  int setLocalIP();
	  bool SendData(int function, char *params);
	   
};




