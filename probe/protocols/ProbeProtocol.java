 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.probe.protocols;
import java.io.*;
import java.lang.*;
import psl.chime.sienautils.*;
import psl.chime.probe.*;

public abstract class ProbeProtocol {

    /**
     * This is a method that must be implemented by all Protocols. This method
     * is responsible for taking an object and calling goPlug
     */
    public abstract boolean processObject();

    /**
     * a method which should be called in order to find a plug that best suits the
     * data which we are retrieving
     * @params: PROTOCOL_NAME - http, etc. - what it can find in the conf.file
     * @params: OBJECT - the object which we are dealing with
     * @params: pass_to_manager - see makeParams()
     */
    public boolean goProbe(String PROTOCOL_NAME, String OBJECT, Object[] pass_to_manager) {
	ProbeManager pm = ProbeManager.getInstance();
	pm.checkDifference(PROTOCOL_NAME, OBJECT,  pass_to_manager);
	return true;
    }

    /**
     * Helper method to make the parameters necessary for the last argument of goProbe
     *  @params: addon - what info was given from the protocol
     *  @params: ds - the stream to the data
     *  @params: value - the length of the data if available (otherwise 0)
     **/
    public Object[] makeParams(SienaObject s, String addon, DataInputStream ds, Long value) {
	System.out.println("Making params");
	int length = value.intValue();

	if (addon == null) {
	    addon = "";
	}

	if (ds == null) {
	    ByteArrayInputStream bs = new ByteArrayInputStream(new byte[1]);
	    ds = new DataInputStream(bs);
	}

	if (length == 0) {
	    System.out.println("The length is: " +  length);
	    value = new Long(0);
	}

	Object[] temp = new Object[4];
	temp[0] = addon;
	temp[1] = ds;
	temp[2] = value;
	temp[3] = s;
	return temp;
    }
}
