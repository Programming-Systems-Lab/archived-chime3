package psl.chime.sienautils;
import siena.*;
import psl.chime.frax.*;
import psl.chime.auth.*;
import psl.chime.DataServer.*;
import psl.chime.EventTracer.*;
import psl.chime.vem.*;

public class Subscriber {

    public HierarchicalDispatcher siena;
    private static Subscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized Subscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new Subscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static Subscriber getInstance() {
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
    private Subscriber(String siena_location) {
	myself = this;

	//start the VEM
	Vem V = new Vem ();
	V.VemSetHost (siena_location);
	V.start ();

	//start the Data Server and Theme Manager as per Shen's request
	DataServer ds = DataServer.getInstance();

	//start the Event Tracer
	EventTracer ev = EventTracer.getInstance();



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
	    fraxSubscriber();
	    dataServerSubscriber();
	    vemSubscriber();
	    eventTracerSubscriber();
	    //loop forever until we shutdown
	    while(true)
			Thread.sleep(99999999);

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

	System.err.println("Authorization has been called...");
	s.setDispatcher(siena);

	//call the authenticator here
	Authorization auth = new Authorization(siena, s);
    }


    /**
     * frax subscriber
     */

    public void fraxSubscriber() throws Exception {
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "data_server");
	f.addConstraint("chime_method", Op.EQ, "s_queryFrax");


	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertFrax(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });
    }


    /**
     * alert frax when a significant notification has been received
     */

    public void alertFrax(SienaObject s) {

	System.err.println("FRAX has been called...");
	s.setDispatcher(siena);

	//call something in frax when an event has occurred
	try {
	    FRAXProtLoader fpl = new FRAXProtLoader();
	    fpl.runProt(s);
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    /**
     * VEM Subscriber
     */

    public void vemSubscriber() throws Exception {
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "data_server");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertVEM(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });
    }


    /**
     * VEM alert - function which alerts the VEM to an interesting event
     */

    public void alertVEM(SienaObject s) {

	System.err.println("VEM has been called...");
	s.setDispatcher(siena);

	//call the method which deals with a VEM event
    }


    /**
     * Event Tracer
     */

    public void eventTracerSubscriber() throws Exception {
	//subscribe for all message from VEM
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "vem");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_connect
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_connect");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_moveObject
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_moveObject");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_moveUser
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_moveUser");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	   });


	//subscribe for all messages from client where the method is: c_enteredRoom
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_enteredRoom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_subscribeRoom
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_subscribeRoom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_leftRoom
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_leftRoom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_unsubscribeRoom
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_unsubscribeRoom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//subscribe for all messages from client where the method is: c_disconnect
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_disconnect");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertEventPackager(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

    }

    /**
     * Alert the theme manager to an interesting event
     */

    public void alertEventPackager(SienaObject s) {

	System.err.println("Event Tracer has been called...");
	s.setDispatcher(siena);

	//call the method which deals with an event that is interesting to a theme manager
	EventTracer ev = EventTracer.getInstance();
	ev.eventReceived(s);

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


	//the c_getRoom method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_getRoom");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });


	//the c_addObject method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_addObject");

	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertDataServer(new SienaObject(e)); }
		public void notify(Notification [] s) { }
	    });

	//the c_deleteObject method
	f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	f.addConstraint("chime_method", Op.EQ, "c_deleteObject");

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

	System.err.println("Data Server has been called...");
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
	    System.err.println("Usage: Subscriber <server-uri>");
	    System.exit(1);
	}

	Subscriber s = Subscriber.getInstance(args[0]);
    }
}

