#ifndef __DIRECTORY_INTERFACE_ZONE__
#define __DIRECTORY_INTERFACE_ZONE__

//predeclare
class DirectoryInterface;


//connection modes
#define DI_MODE_INFRASTRUCTURE 1
#define DI_MODE_LEAF 2


//zone definition
class DIZone
{
	friend class DirectoryInterface;

	public:
		DIZone( char *setAddress, int setPort, int setMode );
		~DIZone( void );

	private:
		class DIZone *next;

		char *address;
		int port;
		int mode;

		elvin_handle_t elvinHandle;
}

#endif