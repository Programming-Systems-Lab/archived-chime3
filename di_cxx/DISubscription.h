#ifndef __DIRECTORY_INTERFACE_SUBSCRIPTION__
#define __DIRECTORY_INTERFACE_SUBSCRIPTION__

//no need for dependencies, always included through DIAccessor


//utility classes
class DISubscriber
{
	public:
		DISubscriber( DIEventReceiver *setReceiver );

		class DISubscriber *next;
		DIEventReceiver *receiver;
};


//subscription definition
class DISubscription
{
	public:
		DISubscription( DIType *subType );
		~DISubscription( void );

	private:
		class DISubscription *next;

		DIType type;
		DISubscriber *subscribers;
};

#endif