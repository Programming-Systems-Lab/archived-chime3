 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax;
import java.lang.Class;
import java.lang.reflect.*;
import psl.chime.frax.fraxExceptions.*;

public class FRAXProtLoader {

    /**
     * Load and run the protocol class.
     **/
    public synchronized boolean runProt(String prot, String arg) throws ClassNotFoundException, EntryNotFoundException, MethodNotFoundException {

	String prot_class = ConfigReader.findInProtConf(prot);
	FRAXClassLoader fcl = new FRAXClassLoader();
	Class prot_CLASS = fcl.loadClass(prot_class);
	boolean success = execProt(fcl, prot_CLASS, arg);

	if (success == false)
	    throw new MethodNotFoundException();

	System.out.println("Protocol Ended Successfully? " + success);
	return true;
    }


    /**
     * Run the processObject method of the protocol class loaded. Each
     * protocol class is supposed to extend the class Protocol which
     * requires each protocol class to implement a processObject method
     * which this method invokes. It returns true if the protocol and
     * the plug class has loaded and executed successfully and no
     * exceptions were thrown
     **/
    public synchronized boolean execProt(FRAXClassLoader fcl, Class protocol, Object object) {
	try {

	    Object[] params = {object};
	    System.out.println("The class of this object is: " + object.getClass());

	    Object myinstance = fcl.instantiateClass(protocol, params);
	    System.out.println("Object loaded was: " + myinstance.getClass());

	    System.out.println(protocol.getMethod("processObject", null));
	    Method ProcessObject = protocol.getMethod("processObject", null);
	    ProcessObject.invoke(myinstance, null);
	    return true;

	} catch (NoSuchMethodException e) {
	    System.err.println("The method processObject() doesn't exist: " + e.getMessage());
	    return false;
	} catch (IllegalAccessException e) {
	    System.err.println("The method processObject() is unaccessible: " + e.getMessage());
	    return false;
	} catch (InvocationTargetException e) {
	    System.err.println("the processObject() method has thrown an exception: " + e.getMessage());
	    return false;
	}

    }
}
