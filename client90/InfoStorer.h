#ifndef __INFOSTORER_H_
#define __INFOSTORER_H_

//forward declarations
class ClientComm;

class InfoStorer {

//this class will probably need a Mutex...
//will keep this in the back of my head for now
private:
	 
  //the username
  char username[50];
  
  //the password
  char password[50];

  //siena location
  char siena_location[50];

  //siena port
  int siena_port;

  //chat port to use
  int chat_port;

  //ip_address of this machine
  char ip_address[50];

  //communication object
  ClientComm *comm_object;

  // Create a Mutex object
  volatile HANDLE mutex; 

public:
	//setup internal things
	InfoStorer::InfoStorer();

	//get the client comm object
	ClientComm* GetCommObject();

	//set the communications object
	void SetCommObject(ClientComm *comm_object);

	//get the username
	void GetUsername(char *username);

	//get the password
	void GetPassword(char *password);

	//return the port used to send chat commands
	const int GetChatPort();

	//get the hostname of the siena
	void GetSienaLocation(char *siena_location);

	//get the port on which Siena is listening
	const int GetSienaPort();

	//set the username
	void SetUsername(const char *username);

	//set the password
	void SetPassword(const char *password);

	//set the port used to send chat commands
	void SetChatPort(int chat_port);

	//set the hostname of the siena
	void SetSienaLocation(const char *siena_location);

	//set the port on which Siena is listening
	void SetSienaPort(int siena_port);

	//set my IP address
	void SetMyIPAddress(char *ip_address);

	//get my IP address
	void GetMyIPAddress(char *my_address);

	char* GetLocalIP();
};

#endif