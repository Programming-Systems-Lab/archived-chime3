import siena.*;

public class Tester {
    public static void main(String[] args) {
	try {
	    if (args.length < 1) {
		System.err.println("Usage:java Tester [server-uri]");
		System.exit(1);
	    }
	    ThinClient siena;
	    siena = new ThinClient(args[0]);

	    Notification e = new Notification();

	    e.putAttribute("from_component", "client");
	    e.putAttribute("data", "http://www.google.com");
	    e.putAttribute("auth", "true");
	    e.putAttribute("username", "denis");
	    e.putAttribute("password", "denis");
	    e.putAttribute("prot", "HTTP");
	    e.putAttribute("address", "http://www.google.com");
	    e.putAttribute("access", "");
	    e.putAttribute("chime_method", "c_getRoom");

	    System.out.println("publishing " + e.toString());
	    try {
		siena.publish(e);
    } catch (SienaException ex) {
	System.err.println("Siena error:" + ex.toString());
    }
	    System.out.println("shutting down.");
	    siena.shutdown();
	    //Thread.sleep(1000);
	    //System.exit(0);
	} catch (Exception ex) {
	    ex.printStackTrace();
	    System.exit(1);
	}
    }
}
