#ifndef __ENVIRONMENT_MODELER_ELEMENT__
#define __ENVIRONMENT_MODELER_ELEMENT__

//dependencies
#include "ChimeID.h"


//element definition
class EMElement
{
	public:
		virtual EMElement( ChimeID setID );
		virtual ~EMElement( void );

		void setModel( ChimeID modelID );
	
		class EMElement *hashNext;
		ChimeID elementID;
		ChimeID modelID;
		EMModel *model;
		Coords dimensions;

	private:
		EMElement( void );
}

#endif