/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * HtmlParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Directory Parser.
 */


package psl.chime.DataServer;

import com.sun.java.util.collections.List;
import org.jdom.*;
import org.jdom.input.*;


public class HtmlParser implements Parser {
    
    private Document doc;
    private String data;
    
    public HtmlParser(Document arg, String s) {
	doc = arg;
	data = s;
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
	} catch (Exception e) {}
	 
	opt[2] = "";
	int idx1 = 0;
	int idx2 = 0;
	int i=0;
	for (;idx2<data.length(); i++) {
	    idx1 = data.indexOf("<Image>", idx1)+7;
	    idx2 = data.indexOf("</Image>", idx2);
	    if (idx1>=data.length() || idx2>=data.length() 
		|| idx1<=0 || idx2 <=0)
		break;
	    opt[2] += data.substring(idx1,idx2);
	    opt[2] += "\n";
	    idx2 ++;
	}
	
	opt[1] = "" + i;
	idx1 = 0;
	idx2 = 0;
	i=0;
	opt[4] = "";
	for (;idx2<data.length(); i++) {
	    idx1 = data.indexOf("<Link>", idx1)+6;
	    idx2 = data.indexOf("</Link>", idx2);
	    if (idx1>=data.length() || idx2>=data.length() 
		|| idx1<=0 || idx2 <=0)
		break;
	    opt[4] += data.substring(idx1,idx2);
	    opt[4] += "\n";
	    idx2 ++;
	}
	opt[3] = "" + i;
	
	return new SourceTuple(-1, protocol, url, size, type, created, last_mod, src, -1, opt);
    }    
}


