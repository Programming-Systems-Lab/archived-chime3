#ifndef __DIRECTORY_INTERFACE_BODY__
#define __DIRECTORY_INTERFACE_BODY__

class DIMessageBody
{
	public:
		DIMessageBody( char *msgData, int length );
		~DIMessageBody( void );

		char *getData( int *length );

	private:
		char *data;
		int dataLength;
};

#endif