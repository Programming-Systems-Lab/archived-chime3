/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * MovementParser.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * Movement Parser.
 */

package psl.chime.EventTracer;

import com.sun.java.util.collections.List;
import org.jdom.*;
import org.jdom.input.*;
import java.lang.*;

public class MovementParser {
    
    private Document doc;
    private String data;
    
    public MovementParser(Document arg, String s) {
	doc = arg;
	data = s;
    }
    
    public MovementTuple parseDoc() {
	String room = null;
	String obj = null;
	double x0 = 0;
	double y0 = 0;
	double z0 = 0;
	double x1 = 0;
	double y1 = 0;
	double z1 = 0;
	Element root;

	root = doc.getRootElement();
	room = root.getChild("Room").getText();
	obj = root.getChild("Obj").getText();
	
	try {
	    x0 = Double.parseDouble(root.getChild("X0").getText());
	    y0 = Double.parseDouble(root.getChild("Y0").getText());
	    z0 = Double.parseDouble(root.getChild("Z0").getText());
	    x1 = Double.parseDouble(root.getChild("X1").getText());
	    y1 = Double.parseDouble(root.getChild("Y1").getText());
	    z1 = Double.parseDouble(root.getChild("Z1").getText());
	} catch (Exception e) {
	    System.err.println(e);
	}

	return new MovementTuple(room, obj, x0, y0, z0, x1, y1, z1);
    }    
}
