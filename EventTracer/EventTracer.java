/**
 * Copyright (c) 2001: The Trustees of Columbia University
 * in the City of New York.  All Rights Reserved.
 *
 * EventTracer.java
 *
 * @author Shen Li
 * @version 1.0
 *
 * *******************************************************
 * List of Methods:
 * *******************************************************
 *
 * public static synchronized EventTracer getInstance();
 * public EventTracer(String file,String usr,String pwd,DataServer ds);
 * public EventTracer(String file, DataServer ds);
 * public EventTracer(DataServer ds);
 * public void shutDown();
 * public void eventReceived(SienaObject e);
 * public boolean addRoomTuple(String url, String user);
 * public boolean addMovementTuple(String room, String obj, double x0, double y0, double z0, double x1, double y1, double z1);
 * public boolean removeRoomTuple(String url, String user);
 * public boolean removeMovementTuple(String room, String obj);
 * public boolean removeUser(String user);
 * public boolean removeUser(String roomUrl, String user);
 * public Vector findRoomTuple(String url);
 * public String findUserRoom(String user);
 * public Vector findMovementTuple(String room, String obj);
 * public MovementTuple findLastMovement(String room, String obj);
 * public void printTable(String tableName);
 */

package psl.chime.EventTracer;

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
import psl.chime.DataServer.*;


public class EventTracer {

    // SQL statements to create the tables
    private static String[] tableCreationSQL = {
	"create table ROOM (url varchar(255), user varchar(255), PRIMARY KEY (url, user))",
       	"create index url on ROOM(url)",
	"create table MOVEMENT (room varchar(255), obj varchar(255), x0 real, y0 real, z0 real, x1 real, y1 real, z1 real)",
	"create index obj on MOVEMENT(obj)"
    };

    private DataServer ds; // dataserver
    private Connection conn; // database connection
    private Statement statement; // sql statement
    private DatabaseMetaData meta; // database metadata
    private HierarchicalDispatcher siena; // siena connection
    private Filter filter1; // siena filter

    private static EventTracer myself;

    public static synchronized EventTracer getInstance() {
		return myself;
    }

    // constructor
    public EventTracer(String file,String usr,String pwd, DataServer arg_ds){

	// establish connection to the local database
	connectDB(file, usr, pwd);

	// establish connection to the siena server
	// connectSiena(sienaURL);

	System.out.println("EVENT TRACER STARTED.");

	ds = arg_ds;

	myself = this;
    }

    public EventTracer(String file, DataServer arg_ds) {
	this(file,"sa","",arg_ds);
    }

    public EventTracer(DataServer arg_ds) {
	this("ET","sa","",arg_ds);
    }

    // shut down the data server -- destructor
    public void shutDown() {
	// disable connection to siena server
	// disconnectSiena();

	// disable connection to the local database
	disconnectDB();

	System.out.println("EVENT TRACER SHUT DOWN SUCCESSFULLY.");
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

	// create the tables, if not already exists
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

	System.out.println("CONNECTION TO " + file + " ESTABLISHED.");
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


    // setting up siena connection to the server
    private void connectSiena(String url) {
    /*
       try {
	   siena = new HierarchicalDispatcher();
	   siena.setMaster(url);

	   filter1 = new Filter();
	   filter1.addConstraint("component", "client");

	   try {
	   siena.subscribe(filter1, this);
	   } catch (SienaException ex) {
	   System.err.println("Siena error:" + ex.toString());
	   System.exit(1);
	   }

	    System.out.println("CONNECTION TO SIENA SERVER ESTABLISHED.");
	    System.out.println("SUBSCRIBING FOR " + filter1.toString());
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
	    siena.unsubscribe(filter1, this);
	    siena.shutdown();
	    System.out.println("CONNECTION TO SIENA SERVER CLOSED.");
	} catch (Exception ex) {
	    ex.printStackTrace();
	    System.exit(1);
	}
    */
    }

    // handle an event
    // handle an event
    public void eventReceived(SienaObject e) {

	String fromComponent = e.getFromComponent();
	String method = e.getMethod();

	System.err.println("ET New Events Received.");
	System.err.println("from_component: " + fromComponent );
	System.err.println("method: " + method);

	String protocol = e.getProtocol();
	String data = e.getData();
	StringTokenizer st = new StringTokenizer(data," ");

	// client Call
	if (fromComponent.equals("client")) {

	    // method: c_connect
	    if (method.equals("c_connect")) {

		System.err.println("METHOD CALL: CLIENT.C_CONNECT");
	       if (st.countTokens() < 3)
		   return;
	       st.nextToken();
	       String user = st.nextToken();
	       addRoomTuple("default", user);

	       System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: c_moveObject
	    else if (method.equals("c_moveObject")) {

		System.err.println("METHOD CALL: CLIENT.C_MOVEOBJECT");
		if (st.countTokens() < 5)
		    return;
		String roomUrl = st.nextToken();
		String url = st.nextToken();
		double x = 0;
		double y = 0;
		double z = 0;
		try {
		    x = Double.parseDouble(st.nextToken());
		    y = Double.parseDouble(st.nextToken());
		    z = Double.parseDouble(st.nextToken());
		} catch (Exception ex) {
		    System.err.println(ex);
		}
		addMovementTuple(roomUrl, url, 0, 0, 0, x, y, z);

		// propagate the info
		e.setFromComponent("event_tracer");
		e.setMethod("s_moveObject");

		try {
		    Vector v = findRoomTuple(roomUrl);
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

		System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: c_enteredRoom
	    else if (method.equals("c_enteredRoom")) {

		System.err.println("METHOD CALL: CLIENT.C_ENTEREDROOM");

		if (st.countTokens() < 2)
		    return;
		String user = st.nextToken();
		String roomUrl = st.nextToken();

		// notify users in the old room
		String oldRoom = null;
		Vector v = null;
		String roommate;
		oldRoom = findUserRoom(user);
		if (oldRoom != null && !oldRoom.equals("default")) {
		    v = findRoomTuple(oldRoom);
		    e.setMethod("s_leftRoom");

		    try {
			if (v!=null) {
			    for (int idx=0; idx<v.size(); idx++) {
				roommate = ((RoomTuple)v.elementAt(idx)).getUser();
				if (!roommate.equals(user)) {
				    e.setUsername(roommate);
				    e.setData(user + " " + oldRoom);
				    e.publish();
				}
			    }
			}
		    } catch (Exception ex) {
			System.err.println(ex);
			return;
		    }
		}

		// add user to the new room
		addRoomTuple(roomUrl, user);

		// notify the new user about its roommates
		// notify the roomates about the new user
		v = null;
		roommate = null;
		v = findRoomTuple(roomUrl);
		e.setMethod("s_enteredRoom");

		try {
		    if (v!=null) {
			for (int idx=0; idx<v.size(); idx++) {
			    roommate = ((RoomTuple)v.elementAt(idx)).getUser();
			    if (!roommate.equals(user)) {
				e.setUsername(roommate);
				e.setData(user + " " + roomUrl);
				e.publish();

				e.setUsername(user);
				e.setData(roommate + " " + roomUrl);
				e.publish();
			    }
			}
		    }
		} catch (Exception ex) {
		    System.err.println(ex);
		    return;
		}

		System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: c_subscribeRoom
	    else if (method.equals("c_subscribeRoom")) {

		System.err.println("METHOD CALL: CLIENT.C_SUBSCRIBEROOM");

	    	if (st.countTokens() < 2)
		    return;
		String user = st.nextToken();
		String roomUrl = st.nextToken();

		// add user to the new room
		addRoomTuple(roomUrl, user);

		System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: s_leftRoom
	    else if (method.equals("c_leftRoom")) {

		System.err.println("METHOD CALL: CLIENT.C_LEFTROOM");

		if (st.countTokens() < 2)
		    return;
		String user = st.nextToken();
		String roomUrl = st.nextToken();

		// notify users in the old room
		String oldRoom = null;
		Vector v = null;
		String roommate;
		oldRoom = findUserRoom(user);
		if (oldRoom != null && !oldRoom.equals("default")) {
		    v = findRoomTuple(oldRoom);
		    e.setMethod("s_leftRoom");

		    try {
			if (v!=null) {
			    for (int idx=0; idx<v.size(); idx++) {
				roommate = ((RoomTuple)v.elementAt(idx)).getUser();
				if (!roommate.equals(user)) {
				    e.setUsername(roommate);
				    e.setData(user + " " + oldRoom);
				    e.publish();
				}
			    }
			}
		    } catch (Exception ex) {
			System.err.println(ex);
			return;
		    }
		}

		System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: c_unsubscribeRoom
	    else if (method.equals("c_unsubscribeRoom")) {

		System.err.println("METHOD CALL: CLIENT.C_UNSUBSCRIBEROOM");
	    	if (st.countTokens() < 2)
		    return;
		String user = st.nextToken();
		String roomUrl = st.nextToken();

		// remove user from the old room
		removeUser(roomUrl, user);

		System.err.println("END OF METHOD CALL PROCESS.");
	    }

	    // method: c_disconnect
	    else if (method.equals("c_disconnect")) {

		System.err.println("METHOD CALL: CLIENT.C_DISCONNECT");
		if (st.countTokens() < 1)
		    return;
		String user = st.nextToken();

		// notify users in the old room
		String oldRoom = null;
		Vector v = null;
		String roommate;
		oldRoom = findUserRoom(user);
		if (oldRoom != null && !oldRoom.equals("default")) {
		    v = findRoomTuple(oldRoom);
		    e.setMethod("s_leaveRoom");
		    try {
			if (v!=null) {
			    for (int idx=0; idx<v.size(); idx++) {
				roommate = ((RoomTuple)v.elementAt(idx)).getUser();
				if (!roommate.equals(user)) {
				    e.setUsername(roommate);
				e.setData(user + " " + oldRoom);
				e.publish();
				}
			    }
			}
		    } catch (Exception ex) {
			System.err.println(ex);
			return;
		    }
		}

		// remove user from the old room
		removeUser(user);

		System.err.println("END OF METHOD CALL PROCESS.");
	    }
	}
    }

    // insert a tuple into the database, if not already there.
    // all string arguments must be enclosed by '' except url.
    public boolean addRoomTuple(String url, String user) {

	user = user.toLowerCase();
	url = url.toLowerCase();

	try {
	    statement.executeQuery("insert into ROOM values ('" +
				   url + "','" +
				   user + "')");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }


    // insert a tuple into the database, if not already there.
    public boolean addMovementTuple(String room, String obj, double x0, double y0, double z0, double x1, double y1, double z1) {

	obj = obj.toLowerCase();

	try {
	    statement.executeQuery("insert into MOVEMENT values ('" +
				   room + "','" +
				   obj + "'," +
				   x0 + "," +
				   y0 + "," +
				   z0 + "," +
				   x1 + "," +
				   y1 + "," +
				   z1 + ")");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }


    // remove the tuple with the specific url from the room database
    public boolean removeRoomTuple(String url, String user) {
	user = user.toLowerCase();
	url = url.toLowerCase();

	try {
	    statement.executeQuery("delete from ROOM where URL='" +
				   url + "' AND USER='" + user + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }

    // remove the tuple with the corresponding movement url
    public boolean removeMovementTuple(String room, String obj) {
	room = room.toLowerCase();
	obj = obj.toLowerCase();

	try {
	    statement.executeQuery("delete from MOVEMENT where ROOM='" +
				   room + "' AND OBJ='" + obj + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }

    // remove the tuple with the specific user from the room database
    public boolean removeUser(String user) {
	user = user.toLowerCase();

	try {
	    statement.executeQuery("delete from ROOM where USER='" +
				   user + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }

    // remove user from a specific room
    public boolean removeUser(String url, String user) {
	user = user.toLowerCase();
	url = url.toLowerCase();

	try {
	    statement.executeQuery("delete from ROOM where URL='" +
				   url + "' AND USER='" +
				   user + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return false;
	}

	return true;
    }

    // find all users in a room
    public Vector findRoomTuple(String url) {

	url = url.toLowerCase();
	ResultSet r = null;

	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from ROOM where URL='"
				       + url + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return null;
	}

	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector tmp = RoomTuple.parseResultSet(r);

	// check database consistency
	if ( tmp == null || tmp.size() == 0 ) {
	    // System.err.println("vector: SourceTuple Not Found.");
	    return null;
	}

	return tmp;
    }

    // find the room a user is in
    public String findUserRoom(String user) {

	user = user.toLowerCase();
	ResultSet r = null;

	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from ROOM where USER='"
				       + user + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return null;
	}

	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector tmp = RoomTuple.parseResultSet(r);

	// check database consistency
	if ( tmp == null || tmp.size() == 0 ) {
	    // System.err.println("vector: SourceTuple Not Found.");
	    return null;
	}

	return ((RoomTuple)tmp.elementAt(0)).getUrl();
    }

    // find all movements of an object
    public Vector findMovementTuple(String room, String obj) {

	room = room.toLowerCase();
	obj = obj.toLowerCase();
	ResultSet r = null;

	try {
	    //System.err.println("Try to find " + url);
	    r = statement.executeQuery("select * from MOVEMENT where ROOM='" +
				       room + "' and OBJ='" + obj + "'");
	} catch(SQLException e) {
	    System.err.println(e);
	    return null;
	}

	if ( r == null) {
	    //System.err.println("null SourceTuple Not FOUND.");
	    return null;
	}

	Vector tmp = MovementTuple.parseResultSet(r);

	// check database consistency
	if ( tmp == null || tmp.size() == 0 ) {
	    // System.err.println("vector: SourceTuple Not Found.");
	    return null;
	}

	return tmp;
    }

    // find last movement of an object
    public MovementTuple findLastMovement(String room, String obj) {

	Vector v = findMovementTuple(room, obj);
	if (v == null || v.size()==0)
	    return null;

	return ((MovementTuple)v.elementAt(v.size()-1));
    }

    // Print the entire table
    public void printTable(String tableName) {
	try {
	    if (tableName.equals("ROOM")) {
		Vector v = RoomTuple.parseResultSet(statement.executeQuery("select * from " + tableName ) );

		System.out.println("\nThe current data table is:");
		System.out.println(RoomTuple.tuplesToString(v));
	    } else if (tableName.equals("MOVEMENT")) {
		Vector v = MovementTuple.parseResultSet(statement.executeQuery("select * from " + tableName ) );

		System.out.println("\nThe current data table is:");
		System.out.println(MovementTuple.tuplesToString(v));
	    }
	} catch (SQLException e) {
	    System.err.println(e);
	}
    }


    public static void main(String argv[]) {
	
	// DataServer ds = DataServer.getInstance();
	EventTracer et = EventTracer.getInstance();
	
	/* TEST 3
	et.addRoomTuple("http://www.columbia.edu", "128.59.220.1");
	et.addRoomTuple("http://www.columbia.edu", "128.59.220.2");
	
	et.addMovementTuple("http://www.columbia.edu", "http://www.columbia.edu/a.txt", 1, 2, 3, 1.1, 2.2, 3.3);
	et.addMovementTuple("http://www.columbia.edu", "http://www.columbia.edu/a.txt", 1.1, 2.2, 3.3, 1.2, 2.4, 3.6);
	System.err.println("Is Room Found? " + et.findRoomTuple("http://www.columbia.edu"));
	System.err.println("Is User Found? " + et.findUserRoom("128.59.220.1"));
	System.err.println("Is Movement Found? " + et.findMovementTuple("http://www.columbia.edu", "http://www.columbia.edu/a.txt"));
	*/

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

    }
}





