import siena.*;

public class Tester {

	public static ThinClient siena;

	public static void publishC_getRoom(String username, String method, String args) {
		Notification e = new Notification();

		e.putAttribute("access", "user");
		e.putAttribute("address", "http://www.yahoo.com/");
		e.putAttribute("chime_method", method);
		e.putAttribute("auth", "false");
		e.putAttribute("data", args);
		e.putAttribute("from_component", "event_tracer");
		e.putAttribute("password", username);
		e.putAttribute("prot", "HTTP");
		e.putAttribute("username", username);

		System.out.println("publishing " + e.toString());
		try {
			siena.publish(e);
		} catch (SienaException ex) {
			System.err.println("Siena error:" + ex.toString());
		}
   }


    public static void main(String[] args) {
		try {
		    if (args.length != 4) {
				System.err.println("Usage:java Tester [server-uri] [username] [method] [args]");
				System.exit(1);
			}

	    	siena = new ThinClient(args[0]);

	    	//this is the call
	    	publishC_getRoom(args[1], args[2], args[3]);

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
