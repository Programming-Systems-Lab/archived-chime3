package psl.chime.auth;
import psl.chime.sienautils.*;

public class StartAuthServer {

    public StartAuthServer(String siena_server) {

	SubscribeObject so = new SubscribeObject(siena_server);
	AuthNotification auth_n = new AuthNotification(so.getSiena());
	so.alertAuthenticator(auth_n);
    }

    /**
     * Just for testing purposes
     */
    public static void main(String args[]) {
	if (args.length != 1) {
	    System.out.println("Usage: StartAuthServer senp://host:port");
	    System.exit(1);
	}

	new StartAuthServer(args[0]);
    }
}
   
