/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * LinkTuple.java
 *
 * @author: Shen Li
 * 
 * The definition of a tuple of the links within a data source.
 */

package psl.chime.DataServer;

import java.util.*;
import java.sql.*;

public class LinkTuple implements Tuple {
    
    public static final int NUM_ATTRIB = 6;
    
    // the names of universal attributes
    public static final String[] ATTRIB_NAME = {
	"link", "type", "classtype", "subtype", "shape", "shape2d"};
    
    // variable definitions
    private String link;
    private String type;
    private String classtype;
    private String subtype;
    private String shape;
    private String shape2d;
        
    // constructors
    public LinkTuple (String l, String t, String c, String sub, String s, String s2) {
	link = l;
	type = t;
	classtype = c;
	subtype = sub;
	shape = s;
	shape2d = s2;
    }
    
    public LinkTuple (String l, String t) {
	this (l, t, null, null, null, null);
    }

    public LinkTuple (String l) {
	this (l, null, null, null, null, null);
    }
    
    // parse a result set into a vector of tuples
    public static Vector parseResultSet(ResultSet r) {
	
	String link = null;
	String type = null;
	String classtype = null;
	String subtype = null;	
	String shape = null;
	String shape2d = null;
	Vector v = new Vector();

	try {
	    ResultSetMetaData m=r.getMetaData();
	    int col=m.getColumnCount();
	   
	    while ( r.next() ) {
		//System.err.println("first occurence.");
		for(int i=1;i<=col;i++) {
		    if ( m.getColumnLabel(i).equals("LINK") ) {
			link = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("TYPE") ) {
			type = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("CLASSTYPE") ) {
			classtype = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("SUBTYPE") ) {
			subtype = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("SHAPE") ) {
			shape = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("SHAPE2D") ) {
			shape2d = r.getString(i);
		    } 
		}
		
		//System.err.println("before adding tuple");
		v.add(new LinkTuple(link, type, classtype, subtype, shape, shape2d));
	    }

	} catch(SQLException e) {
	    System.err.println(e);
	}
	
	//System.err.println("vector returned: " + v.size());
	return v;
    }
    
    public String toString() {
	String tmp = "";
	tmp += "LINK: " + link + "\n";
	tmp += "TYPE: " + type + "\n";
	tmp += "CLASSTYPE: " + classtype + "\n";
	tmp += "SUBTYPE: " + subtype + "\n";
	tmp += "SHAPE: " + shape + "\n";
	tmp += "SHAPE2D: " + shape2d + "\n";
	return tmp;
    }
    
    // output an array of tuples to string
    public static String tuplesToString(Vector v) {
	String tmp = "";
	
	if (v==null)
	    return null;
	
	for (int i=0; i<v.size(); i++) {
	    tmp += ((LinkTuple)v.elementAt(i)).toString();
	    tmp += "\n";
	}
	return tmp;
    }

    // accessors
    public String getLink() { return link;}
    public String getType() { return type;}
    public String getClasstype() { return classtype;}
    public String getSubtype() { return subtype;}
    public String getShape() { return shape;}
    public String getShape2D() { return shape2d;}
}



