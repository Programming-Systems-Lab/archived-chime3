/*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */


package psl.chime.sienautils;
import siena.*;


public class PublishObject {

    HierarchicalDispatcher siena;
    SienaObject object;

    /**
     * publish an object onto siena
     */
    public PublishObject(HierarchicalDispatcher s, SienaObject obj) throws Exception {
	siena = s;
	submit(obj);
    }

    /**
     * publish an object onto siena
     */
    public PublishObject(HierarchicalDispatcher s, Notification e) throws Exception {
	siena = s;
	submit(e);
    }

    /**
     * submit the sienaObject onto the Siena bus
     */
    public void submit(SienaObject obj) throws Exception {
	submit(obj.getNotification());
    }

    /**
     * publish what was generated
     * This is a specific method most of the time you will need to
     * use the method above
     */
    private void submit(Notification e) throws Exception {
	siena.publish(e);
	//System.out.println("publishing " + e.toString());
    }

}
