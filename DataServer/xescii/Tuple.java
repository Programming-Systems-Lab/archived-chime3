/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * Tuple.java
 *
 * @author: Shen Li
 * 
 * The definition of a tuple in the database of the data server.
 * Number of attributes can be modified.
 */

package xescii;

import java.util.*;
import java.sql.*;

public class Tuple {
    
    public static final int NUM_FIXED = 9; // number of universal attributes
    public static final int NUM_OPT = 5; // number of optional attributes 
    public static final int NUM_ATTRIB = NUM_FIXED + NUM_OPT; // number of total attributes
    
    // the names of universal attributes
    public static final String[] ATTRIB_NAME = {
	"id", "protocol", "url", "size", "type", "created",
	"last_mod", "src", "parent"
    };
    
    // variable definitions
    private int id;
    private String protocol;
    private String url;
    private int size;
    private String type;
    private long created;
    private long last_mod;
    private String src;
    private int parent;
    private String opt[];
    
    
    // constructors
    public Tuple (int i, String pro, String u, int sz, String t, long c, long l, String s, int p, String[] o) {
	
	id = i;
	protocol = pro;
	url = u;
	size = sz;
	type = t;
	created = c;
	last_mod = l;
	src = s;
	parent = p;
	opt = new String[NUM_OPT];
	
	// initiate opt field
	if (o == null) {
	    for (int j=0; j<NUM_OPT; j++) {
		opt[j] = null;
	    } 
	} else {  
	    for (int j=0; j<o.length && j<NUM_OPT; j++) {
		opt[j] = o[j];
	    }
	    
	    if (o.length > NUM_OPT) {
		System.err.println("WARNING: INSUFFICIENT NUMBER OF FIELDS.  DATA MAY GET LOST.");
	    }
	}
    }
    
    // parse a result set into a vector of tuples
    public static Vector parseResultSet(ResultSet r) {
	
	Tuple[] tmp = null;
	int id = -1;
	String protocol = null;
	String url = null;
	int size = -1;
	String type = null;
	long created = -1;
	long last_mod = -1;
	String src = null;
	int parent = -1;
	String[] opt = new String[NUM_OPT];
	Vector v = new Vector();

	try {
	    ResultSetMetaData m=r.getMetaData();
	    int col=m.getColumnCount();
	    	    
	    if ( col != NUM_ATTRIB ) {
		System.err.println("ERROR: RESULT SET HAS INCOMPATIBLE NUMBER OF COLUMNS.");
		System.exit(1);
	    }

	    while ( r.next() ) {
		//System.err.println("first occurence.");
		for(int i=1;i<=col;i++) {
		    if ( m.getColumnLabel(i).equals("ID") ) {
			id = r.getInt(i);
		    } else if ( m.getColumnLabel(i).equals("PROTOCOL") ) {
			protocol = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("URL") ) {
			url = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("SIZE") ) {
			size = r.getInt(i);
		    } else if ( m.getColumnLabel(i).equals("TYPE") ) {
			type = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("CREATED") ) {
			try {
			    created = Long.parseLong(r.getString(i));
			} catch (Exception e) {
			    e.printStackTrace();
			}
		    } else if ( m.getColumnLabel(i).equals("LAST_MOD") ) {
			try {
			    last_mod = Long.parseLong(r.getString(i));
			} catch (Exception e) {
			    e.printStackTrace();
			}
		    } else if ( m.getColumnLabel(i).equals("SRC") ) {
			src = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("PARENT") ) {
			parent = r.getInt(i);
		    } else if ( m.getColumnLabel(i).equals("OPT1") ) {
			opt[0] = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("OPT2") ) {
			opt[1] = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("OPT3") ) {
			opt[2] = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("OPT4") ) {
			opt[3] = r.getString(i);
		    } else if ( m.getColumnLabel(i).equals("OPT5") ) {
			opt[4] = r.getString(i);
		    } 
		}
		
		//System.err.println("before adding tuple");
		v.add(new Tuple(id, protocol, url, size, type, created, last_mod, src, parent, opt));
	    }

	} catch(SQLException e) {
	    System.err.println(e);
	}
	
	//System.err.println("vector returned: " + v.size());
	return v;
    }
    
    public String toString() {
	String tmp = "";
	tmp += "ID: " + id + "\n";
	tmp += "PROTOCOL: " + protocol + "\n";
	tmp += "URL: " + url + "\n";
	tmp += "SIZE: " + size + "\n";
	tmp += "TYPE: " + type + "\n";
	tmp += "CREATED: " + created + "\n";
	tmp += "LAST MODIFIED: " + last_mod + "\n";
	tmp += "SOURCE: " + src + "\n";
	tmp += "PARENT ID: " + parent + "\n";
	for (int i=1; i<=NUM_OPT; i++) {
	    tmp += "OPTIONAL FIELD " + i + ": " + opt[i-1] + "\n";
	}
	
	return tmp;
    }
    
    // output an array of tuples to string
    public static String tuplesToString(Vector v) {
	String tmp = "";
	
	if (v==null)
	    return null;
	
	for (int i=0; i<v.size(); i++) {
	    tmp += ((Tuple)v.elementAt(i)).toString();
	    tmp += "\n";
	}
	return tmp;
    }

    // accessors
    public int getID() { return id;}
    public String getProtocol() { return protocol;}
    public String getUrl() { return url;}
    public int getSize() { return size;}
    public String getType() { return type;}
    public long getCreated() { return created;}
    public long getLastMod() { return last_mod;}
    public String getSrc() { return src;}
    public int getParent() { return parent;}
    public String[] getOpt() { return opt;}

}



