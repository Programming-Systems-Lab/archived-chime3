#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"

#include "InfoStorer.h"
#include "comm_client/ClientComm.h"

//this class will probably need a Mutex...
//will keep this in the back of my head for now

//setup internal things
InfoStorer::InfoStorer() {
  //siena port
  siena_port = NULL;

  //chat port to use
  chat_port = NULL;

  //client communications object
  comm_object = NULL;

  //set the ip address of this machine
  SetMyIPAddress(GetLocalIP());

  strcpy(username, "");

  strcpy(password, "");

  strcpy(siena_location, "");

  //create the Mutex object the first time we create this class
  mutex=CreateMutex(NULL,FALSE,NULL); // create a mutex object with no name
}

//get the communication object
ClientComm* InfoStorer::GetCommObject() {
	return comm_object;
}

//set the communication object
void InfoStorer::SetCommObject(ClientComm* comm_object) {
	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);
	
	InfoStorer::comm_object = comm_object;

	ReleaseMutex(mutex);
}

//get the username
void InfoStorer::GetUsername(char *return_me) {
	
	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);


	if (strcmp(username, "") == 0)
		strcpy(return_me, "");
	
	else 
		strcpy(return_me, username);

	ReleaseMutex(mutex);

}

//get the password
void InfoStorer::GetPassword(char *return_me) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);


	if (strcmp(password, "") == 0)
		strcpy(return_me, "");
	
	else 
		strcpy(return_me, password);

	ReleaseMutex(mutex);
}

//return the port used to send chat commands
const int InfoStorer::GetChatPort() {
	int return_me;

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	return_me = chat_port;

	ReleaseMutex(mutex);

	return return_me;
}

//get the hostname of the siena
void InfoStorer::GetSienaLocation(char *return_me) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);


	if (strcmp(siena_location, "") == 0)
		strcpy(return_me, "");
	
	else 
		strcpy(return_me, siena_location);

	ReleaseMutex(mutex);
}

//get the port on which Siena is listening
const int InfoStorer::GetSienaPort() {

	int return_me;

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	return_me = siena_port;

	ReleaseMutex(mutex);

	return return_me;
}

//set the username
void InfoStorer::SetUsername(const char *username) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	strcpy(InfoStorer::username, username);

	ReleaseMutex(mutex);
}

//set the password
void InfoStorer::SetPassword(const char *password) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	strcpy(InfoStorer::password, password);

	ReleaseMutex(mutex);
}

//set the port used to send chat commands
void InfoStorer::SetChatPort(int chat_port) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);
	
	InfoStorer::chat_port = chat_port;

	ReleaseMutex(mutex);
}

//set the hostname of the siena
void InfoStorer::SetSienaLocation(const char *siena_location) {
	
	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	strcpy(InfoStorer::siena_location, siena_location);

	ReleaseMutex(mutex);
}

//set the port on which Siena is listening
void InfoStorer::SetSienaPort(int siena_port) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);
	
	InfoStorer::siena_port = siena_port;

	ReleaseMutex(mutex);
}

//*********************************************************************************
//*
//* get the IP address of this machine
//*
//*********************************************************************************

char* InfoStorer::GetLocalIP()
{

	WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		//just return some bogus IP
        return "129.1.1.1";
    }

    char hostname[80];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        cerr << "Error " << WSAGetLastError() <<
                " when getting local host name." << endl;
		//just return some bogus IP
        return "129.1.1.1";
    }
    cout << "Host name is " << hostname << "." << endl;

    struct hostent *ip_list = gethostbyname(hostname);
    if (ip_list == 0) {
        cerr << "Bad host lookup." << endl;
		//just return some bogus IP
        return "129.1.1.1";
    }

	struct in_addr addr;
    memcpy(&addr, ip_list->h_addr_list[0], sizeof(struct in_addr));
    cout << "Address " << inet_ntoa(addr) << endl;

	WSACleanup();

    return inet_ntoa(addr);
}

//set the IP address of the current machine
void InfoStorer::SetMyIPAddress(char *ip_address) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);

	strcpy(InfoStorer::ip_address, ip_address);

	ReleaseMutex(mutex);
}

//get the IP address of the current machine
void InfoStorer::GetMyIPAddress(char *return_me) {

	//get a lock on the mutex
	WaitForSingleObject(mutex,INFINITE);


	if (strcmp(ip_address, "") == 0)
		strcpy(return_me, "");
	
	else 
		strcpy(return_me, ip_address);

	ReleaseMutex(mutex);
}