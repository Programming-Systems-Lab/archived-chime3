package psl.chime.probe;
import psl.chime.sienautils.*;

public class StartProbeServer {

    public static ProbeBuffer buf;

    public StartProbeServer(String siena_server) {

	buf = new ProbeBuffer();
	SubscribeObject so = new SubscribeObject(siena_server);
	ProbeNotification auth_n = new ProbeNotification(so.getSiena());
	Filter ft = new Filter();
	ft.addConstraint("from_component", SienaObject.CLIENT);
	ft.addConstraint("to_component", SienaObject.FRAX);
	so.alert(ft, auth_n);
    }

    /**
     * Just for testing purposes
     */
    public static void main(String args[]) {
	if (args.length != 1) {
	    System.out.println("Usage: StartProbeServer senp://host:port");
	    System.exit(1);
	}

	new StartProbeServer(args[0]);
    }
}
   
