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

    public static String config_file = "probe.cfg";


    /**
     * Helper method to which returns a string representing the
     * class to use in order to deal with the current protocol.
     * @params object - the plug to find (for example, html)
     */
    public static ConfigObject findInProbeConf(String object) throws EntryNotFoundException {
	return findInConf(object, config_file);
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


	public Hashtable getAllProtocols() throws EntryNotFoundException {
	try {
		File cfg = findFile(config_file);
		InputStream inn = new FileInputStream(cfg);
		BufferedReader in = new BufferedReader(new InputStreamReader(inn));
		String line;
		Hashtable all_objects = new Hashtable();

		 while((line = in.readLine()) != null) {

			if (!line.startsWith("//")) {

				StringTokenizer st = new StringTokenizer(line, "\t");

				//should be 5 tokens - variable and value
				if (st.countTokens() == 5) {
					ConfigObject cfg_obj = new ConfigObject();
					cfg_obj.setProtocol(st.nextToken().trim());
					cfg_obj.setClassname(st.nextToken().trim());
					cfg_obj.setLookAtMetadata(st.nextToken().trim());
					cfg_obj.setLookAtData(st.nextToken().trim());
					cfg_obj.setLookAtLength(st.nextToken().trim());
					all_objects.put(cfg_obj.getProtocol(), cfg_obj);
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
    }



    /**
     * This method basically opens up an appropriate config file
     * and returns the class of the object which this config file
     * indicates should be loaded
     */
    private static ConfigObject findInConf(String prot, String file) throws EntryNotFoundException {
	try {
	    File cfg_file = findFile(file);
	    InputStream inn = new FileInputStream(cfg_file);
	    BufferedReader in = new BufferedReader(new InputStreamReader(inn));
	    String line;


	    while((line = in.readLine()) != null) {

			if (!line.startsWith("//")) {

				StringTokenizer st = new StringTokenizer(line, "\t");

				//should be 5 tokens - variable and value
				if (st.countTokens() == 5) {
		 		   String read_variable = st.nextToken().trim();

		 		   if (prot.compareToIgnoreCase(read_variable) == 0) {
					   ConfigObject cfg = new ConfigObject();
					   cfg.setProtocol(read_variable);
					   cfg.setClassname(st.nextToken().trim());
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
