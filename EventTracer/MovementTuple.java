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


public class MovementTuple {
    
    public static final int NUM_ATTRIB = 8; // number of total attributes
    
    // the names of universal attributes
    public static final String[] ATTRIB_NAME = {
	"room", "obj", "x0", "y0", "z0", "x1", "y1", "z1"};
    
    // variable definitions
    private String room;
    private String obj;
    private double x0;
    private double y0;
    private double z0;
    private double x1;
    private double y1;
    private double z1;
    
    // constructors
    public MovementTuple (String rm, String o, double ax0, double ay0, double az0, double ax1, double ay1, double az1) {
	room = rm;
	obj = o;
	x0 = ax0;
	y0 = ay0;
	z0 = az0;
	x1 = ax1;
	y1 = ay1;
	z1 = az1;
    }
    
    // parse a result set into a vector of tuples
    public static Vector parseResultSet(ResultSet r) {
	
	String room = null;
	String obj = null;
	double x0 = 0;
	double y0 = 0;
	double z0 = 0;
	double x1 = 0;
	double y1 = 0;
	double z1 = 0;
	
	Vector v = new Vector();

	try {
	    ResultSetMetaData m=r.getMetaData();
	    int col=m.getColumnCount();
	   
	    while ( r.next() ) {
		//System.err.println("first occurence.");
		for(int i=1;i<=col;i++) {
		    if ( m.getColumnLabel(i).equals("ROOM") ) {
			room = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("OBJ") ) {
			obj = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("X0") ) {
			x0 = r.getDouble(i);
		    } else if ( m.getColumnLabel(i).equals("Y0") ) {
			y0 = r.getDouble(i);
		    } else if ( m.getColumnLabel(i).equals("Z0") ) {
			z0 = r.getDouble(i);
		    } else if ( m.getColumnLabel(i).equals("X1") ) {
			x1 = r.getDouble(i);
		    } else if ( m.getColumnLabel(i).equals("Y1") ) {
			y1 = r.getDouble(i);
		    } else if ( m.getColumnLabel(i).equals("Z1") ) {
			z1 = r.getDouble(i);
		    }
		}

		//System.err.println("before adding tuple");
		v.add(new MovementTuple(room, obj, x0, y0, z0, x1, y1, z1));
	    }
	    
	} catch(SQLException e) {
	    System.err.println(e);
	}
	
	//System.err.println("vector returned: " + v.size());
	return v;
    }
    
    public String toString() {
	String tmp = "";
	tmp += "ROOM: " + room + "\n";
	tmp += "OBJ: " + obj + "\n";
	tmp += "X0: " + x0 + "\n";
	tmp += "Y0: " + y0 + "\n";
	tmp += "Z0: " + z0 + "\n";
	tmp += "X1: " + x1 + "\n";
	tmp += "Y1: " + y1 + "\n";
	tmp += "Z1: " + z1 + "\n";
		
	return tmp;
    }
    
    // output an array of tuples to string
    public static String tuplesToString(Vector v) {
	String tmp = "";
	
	if (v==null)
	    return null;
	
	for (int i=0; i<v.size(); i++) {
	    tmp += ((MovementTuple)v.elementAt(i)).toString();
	    tmp += "\n";
	}
	return tmp;
    }

    // accessors
    public String getRoom() { return room;}
    public String getObj() { return obj;}
    public double getX0() { return x0;}
    public double getY0() { return y0;}
    public double getZ0() { return z0;}
    public double getX1() { return x1;}
    public double getY1() { return y1;}
    public double getZ1() { return z1;}
}



