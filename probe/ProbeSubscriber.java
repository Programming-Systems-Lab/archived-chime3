package psl.chime.probe;
import siena.*;
import psl.chime.probe.probeExceptions.*;
import psl.chime.sienautils.*;

public class ProbeSubscriber extends Thread {

    public HierarchicalDispatcher siena;

	public ProbeSubscriber(HierarchicalDispatcher siena) {
		this.siena = siena;
	}

	public void run() {
		subscribe();
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
			sleep(100000);
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
		f.addConstraint("from_component", Op.EQ, "frax");

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
	 	   ProbeProtLoader ppl = new ProbeProtLoader();
	       ppl.runProt(s);
		} catch (Exception e) {
	    	e.printStackTrace();
		}
    }

}

