package psl.chime.probe;
import java.util.*;

public class ProbeBuffer extends Hashtable {

    public void add(ProbeObject po) {
		put(po.getLocation() + po.getProtocol(), po);
    }


	private String formKey(String protocol, String location) {
		return location + protocol;
	}


    public ProbeObject get(String location, String protocol) {
		return get(formKey(location, protocol));
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
