#ifndef __DIRECTORY_INTERFACE_TYPE__
#define __DIRECTORY_INTERFACE_TYPE__

class DIType
{
	public:
		DIType( char *setType );
		DIType( unsigned int a, unsigned int b, unsigned in c );

		unsigned int a;
		unsigned int b;
		unsigned int c;
};

#endif