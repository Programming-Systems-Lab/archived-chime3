#ifndef __ENVIRONMENT_MODELER_ROOM__
#define __ENVIRONMENT_MODELER_ROOM__

//dependencies gotten through the EnvironmentModeler.h file


//room definition
class EMRoom: public EMElement
{
	public:
		virtual EMRoom( ChimeID setID, Coords setDim, ChimeID setModel );
		virtual ~EMRoom( void );

		void addDoor( EMDoor *addDoor );
		void addObject( EMObject *addObject );
		void addAvatar( EMAvatar *addAvatar );

		EMDoor *getDoor( ChimeID doorID );
		EMObject *getObject( ChimeID objectID );
		EMAvatar *getAvatar( ChimeID avatarID );

		void removeDoor( ChimeID doorID );
		void removeDoor( EMDoor *remDoor );
		void removeObject( ChimeID objectID );
		void removeObject( EMObject *remObject );
		void removeAvatar( ChimeID avatarID );
		void removeAvatar( EMAvatar *remAvatar );

		EMDoorContainer *doors;
		EMObject *objects;
		EMAvatar *avatars;
}

#endif