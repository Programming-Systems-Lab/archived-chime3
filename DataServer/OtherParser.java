/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * OtherParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Other Parser.
 */


package psl.chime.DataServer;

import org.jdom.*;
import org.jdom.input.*;


public class OtherParser implements Parser {
    
    private Document doc;
    
    public OtherParser(Document arg) {
	doc = arg;
    }
    
    public SourceTuple parseDoc() {
	String protocol=null;
	String url=null;
	int size=-1;
	String type=null;
	long created=-1;
	long last_mod=-1;
	String src=null;
	String opt[] = null;
	Element root;

	root = doc.getRootElement();
	try {
	    created = root.getAttribute("createDate").getLongValue();
	} catch (Exception e) {}
	protocol = root.getChild("Protocol").getText();
	url = root.getChild("Name").getText();
	type = root.getChild("Type").getText();
	try {
	    size = Integer.parseInt(root.getChild("Size").getText());
	    last_mod = Long.parseLong(root.getChild("Last-Modified").getText());
	} catch (Exception e) {}
	
	System.err.println(url);
	return new SourceTuple(-1, protocol, url, size, type, created, last_mod, src, -1, opt);
    
    }    
}




