package psl.chime.sienautils;
import siena.*;
import psl.chime.DataServer.*;

public class DataServerSubscriber {

    public HierarchicalDispatcher siena;
    private static DataServerSubscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized DataServerSubscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new DataServerSubscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static synchronized DataServerSubscriber getInstance() {
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
    private DataServerSubscriber(String siena_location) {

		//start the Data Server and Theme Manager as per Shen's request
		DataServer ds = DataServer.getInstance();

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
	    dataServerSubscriber();

	    //loop forever until we shutdown
	    while(true);

	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}


    }


    /**
     * Data Server subscriber
     */

    public void dataServerSubscriber() throws Exception {

	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "frax");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });


	//the c_getroom method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("method", Op.EQ, "c_getroom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });


	//the c_getroom method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("method", Op.EQ, "c_addObject");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//the c_deleteObject method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("method", Op.EQ, "c_addObject");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });
    }

    /**
     * Alert the data server
     */

    public void alertDataServer(SienaObject s) {
	s.setDispatcher(siena);

	// alert the data server that an interesting event has occurred
	DataServer ds = DataServer.getInstance();
	ds.eventReceived(s);
    }


    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 1) {
	    System.err.println("Usage: DataServerSubscriber <server-uri>");
	    System.exit(1);
	}

	DataServerSubscriber s = DataServerSubscriber.getInstance(args[0]);
    }
}

