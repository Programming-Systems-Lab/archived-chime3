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

}

ClientComm* InfoStorer::GetCommObject() {
	return comm_object;
}

void InfoStorer::SetCommObject(ClientComm* comm_object) {
	InfoStorer::comm_object = comm_object;
}

//get the username
const char* InfoStorer::GetUsername() {
	return username;
}

//get the password
const char* InfoStorer::GetPassword() {
	return password;
}

//return the port used to send chat commands
const int InfoStorer::GetChatPort() {
	return chat_port;
}

//get the hostname of the siena
const char* InfoStorer::GetSienaLocation() {
	return siena_location;
}

//get the port on which Siena is listening
const int InfoStorer::GetSienaPort() {
	return siena_port;
}

//set the username
void InfoStorer::SetUsername(char *username) {
	strcpy(InfoStorer::username, username);
}

//set the password
void InfoStorer::SetPassword(char *password) {
	strcpy(InfoStorer::password, password);
}

//set the port used to send chat commands
void InfoStorer::SetChatPort(int chat_port) {
	InfoStorer::chat_port = chat_port;
}

//set the hostname of the siena
void InfoStorer::SetSienaLocation(char *siena_location) {
	strcpy(InfoStorer::siena_location, siena_location);
}

//set the port on which Siena is listening
void InfoStorer::SetSienaPort(int siena_port) {
	InfoStorer::siena_port = siena_port;
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
