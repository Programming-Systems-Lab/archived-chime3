package psl.chime.sienautils;
import siena.*;
import psl.chime.probe.*;


public class ProbeSubscriber {

    public HierarchicalDispatcher siena;
    private static ProbeSubscriber myself;


    /*
    * This is the method which should be called in order to start a subscriber
    */
    public static synchronized ProbeSubscriber getInstance(String siena_location) {
        if (myself == null) {
            myself = new ProbeSubscriber(siena_location);
        }
        return myself;
    }

    /*
     * This is the method which should be called to get an instance of a Subscriber
     */
    public static synchronized ProbeSubscriber getInstance() {
	if (myself == null) {
	    System.err.println("Need Location in order to Start Probe Subscriber - please use other getInstance method");
	    System.exit(1);
        }
        return myself;
    }


    /**
     * Check if the siena server is really running where the
     * user has told it is running
     */
    private ProbeSubscriber(String siena_location) {

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
		probeSubscriber();

	    //loop forever until we shutdown
	    //just wait
	    while(true) {
			Thread.sleep(100000);
		}

	} catch (Exception e) {
	    e.printStackTrace();
	    System.exit(0);
	}


    }


    /**
     * probe subscriber
     */

    public void probeSubscriber() throws Exception {
	Filter f = new Filter();
		f.addConstraint("auth", Op.EQ, "false");
		f.addConstraint("from_component", Op.EQ, "data_server");
		f.addConstraint("chime_method", Op.EQ, "s_queryFrax");


		System.out.println("subscribing for " + f.toString());
		siena.subscribe(f, new Notifiable() {
			public void notify(Notification e) { alertProbe(new SienaObject(e)); }
			public void notify(Notification [] s) { }
	    });
    }


    /**
     * alert frax when a significant notification has been received
     */

    public void alertProbe(SienaObject s) {
	s.setDispatcher(siena);

	//call something in frax when an event has occurred
	try {
	    //ProbeProtLoader ppl = new ProbeProtLoader();
	    //ppl.runProt(s);
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }



    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 1) {
	    System.err.println("Usage: ProbeSubscriber <server-uri>");
	    System.exit(1);
	}

	//ProbeSubscriber s = ProbeSubscriber.getInstance(args[0]);
    }
}

