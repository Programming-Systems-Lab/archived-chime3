#ifndef __CHIME4_DIRECTORY_INTERFACE__
#define __CHIME4_DIRECTORY_INTERFACE_ACCESSOR__

//dependencies
#include "DIBase.h"
#include "DIType.h"
#include "DIEvent.h"
#include "DIMessage.h"
#include "DIHost.h"
#include "DIZone.h"
#include "DISubscription.h"


//hash stuff
#define DI_NODE_TYPE_HASH_SIZE
#define DI_NODE_ID_HASH_SIZE
#define DI_SUBSCRIPTION_HASH_SIZE
#define DI_SEARCH_HASH_SIZE


//event types



//event receiver abc
class DIEventReceiver
{
	public:
		virtual void receiveEvent( DIEvent *event ) = 0;
		virtual void receiveResult( DIHost *result ) = 0;
		virtual void receiveMessage( DIHost *result ) = 0;
};


//accessor class
class DirectoryInterface
{
	public:
		static void initialize( DIEventReceiver *setMaster );
		static void shutdown( void );

		static void connect( char *address, int port, int mode );
		static void disconnect( DIZone *disZone );
		
		static void update( DIType *hostType, char *metaInfo, int length );
		static void search( DIEventReceiver *receiver, DIType *searchType );

		static void subscribe( DIEventReceiver *receiver, DIType *subType );
		static void unsubscribe( DIEventReceiver *receiver, DIType *subType );
		static void publish( DIType *msgType, DIMessageBody *msgBody );

		static void communicate( DIHost *comHost, DIType *msgType, DIMessageBody *msgBody );

		static void promote( void );
		static void demote( void );


	private:
		//elvin layer
		static void receiveLocalMessage(	elvin_handle_t handle,
											elvin_subscription_t sub,
											elvin_notification_t notif,
											int is_secure,
											void *rock,
											elvin_error_t error);

		static void receiveZoneMessage(		elvin_handle_t handle,
											elvin_subscription_t sub,
											elvin_notification_t notif,
											int is_secure,
											void *rock,
											elvin_error_t error);

		DIZone *zones;

		DIHost *nodeTypeHash[DI_NODE_TYPE_HASH_SIZE];
		DIHost *nodeIDHash[DI_NODE_ID_HASH_SIZE];

		DISubscription *subHash[DI_SUBSCRIPTION_HASH_SIZE];
		DISubscription *searchHash[DI_SEARCH_HASH_SIZE];


		static DIEventReceiver *master;
};


//elvin receiver


#endif