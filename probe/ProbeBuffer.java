package psl.chime.probe;
import java.util.*;

public class ProbeBuffer extends Hashtable {

    public void add(ProbeObject po) {
		put(formKey(po.getProtocol(), po.getLocation()), po);
    }


	private String formKey(String protocol, String location) {
		return protocol+location;
	}


    public ProbeObject get(String protocol, String location) {
		return get(formKey(protocol, location));
	}

	public ProbeObject get(String key) {
		Object obj = super.get(key);
		if (obj == null)
			return null;
		else
			return (ProbeObject) obj;
	}

    public Enumeration getEnumeration() {
		return keys();
    }
}
