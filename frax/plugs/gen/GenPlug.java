 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax.plugs.gen;

import psl.chime.frax.plugs.FRAXPlug;
import java.lang.*;
import java.io.*;

public class GenPlug extends FRAXPlug {

    public String final_XML;

    /**
     * Wraps <other> around the info given by the protocol
     * no other information is available because an appropriate
     * plug can't be found
     */
    public GenPlug(String toAdd, DataInputStream ds, Long length) {
	final_XML =  "<?xml version=\"1.0\"?>\n<MetaInfo createDate=\'" + System.currentTimeMillis() + "\' type = \'other\'>\n" + toAdd + "</MetaInfo>\n\n";
    }

    /**
     * returns the XML
     */
    public String toXML() {
	return final_XML;
    }
}

