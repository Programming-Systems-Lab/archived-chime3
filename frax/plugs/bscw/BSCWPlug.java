 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs.bscw;

import psl.chime.frax.plugs.FRAXPlug;
import java.lang.*;
import java.io.*;

public class BSCWPlug extends FRAXPlug {

    public String final_XML = "";
    BufferedReader br;

    /**
     * constructor which creates an internal representation
     * of the object
     */
    public BSCWPlug(String toAdd, DataInputStream ds, Long length) {
	br = new BufferedReader(new InputStreamReader(ds));

    }


    /**
     * Since this is already XML - just return it with the info
     * which the protocol was able to obtain
     */
    public String toXML() {
	try {
	    String temp;
	    while((temp = br.readLine()) != null) {
		final_XML = final_XML + temp + "\n";
		System.err.println("Reading BSCW Data");
	    }

	} catch (IOException e) {
	    System.err.println("An IOException has been thrown");
	}

	final_XML =  "<?xml version=\"1.0\"?>\n<MetaInfo createDate=\'" + System.currentTimeMillis() + "\' type = \'bscw\'>\n" + final_XML + "</MetaInfo>\n\n";
	return final_XML;
    }
}

