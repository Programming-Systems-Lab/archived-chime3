/**
 * Copyright (c) 2001: The Trustees of Columbia University 
 * in the City of New York.  All Rights Reserved.
 *
 * Xescii.java
 * (Xml Enabled Server-side Chime Information Integrator)
 *
 * @author Shen Li
 * @version 1.1
 *
 * The DataServer.
 */

package xescii;

import org.hsql.*;
import org.hsql.util.*;
import org.jdom.*;
import org.jdom.input.*;
import siena.*;
import java.io.*;
import java.sql.*;
import java.net.*;
import java.util.*;

public class Xescii implements Notifiable {
    
    // SQL statements to create the tables
    private static String[] tableCreationSQL = {		
	"create table SOURCE (id integer, protocol varchar(255), url varchar(255), size integer, type varchar(20), created varchar(255), last_mod varchar(255), src varchar(255), parent integer, shape varchar(255), opt1 varchar(255), opt2 varchar(255), opt3 varchar(255), opt4 varchar(255), opt5 varchar(255), PRIMARY KEY (id), UNIQUE (protocol, url) )",

       	"create index id on SOURCE(id)",
	"create index url on SOURCE(protocol, url)"
    };

    private Connection conn; // database connection
    private Statement statement; // sql statement
    private DatabaseMetaData meta; // database metadata
    private int current; // the largest assigned index 
    private HierarchicalDispatcher siena; // siena connection
    private Filter filter; // siena filter
    private String filterName; // filter component name
    
    // constructors
    public Xescii(String file,String usr,String pwd,String sienaURL,String fn){
	
	filterName = fn;
	
	// establish connection to the local database
	connectDB(file, usr, pwd);
	
	// establish connection to the siena server
	connectSiena(sienaURL);
	
	System.out.println("DATASERVER STARTED.");
    }
    
    public Xescii(String file, String sienaURL, String fn) {
	this(file,"sa","",sienaURL, fn);
    }

    public Xescii(String sienaURL, String fn) {
	this("DB","sa","",sienaURL, fn);
    }
    
    // shut down the data server -- destructor 
    public void shutDown() {
	// disable connection to siena server
	disconnectSiena();

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
    private void connectSiena(String url) {
	try {
	    siena = new HierarchicalDispatcher();
	    siena.setMaster(url);

	    filter = new Filter();
	    filter.addConstraint("component", filterName);
	
	    try {
		siena.subscribe(filter, this);
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
    }
    
    // disable connection to the siena server
    private void disconnectSiena() {
	try {
	    System.out.println("UNSUBSCRIBING");
	    siena.unsubscribe(filter, this);
	    siena.shutdown();
	    System.out.println("CONNECTION TO SIENA SERVER CLOSED.");
	} catch (Exception ex) {
	    ex.printStackTrace();
	    System.exit(1);
	} 
    }
    
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
    
    
    // handle an event
    public void notify(Notification e) {
	String type;
	String data = "";
	Parser p = null;
       
	if (e.getAttribute("component").stringValue().equals(filterName)) {
	    try {
		SAXBuilder builder = new SAXBuilder();
		data = e.getAttribute("data").stringValue(); 
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
					       " (link varchar(255), shape varchar(255) )");
			data = t.getOpt()[2];
			StringTokenizer st = new StringTokenizer(data, " \n");
			while ( st.hasMoreTokens() ) {
			    statement.executeQuery("insert into " + 
						   tableName + 
						   " values ('" + 
						   st.nextToken() +
						   "', null)");
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
					       " (link varchar(255), shape varchar(255) )");
			int idx1 = 0;
			int idx2 = 0;
			data = t.getOpt()[2];
			System.err.println("get into loop");
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
						   "', null)");
			    System.err.println("one iteration " + idx1 + " " + idx2);
			}
			System.err.println("out of loop");
			data = t.getOpt()[4];
			StringTokenizer st = new StringTokenizer(data, " \n");
			while ( st.hasMoreTokens() ) {
			    statement.executeQuery("insert into " + 
						   tableName + 
						   " values ('" + 
						   st.nextToken() +
						   "', null)");
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
	}
	
	printTable("SOURCE");
    }


    public void notify(Notification [] s) {
	for (int i=0; i<s.length; i++) {
	    notify(s[i]);
	}
    }
    

    // insert a tuple into the database, if not already there.
    // all string arguments must be enclosed by '' except url.
    public int addSourceTuple(String protocol, String url, int size, String type, long created, long last_mod, String src, String opt0, String opt1, String opt2, String opt3, String opt4) {
	
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
					   ",null,'" + 
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
				       ",null,'" +
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
					   ",null,null,null,null,null,null)" );
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
    
    /* find tuples corresponding to the given url
     * if a file, return a tuple containing the info of file
     * if a directory, return all contents of that directory
     */
    public Vector findSourceTupleRecur(String protocol, String url) {
	
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

    
    /* set the shape field of a tuple in the database */
    public boolean setShape(String shape, String protocol, String url) {
	
	protocol = protocol.toLowerCase();
	url = url.toLowerCase();
	ResultSet r = null;
   
	try {
	    //System.err.println("Try to set shape " + url);
	    statement.executeQuery("update SOURCE set shape='" + shape + 
				   "' where PROTOCOL='" + protocol + 
				   "' AND URL = '" + url + "'");
	} catch(SQLException e) {
	  System.err.println(e);
	  return false;
	}
	
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
    

  // add some sample tuples to the data
  public void addTestData() {
    
      addSourceTuple("http", "http://www.columbia.edu/", -1, "dir", 1999000, 1999001, null, "start node", null, null, null, null);
	
	addSourceTuple("http", "http://www.columbia.edu/~shen/", -1, "dir", 1999033, 19991, null, "2nd", null, null, null, null);
	
	addSourceTuple("http", "http://www.columbia.edu/abc.doc", 23, "doc", 1990, 199901, null, "2nd", "file", null, null, null);
	
	addSourceTuple("http", "http://www.columbia.edu/abc.pdf", 3333, "pdf", 1999000, 1999001, "ftp://122.22.22.2/abc.pdf", "2nd", null, "link", null, null);
	
	addSourceTuple("http", "http://www.columbia.edu/~shen/abc/abc.pdf", 12, "pdf", 1, 2, null, "3rd", null, null, null, null);
	
	addSourceTuple("ftp", "ftp://www.columbia.edu/~shen/abc/abc.pdf", 12, "pdf", 1, 2, null, "3rd", null, null, null, null);	
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
    
    String readFile(String file) {
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
    
    void writeFile(String file,String text) {
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
	if (argv.length != 3) {
	    System.err.println("3 Command-Line Arguments Expected for the DataServer.");
	    System.exit(-1);
	}
		    
	//Xescii ds = new Xescii("DB", "senp://localhost:1111", "FRAX");
	// argv[0] is the database name, ie. "DB"
        // argv[1] is the siena server IP, ie. "senp://localhost:1111"
        // argv[2] is the siena event id, ie. "FRAX"
	Xescii ds = new Xescii(argv[0], argv[1], argv[2]);
	
	/* 
	  System.out.println("");
	  System.out.println(ds.getDomain("http://www.columbia.edu/~shen/abc"));
	  System.out.println(ds.getDomain("ftp://160.39.226.31"));
	  System.out.println(ds.getDomain("Localhost://C:/"));
	  System.out.println(ds.getDomain("sql:select *"));
	  System.out.println(ds.getDomain("telnet://cunix/abc"));
	  System.out.println(ds.getDomain("ftp://"));
	*/
	
	// ds.addTestData();
	
	// ds.printTable();
	
	//System.out.println("\nFind File:");
	//System.out.println(SourceTuple.tuplesToString(ds.findSourceTupleRecur("http://www.columbia.edu/abc.doc")));

	//System.out.println("\nFind Directory:");
	//System.out.println(SourceTuple.tuplesToString(ds.findSourceTupleRecur("http://www.columbia.edu/")));
	//System.out.println("\nAfter Remove Directory:");
	//ds.removeSourceTuple("http://www.columbia.edu/~shen/");
	//ds.printTable();
	
	//	ds.shutDown();
    }
}






