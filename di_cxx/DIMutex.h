#ifndef __DIRECTORY_INTERFACE_MUTEX__
#define __DIRECTORY_INTERFACE_MUTEX__

//mutex definition
class DIMutex
{
	public:
		DIMutex( void );
		~DIMutex( void );

		void lock( void );
		void unlock( void );
		int trylock( void );
}

#endif