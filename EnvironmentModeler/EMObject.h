#ifndef __ENVIRONMENT_MODELER_OBJECT__
#define __ENVIRONMENT_MODELER_OBJECT__


//object definition
class EMObject: public EMElement
{
	public:
		virtual EMObject( ChimeID setID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );
		virtual ~EMObject( void );

		EMRoom *room;
		Coords roomCoords;

		class EMObject *next;
}

#endif