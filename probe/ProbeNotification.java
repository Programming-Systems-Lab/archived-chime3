package psl.chime.auth;
import siena.*;
import java.util.*;
import psl.chime.sienautils.*;
import java.util.*;
import java.io.*;

/**
 * This is a callback class which is called when an object
 * of interest is published on Siena
 */
class ProbeNotification implements Notifiable {
    HierarchicalDispatcher siena;
    Hashtable hash;

    public ProbeNotification(HierarchicalDispatcher s) {
        siena = s;
	readFile("passwd.txt");
    }

    /**
     * Is called when a notification is received from the Siena bus
     */
    public void notify(Notification e) {
          System.out.println("local notifiable: " + e.toString() + "\n\n");
	  SienaObject siena_obj = new SienaObject(e);

	  //insert what should be called once notification has been received
	  ProbeManager.add(siena_obj);
	  
    }


    /**
     * Same as above but when an array of notifications is returned
     */
    public void notify(Notification [] s) {
	System.out.println("Notification received");
	for (int i = 0; i < s.length; i++) {
		notify(s[i]);
	}
    }
    
   
