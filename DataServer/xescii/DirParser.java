/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * DirParser.java
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


public class DirParser implements Parser {
    
    private Document doc;
    private String data;
    
    public DirParser(Document arg, String s) {
	doc = arg;
	data = s;
    }
    
    public Tuple parseDoc() {
	String protocol=null;
	String url=null;
	int size=-1;
	String type=null;
	long created=-1;
	long last_mod=-1;
	String src=null;
	String opt[]= new String[Tuple.NUM_OPT];
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
	} catch (Exception e) {} 
	    
	opt[2] = "";
	int idx1 = 0;
	int idx2 = 0;
	int i=0;
	for (;idx1<data.length(); i++) {
	    idx1 = data.indexOf("<FILE>", idx1)+6;
	    idx2 = data.indexOf("</FILE>", idx2);
	    if (idx1>=data.length() || idx2>=data.length() 
		|| idx1<=0 || idx2 <=0)
		break;
	    opt[2] += data.substring(idx1,idx2);
	    opt[2] += "\n";
	    idx2 ++;
	}
	opt[1] = "" + i;
		
	return new Tuple(-1, protocol, url, size, type, created, last_mod, src, -1, opt);
    }    
}
