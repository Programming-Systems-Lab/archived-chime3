package psl.chime.sienautils;
import siena.*;
import psl.chime.vem.*;

public class VEMSubscriber {

    public HierarchicalDispatcher siena;
    private static VEMSubscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized VEMSubscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new VEMSubscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static synchronized VEMSubscriber getInstance() {
	if (myself == null) {
	    System.err.println("Need Location in order to Start Subscriber - please use other getInstance method");
	    System.exit(1);
        }
        return myself;
    }


    /**
     * Check if the siena server is really running where the
     * user has told it is running
     */
    private VEMSubscriber(String siena_location) {

	Vem V = new Vem ();
	V.VemSetHost (siena_location);
	V.start ();

	}


    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 1) {
	    System.err.println("Usage: VEMSubscriber <server-uri>");
	    System.exit(1);
	}

	VEMSubscriber s = VEMSubscriber.getInstance(args[0]);
    }
}

