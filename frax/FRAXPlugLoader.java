 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax;
import java.lang.Class;
import java.lang.reflect.*;
import psl.chime.frax.fraxExceptions.*;
import psl.chime.frax.gui.*;
import psl.chime.sienautils.*;

public class FRAXPlugLoader {

    /**
     * Run the plug class. This procedure would also publishes the SmartEvent
     * that was created by the plug (this method adds some wrappers to what the
     * plug creates.
     **/
    public synchronized void runPlug(String plug_class, Object[] constructor_args) throws ClassNotFoundException, MethodNotFoundException {

	FRAXClassLoader fcl = new FRAXClassLoader();
	Class plug_CLASS = fcl.loadClass(plug_class);
	String xml_ret = execPlug(fcl, plug_CLASS, constructor_args);

	if (xml_ret == null)
	    throw new MethodNotFoundException();

	//xml_ret = "<?xml version=\"1.0\"?>\n<MetaInfo createDate=\'" + System.currentTimeMillis() + "\'>\n" + xml_ret + "</MetaInfo>\n";
	new SimpleDialog("FRAX SmartEvent", xml_ret);
	System.err.println("^^^^^Publishing to Siena^^^^^");
	//the siena server address is hardcoded here:
	//in future version will be changed
	PublishObject po = new PublishObject("senp://localhost:1111");
	po.submit("FRAX", xml_ret);

    }

    /**
     * Primarily for debugging purposes
     * this is used to print all the objects which are
     * passed to the constructor
     **/

    private void printObjects(Object[] args) {
	System.out.print("The parameters I am passing are: ");
	    for (int j = 0; j < args.length; j++) {
		if (args[j] == null)
		    System.out.print(args[j] + ",");
		else
		    System.out.print(args[j].getClass().getName() + ", ");
	    }
	    System.out.println();
    }


    /**
     * Given a series of arguments for constructor and method execute the
     * toXML() method of that plug. This basically loads the plug and returns the
     * information (MetaData in the form of a SmartEvent) to the callign function
     **/
    private synchronized String execPlug(FRAXClassLoader fcl, Class plug, Object[] const_param)  {


	printObjects(const_param);

	try {

	    Object myinstance = fcl.instantiateClass(plug, const_param);
	    System.out.println("Object loaded was: " + myinstance.getClass());

	    System.out.println(plug.getMethod("toXML", null));
	    Method toXML = plug.getMethod("toXML", null);

	    return (String) toXML.invoke(myinstance, null);

	} catch (NoSuchMethodException e) {
	    System.err.println("The method toXML() doesn't exist: " + e.getMessage());
	    return null;
	} catch (IllegalAccessException e) {
	    System.err.println("The method toXML() is unaccessible: " + e.getMessage());
	    return null;
	} catch (InvocationTargetException e) {
	    System.err.println("the toXml() method has thrown an exception: " + e.getMessage());
	    return null;
	}
    }
}
