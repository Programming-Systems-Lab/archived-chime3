 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs;
import java.lang.*;
import java.lang.reflect.*;
import psl.chime.frax.*;
import psl.chime.frax.fraxExceptions.*;

public class PlugStarter {

    public PlugStarter() {
   	}

    /**
     * Call this method to figure out which plug should
     * be used when dealing with this object and run the plug
     **/
    public boolean findAndRunPlug(String object, Object const_param) {
	Object[] args = {const_param};
	findAndRunPlug(object, args);
	return true;
	}

     /**
     * Call this method to figure out which plug should
     * be used when dealing with this object and run the plug
     **/
    public boolean findAndRunPlug(String protocol, String object, Object[] const_param) {
	String plug_class = getExtension(object);
	if (plug_class == null)
	    plug_class = getType(protocol, object);

	//it's probably a directory (this should NEVER be the case)
	if (plug_class == null)
            plug_class = "psl.chime.frax.plugs.gen.GenPlug";

	System.out.println("Loading Plug Class: " +  plug_class);
	FRAXPlugLoader fpl = new FRAXPlugLoader();
	try {
	    fpl.runPlug(plug_class, const_param);
	    System.err.println("***Exiting Plug Starter");
	    return true;

	} catch (ClassNotFoundException e) {
	    System.err.println("The plug class " + plug_class + " was not found:" + e.getMessage());
	    return false;
	} catch (MethodNotFoundException e) {
	    System.err.println("The toXML() method was not found in: " + plug_class + ": " + e.getMessage());
	    return false;
	}
    }


    /**
     * This will work for almost any object except weird ones
     * that don't have extensions like SQL queries
     **/
    private String getExtension(String object) {
	int dot_index = object.lastIndexOf(".");
	String ext = object.substring(dot_index+1, object.length());
	System.out.println("The extension of this source is: " + ext);
	//if no extension
	if (ext.length() == 0)
	    return null;

	try {
	    return ConfigReader.findInPlugConf(ext);

	} catch (EntryNotFoundException e) {
	    return null;
	}
    }

    /**
     * This will work for something which doesn't have an extension
     * like an SQL query (NOT IMPLEMENTED MUCH YET)
     **/

    private String getType(String protocol, String object) {
	if (protocol.trim().compareToIgnoreCase("BSCW") == 0)
            return "psl.chime.frax.plugs.bscw.BSCWPlug";

	if (object.trim().endsWith("/") || object.trim().endsWith("\\"))
		return "psl.chime.frax.plugs.dir.DirPlug";

	return null;
    }


}
