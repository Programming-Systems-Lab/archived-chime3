/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * XmlParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Directory Parser.
 */

package xescii;

import com.sun.java.util.collections.List;
import org.jdom.*;
import org.jdom.input.*;


public class XmlParser implements Parser {
    
    private Document doc;
    
    public XmlParser(Document arg) {
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
	String opt[]= new String[SourceTuple.NUM_OPT];
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
	
	try {
	    opt[0] = root.getChild("Hidden").getText();
	    List ls = root.getChildren("Image");
	    opt[1] = "" + ls.size();
	    opt[2] = "";
	    for (int i=0; i<ls.size(); i++) {
		opt[2] += ((Element)ls.get(i)).getText();
		opt[2] += "\n";
	    }
	    ls = root.getChildren("Link");
	    opt[3] = "" + ls.size();
	    opt[4] = "";
	    for (int i=0; i<ls.size(); i++) {
		opt[4] += ((Element)ls.get(i)).getText();
		opt[4] += "\n";
	    }
	} catch (Exception e) {}
	
	return new SourceTuple(-1, protocol, url, size, type, created, last_mod, src, -1, opt);
    }    
}


