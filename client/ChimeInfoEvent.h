#ifndef CHIME_INFO_EVENT
#define CHIME_INFO_EVENT
#define MAX_INFO_SIZE 1024

enum IDENTIFIER {
	VEM_IDENTIFIER,
	CONNECT_IDENTIFIER,
	GET_OBJECT_IDENTIFIER,
	SIENA_WINDOW_IDENTIFIER,
	CHAT_IDENTIFIER,
	HISTORY_IDENTIFIER
};

//this is just a standard chime event number which we will handle
#define CHIME_EVENT 2000

/************************************************************************
/* This is the central class which will be formed/sent when a ChimeEvent occurs
/*
/************************************************************************/
class ChimeInfoEvent {
	private:
		char args[MAX_INFO_SIZE];
		int identifier;
		int num_of_fields;
		bool parser_started;

	public:

		//this will create an event info object with one field to pass
		ChimeInfoEvent(int identifier) {
			ChimeInfoEvent::identifier = identifier;
			strcpy(args, "");
			num_of_fields = 0;
			parser_started = false;
		};


		//Append given token to a string.
		bool AppendToken(const char *token)
		{
			if (strcmp(args, "") == 0)
				strcpy(args, token);

			else {
				strcat(args, " ");
				strcat(args, token);
			}

			num_of_fields++;
			return true;
		};

		//get the first token and write it into to_store_in
		bool getFirstToken(char *to_store_in) {
			parser_started = true;
			strcpy(to_store_in, strtok(args, " "));
			return true;
		}

		//get the next token and write it into to_store_in
		bool getNextToken(char *to_store_in) {
			if (!parser_started) {
				to_store_in = NULL;
				return false;
			}

			strcpy(to_store_in, strtok(NULL, " "));

			if (to_store_in == NULL) {
				parser_started = false;
			}

			return true;
		}

		//get the window that set this object
		int getIdentifier() { return identifier; };

		//get the number of fields
		int getNumOfFields() { return num_of_fields; };
};


#endif

	
