/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * RoomParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Room Parser.
 */

package psl.chime.EventTracer;

import com.sun.java.util.collections.List;
import org.jdom.*;
import org.jdom.input.*;


public class RoomParser {
    
    private Document doc;
    private String data;
    
    public RoomParser(Document arg, String s) {
	doc = arg;
	data = s;
    }
    
    public RoomTuple parseDoc() {
	String user=null;
	String url=null;
	Element root;

	root = doc.getRootElement();
	user = root.getChild("User").getText();
	url = root.getChild("Url").getText();
			
	return new RoomTuple(url, user);
    }    
}
