package di_java;

public class DirectoryInterface
{
    public static void initialize()
    {
	
    }

    public static void shutdown()
    {

    }

    public static void connect( String address, int port, int mode )
    {

    }

    public static void disconnect( DIZone disZone )
    {

    }

    public static void update( DIType hostType, String metainfo, int length )
    {
	
    }

    public static void search( DIEventReceiver receiver, DIType searchType )
    {

    }

    public static void subscribe( DIEventReceiver receiver, DIType subType )
    {

    }

    public static void unsubscribe( DIEventReceiver receiver, DIType subType )
    {

    }

    public static void publish( DIType msgType, DIMessageBody msgBody )
    {

    }

    public static void communicate( DIHost comHost, DIType msgType,
				    DIMessageBody msgBody )
    {

    }

    public static void promote()
    {

    }

    public static void demote()
    {

    }
}
