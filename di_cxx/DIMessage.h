#ifndef __DIRECTORY_INTERFACE_MESSAGE__
#define __DIRECTORY_INTERFACE_MESSAGE__

//dependencies
#include "DIMessageBody.h"
#include "DIType.h"
#include "DIHost.h"


//message definition
class DIMessage
{
	public:
		DIMessage( DIType *setType, DIMessageBody *setBody, DIHost *setSender );
		~DIMessage( void );

		DIType getType( void );
		DIMessageBody *getBody( void );
		DIHost *getSender( void );

	private:
		DIType type;
		DIMessageBody *body;
		DIHost *sender;
};

#endif