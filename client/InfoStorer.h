class InfoStorer {

//this class will probably need a Mutex...
//will keep this in the back of my head for now
private:
	 //the username
  char *username;
  //the password
  char *password;

  //siena location
  char *siena_location;

  //siena port
  int siena_port;

  //chat port to use
  int chat_port;

  //chat port
  char *port;


public:
	//setup internal things
	InfoStorer::InfoStorer();

	//get the username
	const char* GetUsername();

	//get the password
	const char* GetPassword();

	//return the port used to send chat commands
	const int GetChatPort();

	//get the hostname of the siena
	const char* GetSienaLocation();

	//get the port on which Siena is listening
	const int GetSienaPort();

	//set the username
	void SetUsername(char *username);

	//set the password
	void SetPassword(char *password);

	//set the port used to send chat commands
	void SetChatPort(int chat_port);

	//set the hostname of the siena
	void SetSienaLocation(char *siena_location);

	//set the port on which Siena is listening
	void SetSienaPort(int siena_port);

};
