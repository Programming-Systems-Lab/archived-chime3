#ifndef __ENVIRONMENT_MODELER_AVATAR__
#define __ENVIRONMENT_MODELER_AVATAR__


//avatar definition
class EMAvatar: public EMElement
{
	public:
		virtual EMAvatar( ChimeID setID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );
		virtual ~EMAvatar( void );

		EMRoom *room;
		Coords roomCoords;

		class EMAvatar *next;
}

#endif