package psl.chime.probe;
import java.util.*;

public class ProbeBuffer extends Hashtable {

    public void add(ProbeObject po) {
		put(po.getLocation() + po.getProtocol(), po);
    }

    public ProbeObject get(String location, String protocol) {
		return (ProbeObject) get(location + protocol);
    }

    public Enumeration getEnumeration() {
		return keys();
    }
}
