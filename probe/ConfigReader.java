 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.probe;
import psl.chime.probe.probeExceptions.*;
import java.lang.*;
import java.io.*;
import java.util.*;

public class ConfigReader {

    public static String plug_conf = Constants.CONF_FILE;


    /**
     * Helper method to which returns a string representing the plug
     * class to use in order to deal with the current plug.
     * @params object - the plug to find (for example, html)
     */
    public static String findConf(String object) throws EntryNotFoundException {
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


	public Hashtable getAllProtocols() {
		File cfg = findFile(file);
		InputStream inn = new FileInputStream(cfg);
		BufferedReader in = new BufferedReader(new InputStreamReader(inn));
		String line;
		Hashtable all_objects;
		ConfigObject cfg;

		 while((line = in.readLine()) != null) {

			if (!line.startsWith("//") {

				StringTokenizer st = new StringTokenizer(line, "\t");

				//should be 5 tokens - variable and value
				if (st.countTokens() == 5) {
					cfg.setProtName(st.nextToken().trim());
					cfg.setProtClass(st.nextToken().trim());
					cfg.setLookAtMetadata(st.nextToken().trim());
					cfg.setLookAtData(st.nextToken().trim());
					cfg.setLookAtLength(st.nextToken().trim());
					all_objects.add(cfg);
				 }
			}
		}
		}

		if (inn != null)
			inn.close();

		if (in != null)
			in.close();

		return all_objects;

		} catch (Exception e) {
		    System.err.println("ConfigReader: " + e.getMessage());
		    throw new EntryNotFoundException();
		}

		System.err.println("ConfigReader - EntryNotFound: ");
		throw new EntryNotFoundException();
    }



    /**
     * This method basically opens up an appropriate config file
     * and returns the class of the object which this config file
     * indicates should be loaded
     */
    private static String findInConf(String prot, String file) throws EntryNotFoundException {
	try {
	    File cfg = findFile(file);
	    InputStream inn = new FileInputStream(cfg);
	    BufferedReader in = new BufferedReader(new InputStreamReader(inn));
	    String line;
	    ConfigObject cfg;

	    while((line = in.readLine()) != null) {

			if (!line.startsWith("//") {

				StringTokenizer st = new StringTokenizer(line, "\t");

				//should be 5 tokens - variable and value
				if (st.countTokens() == 5) {
		 		   String read_variable = st.nextToken().trim();

		 		   if (object.compareToIgnoreCase(read_variable) == 0) {
					   cfg.setProtName(read_variable);
					   cfg.setProtClass(st.nextToken().trim());
					   cfg.setLookAtMetadata(st.nextToken().trim());
					   cfg.setLookAtData(st.nextToken().trim());
					   cfg.setLookAtLength(st.nextToken().trim());
					   return cfg;
		 		   }


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
