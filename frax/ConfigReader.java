 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax;
import psl.chime.frax.fraxExceptions.*;
import java.lang.*;
import java.io.*;
import java.util.*;

public class ConfigReader {

    public static String plug_conf = Constants.PLUG_CONF_FILE;
    public static String prot_conf = Constants.PROT_CONF_FILE;


    /**
     * Helper method to which returns a string representing the plug
     * class to use in order to deal with the current plug.
     * @params object - the plug to find (for example, html)
     */
    public static String findInPlugConf(String object) throws EntryNotFoundException {
	return findInConf(object, plug_conf);
    }


    /**
     * Helper method which returns a string representing the protocol
     * class to use when dealing with a user specified protocol
     * @params object - the protocol to find like http
     */
    public static String findInProtConf(String object) throws EntryNotFoundException {
	return findInConf(object, prot_conf);
    }


	/**
	 * Try to find the config file in the classpath
	 */
	private static File findFile(String file) {
		String classpath = System.getProperty("java.class.path");
		StringTokenizer st = new StringTokenizer(classpath, File.pathSeparator);

		while (st.hasMoreTokens()) {
			String path = st.nextToken();
			System.out.println(path + File.separatorChar + file);
			File pathf = new File(path + File.separatorChar + file);
			if (pathf.exists()) {
				return pathf;
			}

		}

		return null;
	}

    /**
     * This method basically opens up an appropriate config file
     * and returns the class of the object which this config file
     * indicates should be loaded
     */
    private static String findInConf(String object, String file) throws EntryNotFoundException {
	try {
	    File cfg = findFile(file);
	    InputStream inn = new FileInputStream(cfg);
	    BufferedReader in = new BufferedReader(new InputStreamReader(inn));
	    String line;

	    while((line = in.readLine()) != null) {
		StringTokenizer st = new StringTokenizer(line, "=");

		//should be only 2 tokens - variable and value
		if (st.countTokens() == 2) {
		    String read_variable = st.nextToken().trim();

		    if (object.compareToIgnoreCase(read_variable) == 0) {
			return st.nextToken().trim();
		    }


		}
	    }

	    if (inn != null)
		inn.close();

	    if (in != null)
		in.close();

	} catch (Exception e) {
	    System.err.println("ConfigReader: " + e.getMessage());
	    throw new EntryNotFoundException();
	}
	System.err.println("ConfigReader - EntryNotFound: ");
	throw new EntryNotFoundException();
    }
}
