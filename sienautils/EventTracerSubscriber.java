package psl.chime.sienautils;
import siena.*;
import psl.chime.EventTracer.*;

public class EventTracerSubscriber {

    public HierarchicalDispatcher siena;
    private static EventTracerSubscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized EventTracerSubscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new EventTracerSubscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static synchronized EventTracerSubscriber getInstance() {
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
    private EventTracerSubscriber(String siena_location) {


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
	    eventTracerSubscriber();
	    //loop forever until we shutdown
	    while(true);

	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}


    }



    /**
     * Event Tracer Subscriber
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
	s.setDispatcher(siena);

	//call the method which deals with an event that is interesting to a theme manager
	EventTracer ev = EventTracer.getInstance();
	ev.eventReceived(s);
    }


    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 1) {
	    System.err.println("Usage: EventTracerSubscriber <server-uri>");
	    System.exit(1);
	}

	EventTracerSubscriber s = EventTracerSubscriber.getInstance(args[0]);
    }
}

