 /*    
 * Copyright (c) 2001: The Trustees of Columbia University 
 *    in the City of New York.  All Rights Reserved.
 *
 */

package psl.chime.frax;
import java.lang.*;

public class FRAX {

    /**
     * Take a set of arguments and run frax
     * this is used primarily for demo purposes
     * params:
     * 1) The protocol to be used
     * 2) The object (a filename, a url, etc.)
     */
    public static void main(String args[]) {
	String MyProtocol = args[0];
	String MyObject = args[1];

	try {
	    FRAXProtLoader fpl = new FRAXProtLoader();
	    fpl.runProt(args[0], args[1]);

	} catch (Exception e) {
	    System.err.println(e.getMessage());
	}

	System.out.println("Done...");
	//System.exit(0);
    }
}
