package psl.chime.probe;
import siena.*;

public class ProbeEvent {

	HierarchicalDispatcher siena;

	public ProbeEvent (HierarchicalDispatcher siena) {
		this.siena = siena;
	}

	public void publish(String message) {
		Notification e = new Notification();

		e.putAttribute("error:", message);

		System.out.println("publishing " + e.toString());
		try {
			siena.publish(e);
		} catch (SienaException ex) {
			System.err.println("Siena error:" + ex.toString());
		}
   }
}
