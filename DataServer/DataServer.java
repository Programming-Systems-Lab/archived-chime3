/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * DataServer.java
 *
 * @author Shen Li
 * @version 1.1
 *
 * ***********************************************************
 * List of Methods
 * ***********************************************************
 * public static synchronized DataServer getInstance()
 * public DataServer(String file,String usr,String pwd,String sienaURL)
 * public DataServer(String file, String sienaURL)
 * public DataServer(String sienaURL)
 * public void shutDown()
 * public void subscribeEvent()
 * public void eventReceived(SienaObject e)
 * public int addSourceTuple(String protocol, String url, int size, String type, long created, long last_mod, String src, String opt0, String opt1, String opt2, String opt3, String opt4)
 * public int addSourceTuple(String protocol, String url, int size, String type, long created, long last_mod, String src, String[] opt)
 * public boolean addLinkTuple(String roomUrl, String url, String type)
 * public boolean removeSourceTuple(String protocol, String url)
 * public boolean removeLinkTuple(String roomUrl, String url) {
 * public Vector findSourceTupleRecur(String protocol, String url)
 * public Vector findSourceTuple(String protocol, String url)
 * public boolean setShape(String classtype, String subtype, String shape, String protocol, String url)
 * public boolean setLinkShape(String classtype, String subtype, String shape, String url, String roomUrl)
 * public String getParentDomain(String input)
 * public String getDomain(String input)
 * public void printTable(String tableName)
 */

package psl.chime.DataServer;

import org.hsql.*;
import org.hsql.util.*;
import org.jdom.*;
import org.jdom.input.*;
import siena.*;
import java.io.*;
import java.sql.*;
import java.net.*;
import java.util.*;
import psl.chime.sienautils.*;
import psl.chime.EventTracer.*;

public class DataServer {
    
    // SQL statements to create the tables
    private static String[] tableCreationSQL = {		
	"create table SOURCE (id integer, protocol varchar(255), url varchar(255), size integer, type varchar(20), created varchar(255), last_mod varchar(255), src varchar(255), parent integer, classtype varchar(255), subtype varchar(255), shape varchar(255), shape2d varchar(255), opt1 varchar(255), opt2 varchar(255), opt3 varchar(255), opt4 varchar(255), opt5 varchar(255), PRIMARY KEY (id), UNIQUE (protocol, url) )",
	
       	"create index id on SOURCE(id)",
	"create index url on SOURCE(protocol, url)"
    };

    private EventTracer et = null;
    private Connection conn; // database connection
    private Statement statement; // sql statement
    private DatabaseMetaData meta; // database metadata
    private int current; // the largest assigned index 
    private HierarchicalDispatcher siena; // siena connection
    private Filter filter1; // siena filter
    private Filter filter2; // siena filter

    private static DataServer myself;
    
    public static synchronized DataServer getInstance() {
	if (myself == null)
	    myself = new DataServer();
	return myself;
    }
    
    // constructors
    public DataServer(String file,String usr,String pwd){
	
 	// establish connection to the local database
	connectDB(file, usr, pwd);
	
	// establish connection to the siena server
	// connectSiena(sienaURL);
	
	System.out.println("DATASERVER STARTED.");
	
	et = new EventTracer(this);
    }
    
    public DataServer(String file) {
	this(file,"sa","");
    }

    public DataServer() {
	this("DB","sa","");
    }
    
    // shut down the data server -- destructor 
    public void shutDown() {
	// disable connection to siena server
	// disconnectSiena();

	// disable connection to the local database
	disconnectDB();
	
	System.out.println("DATA SERVER SHUT DOWN SUCCESSFULLY.");
    }

    // establish connection to the local database
    private void connectDB(String file, String usr, String pwd) {
	try {
	    Class.forName("org.hsql.jdbcDriver").newInstance();
	    conn = DriverManager.getConnection("jdbc:HypersonicSQL:"+file,usr,pwd);
	    statement = conn.createStatement();
	} catch (Exception e) {
	    System.err.println("ERROR: FAILS TO ESTABLISH CONNECTION TO " + file + ".");
	    System.exit(1);
	}
	
	// create the table SOURCE, if not already exists
	for(int i=0;i<tableCreationSQL.length;i++) {
	    try {
		statement.executeQuery(tableCreationSQL[i]);
	    } catch(SQLException e) {
		if (e.getErrorCode() != 0) {
		    System.err.println("ERROR: FAILS TO CREATE TABLE");
		    System.exit(1);
		} 
		
		System.err.println(e);
	    }
	}
	
	current = getMaxIndex();
    
	System.out.println("CONNECTION TO " + file + " ESTABLISHED.");
	System.out.println("CURRENT MAX ID: " + current);
    }
    
    
    // close database connection
    private void disconnectDB() {
	try {
	    conn.close();
	} catch (Exception e) {
	    System.err.println("ERROR: FAILS TO CLOSE DATABASE CONNECTION.");
	    System.exit(1);
	}
	
	System.out.println("CONNECTION TO DATABASE CLOSED.");
    }
    
    // retrieve the maximum id in the database
    private int getMaxIndex() {
	int tmp = -1;
	try {
	    ResultSet r = statement.executeQuery("select max(id) from SOURCE");
	    if ( r.next() )
		tmp = r.getInt(1);
	} catch(SQLException e) {} 
	
	return tmp;
    }
    
    // setting up siena connection to the server
    public void subscribeEvent() {
    }

    private void connectSiena(String url) {
	/*	try {
	    siena = new HierarchicalDispatcher();
	    siena.setMaster(url);
	    
	    filter1 = new Filter();
	    filter1.addConstraint("component", "FRAX");
	    filter2 = new Filter();
	    filter2.addConstraint("component", "CLIENT");
	
	    try {
		siena.subscribe(filter1, this);
		siena.subscribe(filter2, this);
	    } catch (SienaException ex) {
		System.err.println("Siena error:" + ex.toString());
		System.exit(1);
	    }
	    System.out.println("CONNECTION TO SIENA SERVER ESTABLISHED.");
	    System.out.println("SUBSCRIBING FOR " + filter.toString());
	} catch (Exception ex) {
	    ex.printStackTrace();
	    System.exit(1);
	} 
	*/
    }
    
    // disable connection to the siena server
    private void disconnectSiena() {
	/*
	try {
	    System.out.println("UNSUBSCRIBING");
	    siena.unsubscribe(filter, this);
	    siena.shutdown();
	    System.out.println("CONNECTION TO SIENA SERVER CLOSED.");
	} catch (Exception ex) {
	    ex.printStackTrace();
	    System.exit(1);
	} 
	*/
    }
    
    // handle an event
    public void eventReceived(SienaObject e) {
	
	String fromComponent = e.getFromComponent();
	String method = e.getMethod();
	String protocol = e.getProtocol();
	String data = e.getData();
	StringTokenizer st;

	// FRAX Call
	if (fromComponent.equals("FRAX")) {
	    
	    String type;
	    Parser p = null;
	    
	    try {
		SAXBuilder builder = new SAXBuilder();
		data = e.getData(); 
		Document doc = builder.build( new StringReader(data) ); 
		//Document doc = builder.build( new StringReader(xmlsample));
		System.err.println("I just got this event:");
		System.err.println(doc);
		type = doc.getRootElement().getAttribute("type").getValue();
		
		if (type.equals("dir")) {
		    p = new DirParser(doc, data);
		    //p = new DirParser(doc, xmlsample);
		    SourceTuple t = p.parseDoc();
		    int tupleID = addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		    
		    // create a table of links and images
		    try {
			String tableName = "table" + tupleID;
			statement.executeQuery("create table " + 
					       tableName + 
					       " (link varchar(255), type varchar(255), classtype varchar(255), subtype varchar(255), shape varchar(255), shape2d varchar(255) )");
			data = t.getOpt()[2];
			st = new StringTokenizer(data, " \n");
			while ( st.hasMoreTokens() ) {
			    statement.executeQuery("insert into " + 
						   tableName + 
						   " values ('" + 
						   st.nextToken() +
						   "', 'FILE', null, null, null, null)");
			}
						
			printTable(tableName);
		    
		    } catch(SQLException e1) {
			if (e1.getErrorCode() != 0) {
			    System.err.println("ERROR: FAILS TO CREATE TABLE");
			    System.exit(1);
			} 
			System.err.println(e1);
		    } catch (Exception e2) {
			System.err.println(e2);
		    }

		} else if (type.equals("html")) {

		    p = new HtmlParser(doc,data);
		    //p = new HtmlParser(doc,xmlsample);
		    SourceTuple t = p.parseDoc();
		    int tupleID = addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		    
		    // create a table of links and images
		    try {
			String tableName = "table" + tupleID;
			statement.executeQuery("create table " + 
					       tableName + 
					       " (link varchar(255), type varchar(255), classtype varchar(255), subtype varchar(255), shape varchar(255), shape2d varchar(255) )");
			int idx1 = 0;
			int idx2 = 0;
			data = t.getOpt()[2];
			System.err.println("get into loop");
			System.err.println(data);
			while ( idx2<data.length() ) {
			    idx1 = data.indexOf("<Source>", idx1)+8;
			    idx2 = data.indexOf("</Source>", idx2);
			    if (idx1>=data.length() || idx2>=data.length() 
				|| idx1<=7 || idx2 <=0)
				break;
			    statement.executeQuery("insert into " + 
						   tableName + 
						   " values ('" + 
						   data.substring(idx1,idx2) +
						   "', 'IMAGE', null, null, null, null)");
			    System.err.println("one iteration " + idx1 + " " + idx2);
			}
			System.err.println("out of loop");
			data = t.getOpt()[4];
			st = new StringTokenizer(data, " \n");
			while ( st.hasMoreTokens() ) {
			    statement.executeQuery("insert into " + 
						   tableName + 
						   " values ('" + 
						   st.nextToken() +
						   "', 'LINK', null, null, null, null)");
			}
						
			printTable(tableName);
		    
		    } catch(SQLException e1) {
			if (e1.getErrorCode() != 0) {
			    System.err.println("ERROR: FAILS TO CREATE TABLE");
			    System.exit(1);
			} 
			System.err.println(e1);
		    } catch (Exception e2) {
			System.err.println(e2);
		    }
		    		
		} else if (type.equals("image")) {
		    p = new ImageParser(doc);
		    SourceTuple t = p.parseDoc();
		    addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		} else if (type.equals("txt")) {
		    p = new TxtParser(doc);
		    SourceTuple t = p.parseDoc();
		    addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		} else if (type.equals("other")) {
		    p = new OtherParser(doc);
		    SourceTuple t = p.parseDoc();
		    addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		} else {
		    System.err.println("Type Not Found.");
		}
	    } catch (Exception ex) {
		// check if bscw type
		if (data != null) {
		    if (-1 != data.indexOf("<BSCW>")) {
			p = new BscwParser(data);
			SourceTuple t = p.parseDoc();
			addSourceTuple(t.getProtocol(), t.getUrl(), t.getSize(), t.getType(), t.getCreated(), t.getLastMod(), t.getSrc(), t.getOpt());
		    } else {
			ex.printStackTrace();			
		    }
		} else {
		    ex.printStackTrace();
		}
	    }
	
	    printTable("SOURCE");
	}

	// CLIENT CALL
	else if (e.getFromComponent().equals("CLIENT")) {
	    
	    // method: c_getRoom
	    if (method.equals("c_getRoom")) {
		Vector v = findSourceTuple(protocol, data);
	
		// query frax
		if ( v.size() == 0 ) {
		    e.setFromComponent("SERVER");
		    e.setMethod("s_queryFrax");
		    try {
			e.publish();
		    } catch (Exception ex) {
			System.err.println(ex);
		    }
		    e.setFromComponent("FRAX");
		    e.setMethod("c_getRoom");

		    eventReceived(e); // call the method again
		} else {
		    String tableName = "table" + ((SourceTuple)v.elementAt(0)).getID(); 
		    v = findLinkTuple(tableName);
		    if (v == null || v.size() == 0 ) {
			e.setData(data + " 10 5 1 0\n");
		    } else {
			int ctr = v.size();
			int length = 2* (ctr + 1);
			if (length < 10)
			    length = 10;
			int isDefault = 1;
			double x = 0;
			double y = 0;
			double z = 0;
			LinkTuple tmp;
			data += " 10 5 " + length + " " + ctr + "\n";
			for (int i=0; i<ctr; i++) {
			    tmp = ((LinkTuple)v.elementAt(i));
			    data += tmp.getLink() + " " + 
				tmp.getShape() + " " +
				tmp.getClasstype() + " " + 
				tmp.getSubtype() + " " +
				isDefault + " " +
				x + " " + y + " " + z + "\n";
			}
			e.setData(data);
		    }
		    e.setFromComponent("SERVER");
		    e.setMethod("s_roomInfo");
		    try {
			e.publish();
		    } catch (Exception ex) {
			System.err.println(ex);
		    }
		} 
	    }
	    
	    // method: c_addObject
	    else if (method.equals("c_addObject")) {
		st = new StringTokenizer(data, " ");
		if (st.countTokens() < 5) { 
		    System.err.println("c_addObject Call does not have enough tokens.");
		    return;
		} 
		
		String roomUrl = st.nextToken();
		String objUrl = st.nextToken();
		double x = 0;
		double y = 0;
		double z = 0;
		try {
		    x = Double.parseDouble(st.nextToken());
		    y = Double.parseDouble(st.nextToken());
		    z = Double.parseDouble(st.nextToken());
		} catch (Exception ex) {
		    System.err.println(ex);
		    return;
		}
		
		// add an link tuple to the link table
		addLinkTuple(protocol, roomUrl, objUrl, "LINK");
		
		// add the movement tuple 
		et.addMovementTuple(roomUrl, objUrl, 0, 0, 0, x, y, z);
		
		// find the shape info about the object
		Vector v = findSourceTuple(protocol, roomUrl);
		if ( v.size() != 0 ) {
		    String tableName = "table" + ((SourceTuple)v.elementAt(0)).getID(); 
		    v = findLinkTuple(tableName);
		    if (v != null && v.size() != 0 ) {
			int ctr = v.size();
			LinkTuple tmp;
			for (int i=0; i<ctr; i++) {
			    tmp = ((LinkTuple)v.elementAt(i));
			    if (tmp.getLink().equals(objUrl)) {	
				data = roomUrl + " " + 
				    objUrl + " " +
				    tmp.getShape() + " " +
				    tmp.getClasstype() + " " + 
				    tmp.getSubtype() + " " + 
				    x + " " + y + " " + z + "\n";
			    }
			    e.setData(data);
			}
		    }
		}
		 
		// propagate the info
		e.setFromComponent("SERVER");
		e.setMethod("s_addObject");
		
		try {
		    Vector v1 = et.findRoomTuple(roomUrl);
		    if (v1 == null)
			return;
		    for (int idx=0; idx < v1.size(); idx++) {
			e.setUsername(((RoomTuple)(v1.elementAt(idx))).getUser());
			e.publish();
		    }
		} catch (Exception ex) {
		    System.err.println(ex);
		    return;
		}
	    }
	    
	    // method: c_deleteObject
	    else if (method.equals("c_deleteObject")) {
		st = new StringTokenizer(data, " ");
		if (st.countTokens() < 2) { 
		    System.err.println("c_deleteObject Call does not have enough tokens.");
		    return;
		} 
		
		String roomUrl = st.nextToken();
		String objUrl = st.nextToken();
		
		// remove an link tuple to the link table
		removeLinkTuple(protocol, roomUrl, objUrl);
		
		// remove the movement tuple 
		et.removeMovementTuple(roomUrl, objUrl);
		
		// propagate the info
		e.setFromComponent("SERVER");
		e.setMethod("s_deleteObject");
		
		try {
		    Vector v = et.findRoomTuple(roomUrl);
		    if (v == null)
			return;
		    for (int idx=0; idx < v.size(); idx++) {
			e.setUsername(((RoomTuple)(v.elementAt(idx))).getUser());
			e.publish();
		    }		
		} catch (Exception ex) {
		    System.err.println(ex);
		    return;
		}
	    }
	}
    }


    // insert a tuple into the database, if not already there.
    // all string arguments must be enclosed by '' except url.
    public int addSourceTuple(String protocol, String url, int size, String type, long created, long last_mod, String src, String opt0, String opt1, String opt2, String opt3, String opt4) {
	
	if (protocol == null)
	    protocol = "http";
	else
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	if (src != null)
	    src = src.toLowerCase();
	
	int tupleID = -1;
	Vector v = findSourceTuple(protocol, url);

	// tuple already exists in the database
	if (v != null && v.size() != 0) {
	    SourceTuple t = (SourceTuple) v.elementAt(0);
	    if ( t.getLastMod() >= last_mod ) { 
		// tuple in the database is newer
		System.err.println("Newer tuple exists in the database.  Insert failed.");
		return -1;
	    } else {
		// update the tuple
		
		int p = t.getParent(); // save parent info 
		try {
		    statement.executeQuery("delete from SOURCE where PROTOCOL = '" + protocol + "' AND URL = '" + url + "'");
		    tupleID = current + 1;
		    statement.executeQuery("insert into SOURCE values (" + 
					   ++current + ",'" + 
					   protocol + "','" + 
					   url + "'," + 
					   size + "," + 
					   type + "," + 
					   created + "," +
					   last_mod + ",'" + 
					   src + "'," + 
					   p + 
					   ",null, null, null, null,'" + 
					   opt0 + "','" + 
					   opt1 + "','" + 
					   opt2 + "','" + 
					   opt3 + "','" + 
					   opt4 + "')");
		} catch(SQLException e) {
		    System.err.println(e);
		    return -1;
		} 
	    }
	} else {
	    // tuple does not exist
	    System.err.println("tuple not exist: " + url);
	    try {	  
		tupleID = current + 1;
		statement.executeQuery("insert into SOURCE values (" + 
				       ++current + ",'" + 
				       protocol + "','" + 
				       url + "'," + 
				       size + ",'" + 
				       type + "'," + 
				       created + "," +
				       last_mod + ",'" + 
				       src + "'," + 
				       addParent( protocol, getParentDomain(url) ) + 
				       ",null,null,null,null,'" +
				       opt0 + "','" + 
				       opt1 + "','" + 
				       opt2 + "','" + 
				       opt3 + "','" + 
				       opt4 + "')");
	    } catch(SQLException e) {
		System.err.println(e);
		return -1;
	    } 
	    
	}
	
	return tupleID;
    }
    
    public int addSourceTuple(String protocol, String url, int size, String type, long created, long last_mod, String src, String[] opt) {
	return addSourceTuple(protocol, url, size, type, created, last_mod, src, opt[0], opt[1], opt[2], opt[3], opt[4]);
    }
    
    // add an link tuple to the link table
    public boolean addLinkTuple(String protocol, String roomUrl, String url, String type) {
       	
	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	roomUrl = roomUrl.toLowerCase();
	
	Vector tmp = findSourceTuple(protocol, roomUrl);
	if (tmp == null)
	    return false;
	
	String tableName = "table" + ((SourceTuple)tmp.elementAt(0)).getID();
	
	try {
	    statement.executeQuery("create table " + 
				   tableName + 
				   " (link varchar(255), type varchar(255), classtype varchar(255), subtype varchar(255), shape varchar(255), shape2d varchar(255) )");
	    
	    statement.executeQuery("insert into " + 
				   tableName + 
				   " values ('" + 
				   url + "','" +
				   type + "', null, null, null, null)");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}
	
	return true;
    }

    // recursively add the parent directory into the database
    private int addParent(String protocol, String url) {
	if (protocol == null || url == null) {
	    //System.err.println("no parent.");
	    return -1;
	} else {
	  
	    Vector v = findSourceTuple(protocol, url);
	    if (v == null || v.size() == 0) {	      
		int tmp = ++ current;
		//System.err.println(url + " NOT FOUND IN DB.");
		// parent does not exist in the database
		try { 
		    statement.executeQuery("insert into SOURCE values (" + 
					   tmp + ",'" + 
					   protocol + "','" + 
					   url + 
					   "',-1,'dir',-1,-1,null," +
					   addParent(protocol, getParentDomain(url)) + 
					   ",null,null,null,null,null,null,null,null,null)" );
		} catch(SQLException e) {
		    System.err.println(e);
		} 
		
		return tmp; // return the id of the added parent
	    } else {
		// parent is found in the database
		return ((SourceTuple)v.elementAt(0)).getID();
	    }
	}
    }

  // remove the tuple with the specific url from the database
  // if a directory, all subdirectories and files are removed
  public boolean removeSourceTuple(String protocol, String url) {
      if (protocol == null)
	  protocol = "http";
      else 
	  protocol = protocol.toLowerCase();
      
      url = url.toLowerCase();
      Vector v = findSourceTuple(protocol, url);
    
    if (v == null || v.size() == 0) {
      System.err.println("SourceTuple Not Found.  Remove Failed.");
      return false; // tuple not found
    }
    
    try {
	statement.executeQuery("delete from SOURCE where PROTOCOL='" + 
			       protocol + "' AND URL like '" + url + "%'");
    } catch(SQLException e) {
      System.err.println(e);
      return false;
    } 
    
    return true;
  }

    // remove an link tuple from the link table
    public boolean removeLinkTuple(String protocol, String roomUrl, String url) {
       	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();

	url = url.toLowerCase();
	roomUrl = roomUrl.toLowerCase();
	
	Vector tmp = findSourceTuple(protocol, roomUrl);
	if (tmp == null)
	    return false;
	
	String tableName = "table" + ((SourceTuple)tmp.elementAt(0)).getID();
	
	try {
	    statement.executeQuery("delete from " + tableName +
				   " where URL like '" + url + "%'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}
	
	return true;
    }

    /* find tuples corresponding to the given url
     * if a file, return a tuple containing the info of file
     * if a directory, return all contents of that directory
     */
    public Vector findSourceTupleRecur(String protocol, String url) {

	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	ResultSet r = null;
   
	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from SOURCE where PROTOCOL='"
				       + protocol + "' AND URL = '" + url + "'");
	} catch(SQLException e) {
	  System.err.println(e);
	  return null;
	} 
	
	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector tmp = SourceTuple.parseResultSet(r);
	//System.err.println("vector received: " + tmp.size());

	// check database consistency
	if ( tmp == null || tmp.size() == 0 ) {
	    //  System.err.println("vector: SourceTuple Not Found.");
	    return null;
	} else if ( tmp.size() != 1 ) {
	    System.err.println("ERROR: INCONSISTENT DATABASE.  MULTIPLE ENTRIES WITH SAME URL EXIST.");
	    System.exit(1);
	} 
	
	if ( ! ((SourceTuple)tmp.elementAt(0)).getType().toUpperCase().equals("DIR") ) {
	  // is file
	  return tmp; 
	} else {
	  
	  // is directory
	    try {
	      r = statement.executeQuery("select * from SOURCE where PARENT="+ ((SourceTuple)tmp.elementAt(0)).getID() );
	      tmp = SourceTuple.parseResultSet(r);
	    } catch(SQLException e) {
	      return null;
	    } 
	    
	    return tmp;
	}  
    }
  
    /* find the tuples corresponding to the given url
     * treat file and directory in the same way
     */
    public Vector findSourceTuple(String protocol, String url) {
	
	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	ResultSet r = null;
	
	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from SOURCE where PROTOCOL='"
				       + protocol + "' AND URL = '" + url + "'");
	} catch(SQLException e) {
	  System.err.println(e);
	  return null;
	} 
	
	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector tmp = SourceTuple.parseResultSet(r);

	// check database consistency
	if ( tmp == null || tmp.size() == 0 ) {
	    // System.err.println("vector: SourceTuple Not Found.");
	    return null;
	} else if ( tmp.size() != 1 ) {
	    System.err.println("ERROR: INCONSISTENT DATABASE.  MULTIPLE ENTRIES WITH SAME URL EXIST.");
	    System.exit(1);
	} 
	
	return tmp;
    }

    
    /* find the links in the table */
    public Vector findLinkTuple(String tableName) {
		
	ResultSet r = null;
	
	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from " + tableName);
	} catch(SQLException e) {
	    System.err.println(e);
	    return null;
	} 
	
	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector v = LinkTuple.parseResultSet(r);
	return v;
    }

        
    /* set the shape field of a tuple in the database */
    public boolean setShape(String classtype, String subtype, String shape, String shape2d, String protocol, String url) {
       
	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	ResultSet r = null;
   
	try {
	    //System.err.println("Try to set shape " + url);
	    statement.executeQuery("update SOURCE set CLASSTYPE='" + classtype + 
				   "' where PROTOCOL='" + protocol + 
				   "' AND URL = '" + url + "'");
	    statement.executeQuery("update SOURCE set SUBTYPE='" + subtype + 
				   "' where PROTOCOL='" + protocol + 
				   "' AND URL = '" + url + "'");
	    statement.executeQuery("update SOURCE set SHAPE='" + shape + 
				   "' where PROTOCOL='" + protocol + 
				   "' AND URL = '" + url + "'");
	    statement.executeQuery("update SOURCE set SHAPE2D='" + shape2d + 
				   "' where PROTOCOL='" + protocol + 
				   "' AND URL = '" + url + "'");
	} catch(SQLException e) {
	  System.err.println(e);
	  return false;
	}
	
	return true;
    }

    
    /* set the shape field of a tuple in the database */
    public boolean setLinkShape(String classtype, String subtype, String shape, String shape2d, String protocol, String roomUrl, String url) {
	
	if (protocol == null)
	    protocol = "http";
	else 
	    protocol = protocol.toLowerCase();
	
	url = url.toLowerCase();
	roomUrl = roomUrl.toLowerCase();
	
	Vector tmp = findSourceTuple(protocol, roomUrl);
	if (tmp == null)
	    return false;
	
	String tableName = "table" + ((SourceTuple)tmp.elementAt(0)).getID();
	
	try {
	    //System.err.println("Try to set shape " + url);
	    statement.executeQuery("update " + tableName + 
				   " set CLASSTYPE='" + classtype + 
				   "' where URL = '" + url + "'");
	    statement.executeQuery("update " + tableName + 
				   " set SUBTYPE='" + subtype + 
				   "' where URL = '" + url + "'");
	    statement.executeQuery("update " + tableName + 
				   " set SHAPE='" + shape + 
				   "' where URL = '" + url + "'");
	    statement.executeQuery("update " + tableName + 
				   " set SHAPE2D='" + shape2d + 
				   "' where URL = '" + url + "'");
	} catch(SQLException e) {
	  System.err.println(e);
	  return false;
	}
	
	// post the sienaObject 
	// s_changeCLass -- "roomUrl objectUrl newClassType newSubClassType new3Dfile new2Dfile"
	
	return true;
    }


    // find the url corresponding to direct parent of input string
    public String getParentDomain(String input) {
	
	for (int i=input.length()-2; i>0; i--) {
	    if (input.charAt(i) == '/') {
		if (input.charAt(i-1) == '/') {
		    // protocol delimiter reached.  no more parent
		    return null; 
		} else {
		    return input.substring(0,i+1);
		}
	    }
	}
	
	// no '/' found
	return null;   
    }
    

    // retrieve the domain of an input url
    public String getDomain(String input) {
	StringTokenizer st =  new StringTokenizer(input, ":");
	String proc = st.nextToken();
	String dom;

	if (proc.toUpperCase().equals("LOCALHOST")) {
	    dom = proc;
	} else if (proc.toUpperCase().equals("HTTP") ||
		   proc.toUpperCase().equals("FTP")) {
	    st = new StringTokenizer(input, "/");
	    if (st.countTokens()>=2) {
		st.nextToken();
		dom = proc + "://" + st.nextToken() + "/";
	    } else {
		System.err.println("HTTP/FTP PROTOCOL FORMAT ERROR.");
		dom = null;
	    }
	} else if (proc.toUpperCase().equals("SQL")) {
	    dom = proc;
	} else {
	    System.err.println("UNKNOWN PROTOCOL ENCOUNTERED.");
	    dom = null;
	}

	if (dom != null)
	    return dom.toUpperCase();
	else
	    return null;
    }
    
 
    // Print the entire source table
    public void printTable(String tableName) {
	
	try {
	    if (tableName.equals("SOURCE")) {
		Vector v = SourceTuple.parseResultSet(statement.executeQuery("select * from " + tableName ) );
	    
		System.out.println("\nThe current data table is:");
		System.out.println(SourceTuple.tuplesToString(v));
	    } else {
		Vector v = LinkTuple.parseResultSet(statement.executeQuery("select * from " + tableName ) );
  
		System.out.println("\nThe current data table is:");
		System.out.println(LinkTuple.tuplesToString(v));
	    }
	} catch (SQLException e) {
	    System.err.println(e);
	}
    }	

    private String readFile(String file) {
	try {
	    FileReader read=new FileReader(file);
	    char buffer[]=new char[1024];
	    StringBuffer b=new StringBuffer();
	    while(true) {
		int i=read.read(buffer,0,1024);
		if(i==-1) {
		    break;
		}
		b.append(buffer,0,i);
	    }
	    read.close();
	    return b.toString();
	} catch(IOException e) {
	    return e.getMessage();
	}
    }
    
    private void writeFile(String file,String text) {
	try {
	    FileWriter write=new FileWriter(file);
	    write.write(text.toCharArray());
	    write.close();
	} catch(IOException e) {
	    e.printStackTrace();
	}
    }
    
    // testing function
    public static void main(String argv[]) {
	
	/* if (argv.length != 2) {
	   System.err.println("2 Command-Line Arguments Expected for the DataServer.");
	   System.exit(-1);
	   } 
	*/
		    
	DataServer ds = DataServer.getInstance();
	
	//ds.shutDown();

	/* SAMPLE SQL QUERIES */
	/*
	  setRecent("select * from Customer");
	  setRecent("select * from Customer where place<>3000");
	  setRecent("select * from place where code>3000 or code=1200");
	  setRecent("select * from Customer where nr<=8\nand name<>'Mueller'");
	  setRecent("update Customer set name='Russi'\nwhere name='Rossi'");
	  setRecent("delete from Customer where place=8000");
	  setRecent("insert into place values(3600,'Thun')");
	  setRecent("drop index Customer.iNr");
	  setRecent("select * from Customer where name like '%e%'");
	  setRecent("select count(*),min(code),max(code),sum(code) from place");
	*/
	
	
	/* SAMPLE FRAX DATA FROM SIENA EVENTS */
	/*
	  String xmlsample = "<?xml version=\"1.0\"?><MetaInfo createDate='981875747740' type='dir'><Protocol>LFS</Protocol><Name>c:/chime/testdir</Name><Type>DIRECTORY</Type><Last-Modified>981843542000</Last-Modified><Hidden>false</Hidden><Size>0</Size><FILE><TYPE>LFS</TYPE><URL>c:/chime/testdir/demo.pdf</URL></FILE><FILE><TYPE>LFS</TYPE><URL>c:/chime/test/dir/diamond_red</URL></FILE></MetaInfo>";
	*/
	
	/*
	  String xmlsample = "<?xml version=\"1.0\"?><MetaInfo createDate='983601161046' type = 'bscw'><?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?><!DOCTYPE BSCW PUBLIC '-//GMD//DTD Simple BSCW//EN' 'bscw.dtd'><!-- BSCW XML container listing --><BSCW><HEAD><TITLE>:sl697</TITLE><BASE href='http://mercer.psl.cs.columbia.edu/bscw/bscw.cgi/0/76490'/><!-- multi actions, applicable to content --><ACTIONS><ACTION name='confirm' type='multi'>Catch up</ACTION><ACTION name='send' type='multi'>Send</ACTION><ACTION name='archive' type='multi'>Archive</ACTION><ACTION name='copy' type='multi'>Copy</ACTION><ACTION name='cut' type='multi'>Cut</ACTION><ACTION name='deleteObj' type='multi'>Delete</ACTION></ACTIONS></HEAD><!-- Container page --><ARTIFACT id='76490' name=':sl697' type='Home' info='Home' icon='/bscw_icons/home_s.gif' shared='no' container='yes'><USER id='76487' name='sl697' role='creator'>sl697</USER><DATE type='created' time='979927291.084' format='long'/><DATE type='modified' time='979927291.084' format='long'/><ACTIONS><ACTION name='jget' type='entry'>Get</ACTION><ACTION name='inf' icon='/bscw_icons/info.gif' type='info'>More info</ACTION></ACTIONS><ARTIFACT id='73108' name='3156 Teaching Workspace' type='Folder' info='Folder' icon='/bscw_icons/folder_s.gif' shared='yes' container='yes'><USER id='115' name='kaiser' role='creator'>Gail Kaiser</USER> <USER id='115' name='kaiser' role='owner'>Gail Kaiser</USER> <DATE type='created' time='977173036.261' format='long'/><DATE type='modified' time='982774353.941' format='long'/><DESCRIPTION>For 3156 teaching staff ONLY, no one else should be &quot;invited&quot;.</DESCRIPTION><ACTIONS><ACTION name='jget' type='entry'>Get</ACTION><ACTION name='inf' icon='/bscw_icons/info.gif' type='info'>More info</ACTION></ACTIONS></ARTIFACT><ARTIFACT id='73104' name='3156 Workspace' type='Folder' info='Folder' icon='/bscw_icons/folder_s.gif' shared='yes' container='yes'><USER id='115' name='kaiser' role='creator'>Gail Kaiser</USER> <USER id='115' name='kaiser' role='owner'>Gail Kaiser</USER><DATE type='created' time='977173004.446' format='long'/> <DATE type='modified' time='983284527.468' format='long'/><DESCRIPTION>All 3156 assignments should be uploaded here.</DESCRIPTION><ACTIONS><ACTION name='jget' type='entry'>Get</ACTION><ACTION name='inf' icon='/bscw_icons/info.gif' type='info'>More info</ACTION></ACTIONS></ARTIFACT></ARTIFACT></BSCW></MetaInfo>";
	*/
	
	/*
	  String xmlsample = '<?xml version=\'1.0\'?><MetaInfo createDate='983062136056' type='html'><Protocol>LFS</Protocol><Name>c:/chime/testdir/index.html</Name><Type>HTML</Type><Last-Modified>980652180000</Last-Modified><Size>2313</Size><Hidden>false</Hidden><Image>/img/dept-title-bar.gif Brandeis University --Michtom School of Computer Science</Image><Image>/img/volen-center-wide.gif [A picture of the Volen Center for Complex Systems]</Image><Link>brandeis</Link><Link>http://www.brandeis.edu</Link><Link>mailto:webmaster@cs.brandeis.edu</Link></MetaInfo>';
	*/
	
	/*
	  String xmlsample = '<?xml version='1.0'?><MetaInfo createDate='982637062130' type='image'><Protocol>LFS</Protocol><Name>c:/chime/testdir/dept.gif</Name><Type>image</Type><Last-Modified>980652180000</Last-Modified><Hidden>false</Hidden><Size>17601</Size><WIDTH>500</WIDTH><HEIGHT>176</HEIGHT></MetaInfo>';
	*/
	
	/*
	  String xmlsample = "<?xml version='1.0'?><MetaInfo createDate='982637062130' type='other'><Protocol>LFS</Protocol><Name>c:/chime/testdir/abc.sml</Name><Type>SML</Type><Last-Modified>920652180000</Last-Modified><Size>17601</Size></MetaInfo>";
	*/
	
	/*
	  String xmlsample = "<?xml version='1.0'?><MetaInfo createDate='982637062130' type='txt'><Protocol>LFS</Protocol><Name>c:/chime/testdir/abc.txt</Name><Type>TXT</Type><Last-Modified>920652180000</Last-Modified><Size>17601</Size></MetaInfo>";
	*/
	
	/*
	  String xmlsample = "<?xml version='1.0'?><MetaInfo createDate='982637062130' type='pdf'><Protocol>http</Protocol><Name>http://www.columbia.edu/~shen/abc/abc.pdf</Name><Type>PDF</Type><Last-Modified>920652180000</Last-Modified><Size>17601</Size></MetaInfo>";
	*/	
    }
}






