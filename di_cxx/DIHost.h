#ifndef __DIRECTORY_INTERFACE_HOST__
#define __DIRECTORY_INTERFACE_HOST__


//dependencies
#include "DIType.h"


//predeclare
class DirectoryInterface;


//helper types
typedef unsigned int DIHostID;


//host modes
#define DI_HOST_MODE_LEAF 1
#define DI_HOST_MODE_NORMAL 2
#define DI_HOST_MODE_ZONE 3


//class definition
class DIHost
{
	friend class DirectoryInterface;

	public:
		DIHost( DIHostID *setIdent, DIType *setType, char *setInfo, int length, int setMode );
		~DIHost( void );

		void setHostType( DIType *setType );
		void setHostInfo( char *setInfo, int length );
		void setHostID( DIHostID *setIdent );
		void setHostMode( int setMode );
		
		DIType getHostType( void );
		char *getHostInfo( int *length );
		int getHostMode( void );
		void getHostID( DIHostID *getID );

	private:
		class DIHost *nextType;
		class DIHost *nextID;

		DIHostID *identifier;
		DIType type;
		char *info;
		int infoLength;
		int mode;
};

#endif