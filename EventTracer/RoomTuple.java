/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * RoomTuple.java
 *
 * @author: Shen Li
 * 
 * The definition of a tuple of rooms.
 */

package psl.chime.EventTracer;

import java.util.*;
import java.sql.*;

public class RoomTuple {
    
    public static final int NUM_ATTRIB = 2; // number of total attributes
    
    // the names of universal attributes
    public static final String[] ATTRIB_NAME = {
	"url", "user"};
    
    // variable definitions
    private String user;
    private String url;
    
    // constructors
    public RoomTuple (String ul, String ur) {
	
	url = ul;
	user = ur;
    }
    
    // parse a result set into a vector of tuples
    public static Vector parseResultSet(ResultSet r) {
	
	RoomTuple[] tmp = null;
	String user = null;
	String url = null;
	
	Vector v = new Vector();

	try {
	    ResultSetMetaData m=r.getMetaData();
	    int col=m.getColumnCount();
	   
	    while ( r.next() ) {
		//System.err.println("first occurence.");
		for(int i=1;i<=col;i++) {
		    if ( m.getColumnLabel(i).equals("URL") ) {
			url = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("USER") ) {
			user = r.getString(i);
		    }
		}
		
		//System.err.println("before adding tuple");
		v.add(new RoomTuple(url, user));
	    }
	    
	} catch(SQLException e) {
	    System.err.println(e);
	}
	
	//System.err.println("vector returned: " + v.size());
	return v;
    }
    
    public String toString() {
	String tmp = "";
	tmp += "URL: " + url + "\n";
	tmp += "USER: " + user + "\n";
	
	return tmp;
    }
    
    // output an array of tuples to string
    public static String tuplesToString(Vector v) {
	String tmp = "";
	
	if (v==null)
	    return null;
	
	for (int i=0; i<v.size(); i++) {
	    tmp += ((RoomTuple)v.elementAt(i)).toString();
	    tmp += "\n";
	}
	return tmp;
    }

    // accessors
    public String getUrl() { return url;}
    public String getUser() { return user;}
}



