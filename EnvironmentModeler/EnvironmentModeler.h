#ifndef __CHIME_ENVIRONMENT_MODELER__
#define __CHIME_ENVIRONMENT_MODELER__

//dependencies
#include "EMElement.h"
#include "EMRoom.h"
#include "EMDoor.h"
#include "EMObject.h"
#include "EMAvatar.h"


//environment modeler definition
class EnvironmentModeler
{
	public:
		static void initialize( void );
		static void shutdown( void );

		static void placeRoom( ChimeID setID, Coords setDim, ChimeID setModel );
		static void placeDoor( ChimeID setID, ChimeID roomOne, ChimeID roomTwo,
			Coords setDim, Coords coordsOne, Coords coordsTwo, ChimeID setModel );
		static void placeObjet( ChimeID setID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );
		static void placeAvatar( ChimeID setID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );

		static void updateRoom( ChimeID roomID, Coords setDim, ChimeID setModel );
		static void updateDoor( ChimeID doorID, ChimeID roomOne, ChimeID roomTwo,
			Cords setDim, Coords coordsOne, Coords coordsTwo, ChimeID setModel );
		static void updateObject( ChimeID objectID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );
		static void updateAvatar( ChimeID avatarID, ChimeID setRoom, Coords setDim,
			Coords setCoords, ChimeID setModel );

		static void removeRoom( ChimeID elementID );
		static void removeDoor( ChimeID elementID );
		static void removeObject( ChimeID elementID );
		static void removeAvatar( ChimeID elementID );

		static void moveAvatar( ChimeID avatarID, Coords setCoords );

		static void flush( void );
		static void setCamera( ChimeID roomID, Coords location, Coords focus );
		static void updateModel( ChimeID modelID, char *file );

	private:
		EMModel *modelHash[EM_MODEL_HASH_SIZE];
		EMRoom *roomHash[EM_ROOM_HASH_SIZE];
		EMDoor *doorHash[EM_DOOR_HASH_SIZE];
		EMObject *objectHash[EM_OBJECT_HASH_SIZE];
		EMAvatar *avatarHash[EM_AVATAR_HASH_SIZE];

		Coords cameraLocation;
		Coords cameraFocus;
		EMRoom *cameraRoom;
};

#endif