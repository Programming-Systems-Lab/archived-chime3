import siena.*;

public class Tester {

	public static ThinClient siena;

	public static void publish (String address) {
		Notification e = new Notification();

		e.putAttribute("access", "user");
		e.putAttribute("address", address);
		e.putAttribute("chime_method", "s_queryFrax");
		e.putAttribute("auth", "false");
		e.putAttribute("data", address);
		e.putAttribute("from_component", "data_server");
		e.putAttribute("password", "denis");
		e.putAttribute("prot", "HTTP");
		e.putAttribute("username", "denis");

		System.out.println("publishing " + e.toString());
		try {
			siena.publish(e);
		} catch (SienaException ex) {
			System.err.println("Siena error:" + ex.toString());
		}
   }


    public static void main(String[] args) {
		try {
		    if (args.length != 2) {
				System.err.println("Usage:java Tester [server-uri] [address]");
				System.exit(1);
			}

	    	siena = new ThinClient(args[0]);

	    	//this is the call
	    	publish(args[1]);

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
