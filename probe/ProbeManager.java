package psl.chime.probe;

public class ProbeManager {

    public int delay = 300000;   	//delay for 5 minutes
    private ProbeBuffer pbuf;		//this is a table of all we are monitoring
    private Hashtable DictatorHash;	//this hash contains information to use for probeObject comparison
	private ProbeProtLoader ppl;	//this is the protocol loader

    ProbeManager() {
		pbuf = new ProbeBuffer();
		refreshAllProtocols();
		runProbes();
    }

    //set the delay
    public setDelay(int d) {
		delay = d;
    }

	//refresh all the protocols
	public synchronized refreshAllProtocols() {
		ConfigReader cfg = new ConfigReader();
		DictatorHash = cfg.getAllProtocols();
	}


	//check if any differed
	public synchronized void checkDifference(String protocol, String location, Object[] pass_to_manager) {
		ProbeObject po = pbuf.get(protocol, location);

		if (po == null)
			addToMonitorList(protocol, location, pass_to_manager);

		else {
			//it is already in the hash
			ConfigObject cfg_obj = dictatorHash.get(protocol);
			ProbeObject latest_obj = new ProbeObject(protocol, location);
			boolean different;

			if (cfg_obj.lookAtMetadata()) {
				latest_obj.setMetadata(pass_to_manager[0]);
				if (!latest_obj.equals(po, po.COMPARE_METADATA))  //compare to old object - look at metadata
					//po.setMetadata(pass_to_manager[0]);
					different = true;
			}

			if (cfg_obj.lookAtData()) {
				latest_obj.setData(pass_to_manager[1]);
				if (!latest_obj.equals(po, po.COMPARE_DATA))  //compare to old object - look at data
					//po.setData(pass_to_manager[1]);
					different = true;
			}

			if (cfg_obj.lookAtLength()) {
				latest_obj.setLength(pass_to_manager[2]);
				if (!latest_obj.equals(po, po.COMPARE_LENGTH))  //compare to old object - look at length
					//po.setLength(pass_to_manager[2]);
					different = true;
			}

			po = latest_obj;
		}


	//add to the list of monitored items
	public void addToMonitorList(String protocol, String location, Object[] pass_to_manager) {

			//if the object is not in the hash
			ConfigObject cfg_obj = dictatorHash.get(protocol, location);
			po = new ProbeObject(protocol, location);

			//store everything in case rules change
			po.setMetadata(pass_to_manager[0]);
			po.setData(pass_to_manager[1]);
			po.setLength(pass_to_manager[2]);

		} else {

			//it is already in the buffer so check if the rules have changed
			ConfigObject cfg_obj = dictatorHash.get(protocol);
			if (cfg_obj.lookAtMetadata())
				po.setMetadata(pass_to_manager[0]);
			if (cfg_obj.lookAtData())
				po.setData(pass_to_manager[1]);
			if (cfg_obj.lookAtLength())
				po.setLength(pass_to_manager[2]);
		}
	}


	//sleep for the set delay
	private sleep() {
		try {
			Thread.sleep(delay);

		} catch (Exception e) {
			e.printStackTrace();
	 }

    //run the probes periodically and probe the data sources
    public runProbes() {

		while(true) {
			sleep();

	    	Enumeration enum =  pbuf.getEnumeration();
	    	ProbeObject obj;

	    	while(enum.hasMoreElements()) {
				obj = enum.nextElement();
				pcl.runProt(s);
			}
	}

    public static void main(String args[]) {
		ProbeManager pm = new ProbeManager();
	}
}





