package psl.chime.probe;
import psl.chime.probe.probeExceptions.*;
import siena.*;
import psl.chime.sienautils.*;
import java.util.*;
import java.lang.*;
import java.io.*;
import psl.chime.probe.protocols.http.HTTPProtocol;

public class ProbeManager {

    public int delay = 300000;   	//delay for 5 minutes
    private ProbeBuffer pbuf;		//this is a table of all we are monitoring
    private Hashtable DictatorHash;	//this hash contains information to use for probeObject comparison
	private ProbeProtLoader ppl;	//this is the protocol loader
	private String username;
	private String password;
	private HierarchicalDispatcher siena;
	private static ProbeManager manager;

	public static synchronized ProbeManager getInstance() {
		if (manager == null) {
			System.err.println("Please instantiate the Manager before using it");
			System.exit(1);
			return null;

		} else
			return manager;
		}

    ProbeManager(String username, String password, String siena_location) {
		this.username = username;
		this.password = password;
		pbuf = new ProbeBuffer();
		refreshAllProtocols();
		startSiena(siena_location);
		new ProbeSubscriber(siena).start();   //start subscribing for new events to probe
		ppl = new ProbeProtLoader();
		manager = this;
		runMonitor();
    }

	 /**
	  * Check if the siena server is really running where the
	  * user has told it is running
	  */
	 private void startSiena(String siena_location) {

		Runtime.getRuntime().addShutdownHook(new Thread() {
			public void run() {
			    System.err.println("SLT shutting down");
			    siena.shutdown();
			} });

		try {
		    siena = new HierarchicalDispatcher();
		    if (siena_location != null) {
			siena.setMaster(siena_location);
		    } else {
			System.err.println("can't set empty siena location");
			throw new SienaException();
		    }
		}catch (Exception e) {
		    System.err.println("Problem init Siena:" + e.getMessage());
		}
    }


    //set the delay
    public void setDelay(int d) {
		delay = d;
    }

	//refresh all the protocols
	public void refreshAllProtocols() {
		ConfigReader cfg = new ConfigReader();
		try {
			DictatorHash = cfg.getAllProtocols();
		} catch (EntryNotFoundException e) {
			System.err.println("An error has occurred while reading the config file");
			return;
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
	}


	//check if any differed
	public synchronized void checkDifference(String protocol, String location, Object[] pass_to_manager) {
		System.err.println("Checking for difference...");
		ProbeObject po = pbuf.get(protocol, location);

		if (po == null)
			addToMonitorList(protocol, location, pass_to_manager);

		else {
			//it is already in the hash
			System.err.println("Protocol already in the hash");
			Object cfg_obj = DictatorHash.get(protocol);

			if (cfg_obj == null) {
				System.err.println("Error: Object not in Configuration");
				return;
			}

			ProbeObject latest_obj = new ProbeObject();
			latest_obj.setProtocol(protocol);
			latest_obj.setLocation(location);
			boolean different;

			if (((ConfigObject) cfg_obj).lookAtMetadata()) {
				latest_obj.setMetadata((String) pass_to_manager[0]);
				System.err.println("The metadata is: " + pass_to_manager[0]);
				if (!latest_obj.equals(po, po.COMPARE_METADATA)) { //compare to old object - look at metadata
					System.err.println("The Meta Data of " + location + " has changed");
					different = true;
				}
			}

			if (((ConfigObject) cfg_obj).lookAtData()) {
				try {
					latest_obj.setData((DataInputStream) pass_to_manager[1]);
					if (!latest_obj.equals(po, po.COMPARE_DATA)) { //compare to old object - look at data
						System.err.println("The Data of " + location + " has changed");
						different = true;
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}

			if (((ConfigObject) cfg_obj).lookAtLength()) {
				latest_obj.setLength((Long) pass_to_manager[2]);
				if (!latest_obj.equals(po, po.COMPARE_LENGTH))  //compare to old object - look at length
					System.err.println("The Length of " + location + " has changed");
					different = true;
			}

			po = latest_obj;
		}
	}


	//add to the list of monitored items
	public synchronized void addToMonitorList(String protocol, String location, Object[] pass_to_manager) {

			//if the object is not in the hash
			//ConfigObject cfg_obj = (ConfigObject) DictatorHash.get(protocol);

			//if (cfg_obj == null) {
			//	System.err.println("Error: Object not in Configuration");
			//	return;
			//}

			ProbeObject po = new ProbeObject();
			po.setProtocol(protocol);
			po.setLocation(location);

			//store everything in case rules change
			po.setMetadata((String) pass_to_manager[0]);
			try {
				po.setData((DataInputStream) pass_to_manager[1]);
			} catch (Exception e) {
				e.printStackTrace();
			}

			System.err.println("After set data");

			po.setLength((Long) pass_to_manager[2]);
			pbuf.add(po);

			/*
			//it is already in the buffer so check if the rules have changed
			ConfigObject cfg_obj = dictatorHash.get(protocol);
			if (cfg_obj.lookAtMetadata())
				po.setMetadata(pass_to_manager[0]);
			if (cfg_obj.lookAtData())
				po.setData(pass_to_manager[1]);
			if (cfg_obj.lookAtLength())
				po.setLength(pass_to_manager[2]);
			*/
	}


	//sleep for the set delay
	private void sleep() {
		try {
			Thread.sleep(delay);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

    //run the probes periodically and probe the data sources
    public void runMonitor() {
		System.err.println("Monitor has started");
		long round = 0;
		setDelay(15000);

		while(true) {

			sleep();
			System.err.println("------Checking Data Sources - round: " + round);
			round++;

	    	Enumeration enum =  pbuf.getEnumeration();
	    	ProbeObject obj;

	    	while(enum.hasMoreElements()) {
				String key = (String) enum.nextElement();
				obj = pbuf.get(key);

				try {
					SienaObject s = new SienaObject();
					s.setUsername(username);
					s.setPassword(password);
					s.setDispatcher(siena);
					s.setProtocol(obj.getProtocol());
					s.setAddress(obj.getLocation());
					s.setFromComponent("probe");
					s.setMethod("probe");
					//ppl.runProt(s);
					HTTPProtocol prot = new HTTPProtocol(obj.getLocation(), s);
					prot.processObject();
				} catch (Exception e) {  //catch any class loading exceptions
					e.printStackTrace();
				}
			}
		}
	}

    public static void main(String args[]) {
		 if(args.length != 3) {
			    System.err.println("Usage: ProbeSubscriber <username> <password> <server-uri>");
			    System.exit(1);
			}

		ProbeManager pm = new ProbeManager(args[0], args[1], args[2]);
	}
}





