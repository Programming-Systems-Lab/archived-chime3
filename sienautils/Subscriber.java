package psl.chime.sienautils;
import siena.*;
import psl.chime.frax.*;
import psl.chime.auth.*;

public class Subscriber {

    HierarchicalDispatcher siena;

    /**
     * Check if the siena server is really running where the
     * user has told it is running
     */
    public Subscriber(String siena_location) {
	
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
    

    /** 
     * Setup the subscription filters
     */

    public void subscribe() {

	try {
	    authSubscriber();
	    fraxSubscriber();
	    dataServerSubscriber();
	    vemSubscriber();
	    themeManagerSubscriber();
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
     * frax subscriber 
     */

    public void fraxSubscriber() throws Exception {
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "client");
	//f.addConstraint("from_component", Op.EQ, "data_server");

	
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
	s.setDispatcher(siena);

	//call the method which deals with a VEM event
    }


    /**
     * Theme Subscriber
     */
    
    public void themeManagerSubscriber() throws Exception {
	Filter f = new Filter();
	f.addConstraint("auth", Op.EQ, "false");
	f.addConstraint("from_component", Op.EQ, "vem");
	f.addConstraint("from_component", Op.EQ, "client");
		
	System.out.println("subscribing for " + f.toString());
	siena.subscribe(f, new Notifiable() {
		public void notify(Notification e) { alertTheme(new SienaObject(e)); }
		public void notify(Notification [] s) { } 
	    });
    }

    /**
     * Alert the theme manager to an interesting event
     */

    public void alertTheme(SienaObject s) {
	s.setDispatcher(siena);

	//call the method which deals with an event that is interesting to a theme manager
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
    }

    /**
     * Alert the data server 
     */

    public void alertDataServer(SienaObject s) {
	s.setDispatcher(siena);

	// alert the data server that an interesting event has occurred
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
	
	Subscriber s = new Subscriber(args[0]);
    }
}

