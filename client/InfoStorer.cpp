#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "InfoStorer.h"

//this class will probably need a Mutex...
//will keep this in the back of my head for now

//setup internal things
InfoStorer::InfoStorer() {
  //the username
  username = NULL;
  //the password
  password = NULL;

  //siena location
  siena_location = NULL;

  //siena port
  siena_port = NULL;

  //chat port to use
  chat_port = NULL;

  //chat port
  port = NULL;
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
	InfoStorer::username = username;
}

//set the password
void InfoStorer::SetPassword(char *password) {
	InfoStorer::password = password;
}

//set the port used to send chat commands
void InfoStorer::SetChatPort(int chat_port) {
	InfoStorer::chat_port = chat_port;
}

//set the hostname of the siena
void InfoStorer::SetSienaLocation(char *siena_location) {
	InfoStorer::siena_location = siena_location;
}

//set the port on which Siena is listening
void InfoStorer::SetSienaPort(int siena_port) {
	InfoStorer::siena_port = siena_port;
}
