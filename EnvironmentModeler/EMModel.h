#ifndef __ENVIRONMENT_MODELER_MODEL__
#define __ENVIRONMENT_MODELER_MODEL__

class EMModel
{
	public:
		EMModel( char *file, ChimeID setID );
		~EMModel( void );

		ChimeID modelID;
		char *modelData;
		unsigned int modelDataLength;

		class EMModel *hashNext;
};

#endif