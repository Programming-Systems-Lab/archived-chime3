package psl.chime.sienautils;
import siena.*;
import psl.chime.auth.*;


public class AuthSubscriber {

    public HierarchicalDispatcher siena;
    private static AuthSubscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized AuthSubscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new AuthSubscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static synchronized AuthSubscriber getInstance() {
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
    private AuthSubscriber(String siena_location) {


	Runtime.getRuntime().addShutdownHook(new Thread() {
		public void run() {
		    System.err.println("SLT shutting down");
		    siena.shutdown();
		} });

	try {
	    siena = new HierarchicalDispatcher();
	    if (siena_location != null) {
		siena.setMaster(siena_location);
		subscribe();
	    } else {
		System.err.println("can't set empty siena location");
		throw new SienaException();
	    }
	}catch (Exception e) {
	    System.err.println("Problem init Siena:" + e.getMessage());
	}
    }

    /*
     * Get a Hierarchical Dispatcher
     */
    public HierarchicalDispatcher getDispatcher() {
	return siena;
    }


    /**
     * Setup the subscription filters
     */

    public void subscribe() {

	try {
	    authSubscriber();
	    //loop forever until we shutdown
	    while(true);

	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}


    }

    /**
     * Setup the subscriptions for the authentication server
     */

    public void authSubscriber() throws Exception {
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "true");

	System.out.println("subscribing for " + f.toString());

	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertAuth(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

    }

    /**
     * method which gets called when something appealing to the authenticator
     * is received
     **/
    public void alertAuth(SienaObject s) {
	s.setDispatcher(siena);

	//call the authenticator here
	Authorization auth = new Authorization(siena, s);
    }



    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 1) {
	    System.err.println("Usage: Subscriber <server-uri>");
	    System.exit(1);
	}

	AuthSubscriber s = AuthSubscriber.getInstance(args[0]);
    }
}

