/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * BscwParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * BSCW Parser.
 */

package xescii;

import com.sun.java.util.collections.List;
import org.jdom.*;
import org.jdom.input.*;


public class BscwParser implements Parser {
    
    private String data;

    public BscwParser(String s) {
	data = s;
    }
    
    public SourceTuple parseDoc() {
	String protocol="http";
	String url=null;
	int size=-1;
	String type="bscw";
	long created=-1;
	long last_mod=-1;
	String src=null;
	String opt[]= new String[SourceTuple.NUM_OPT];
	
	try {
	    int idx1 = 0;
	    int idx2 = 0;
	    idx1 = data.indexOf("createDate='") + 12;
	    idx2 = data.indexOf("'", idx1);
	    created = Long.parseLong(data.substring(idx1,idx2));

	    idx1 = data.indexOf("href=\"") + 6;
	    idx2 = data.indexOf("\"", idx1);
	    url = data.substring(idx1,idx2);

	    idx1 = data.indexOf("<ARTIFACT");
	    idx2 = data.lastIndexOf("</ARTIFACT>")+11;
	    opt[0] = data.substring(idx1,idx2);
	    
	} catch (Exception e) {}
			
	return new SourceTuple(-1, protocol, url, size, type, created, last_mod, src, -1, opt);
    }    
}


