package psl.chime.probe;
import java.util;

public class ProbeBuffer extends Hashtable {

    public void add(ProbeObject po) {
		put(po.getLocation() + po.getProtocol(), data);
    }

    public Object get(String location, String protocol) {
		return get(location + protocol);
    }

    public Enumeration getEnumeration() {
		return keys();
    }
}
