/**
 *	This class implements a server for the FUI component.
 *	It listens on a port and creates a new FRAXUserInterfaceServerConnection object
 * for each new client.
 */

package frax.FRAXUserInterface.server;
import java.net.ServerSocket;
import java.lang.Integer;
import java.io.IOException;
import java.util.Timer;
import java.lang.SecurityException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.lang.StringBuffer;
import java.util.HashMap;
import java.lang.Integer;
import java.util.Collections;


public class FRAXUserInterfaceServer implements FUIConstants{
	// the socket
	private ServerSocket FUIServerSocket = null;
	// the port number. start with an Invalid port number
	int portNumber = -1;
	// a listen flag for the server(for future use)
	private boolean serverListening = true;
	// a Timer for monitering connection-time of the clients
	Timer connectionTimer = null;
	// files that a client may access 
	File[] FRAXFiles = null;
	// holds that files that are in use (overkill for two files,but may in future more files will be added?)
	HashMap lockedFiles =  new HashMap();

	public static void main( String[] args ){
		//the server
		FRAXUserInterfaceServer FUIServer = null;
		//port holds the port number passed from the user
		int port = 0;
		//valid flag for port
		boolean validPort = false;

		if( args.length != 0 ){	  
			try
			{
				port = Integer.parseInt( args[0] );			
			}
			catch( NumberFormatException nfe )
			{
				System.err.println( "Can not convert: " 
					+ args[0] + " to an Integer." );
				System.exit(1);	//exit
			}
			validPort = true;
		}

		//create the server	
		try
		{
			if( validPort )
				FUIServer = new FRAXUserInterfaceServer( port );
			else	
				//using default port number
				FUIServer = new FRAXUserInterfaceServer();
		}
		catch( IOException ioe )
		{
			System.err.println( "Can not open the port for listening:" + ioe );
			System.exit(1);	//exit
		}

		//start listening
		try
		{
			FUIServer.listen();
		}
		catch( IOException ioe )
		{
			System.err.println( "Accept failed: " + ioe );
			System.exit(1);	//exit, should we exit here?
		}
	}

	/**
	 * Default constructor. Listens on the <code>DEFAULT_PORT</code>.
	 */

	public FRAXUserInterfaceServer() throws IOException{
		this( DEFAULT_PORT );
	}

	/**
	 * Constructor with the user's <code>portNumber</code> number.
	 * Listens on <code>portNumber</code>.
	 */

	public FRAXUserInterfaceServer( int portNumber ) throws IOException{
		// check the port number.
		if( (portNumber < MIN_PORT) || (portNumber > MAX_PORT) ){
			System.err.println( "Can not use port: " + portNumber );
			System.err.println("Port must be between: " + MIN_PORT 
										+ " and " + MAX_PORT );
			System.exit(1);	//exit
		}

		this.portNumber = portNumber;

		// initilize the server
		init();
	}			

	/**
	 * Server initilization.
	 */
	 
	private void init() throws IOException{
		if( DEBUG )
			System.out.println("entering init()");

		//create the socket
		FUIServerSocket = new ServerSocket( portNumber );

		//create the timer
		connectionTimer = new Timer();

		//checking for files existance
		initFiles();

		//build hashMap on the files
		initHashMap();

		if( DEBUG )
			System.out.println("existing init()");
	}

	/**
	 * build a hash on the files
	 */

	private void initHashMap(){
		for( int index=0; index<FILES.length; ++index)
			//we use the user name to indicate that a file is in use
			lockedFiles.put( FILES[index], null );
	}

		
	/**
	 * check for the existance of the configuration files.
	 */

	private void initFiles(){
		FRAXFiles = new File[ FILES.length ] ;
		for( int index=0; index<FILES.length; ++index){
			FRAXFiles[index] = new File( FILES_PATH + FILES[index] );
			try
			{
				//check the existance of the file AND that it can be read
				if( !FRAXFiles[index].canRead() )
					// the file cannot be read or doesnot exists
					makeFile( FRAXFiles[index] );
			}
			catch( SecurityException se )
			{
				//opps, security manager denies read on the file 
				System.out.println( "Security violation:" + se );
				System.exit(1);	//exit
			}
		}
	}

	/**
	 * A configuartion file <code>file</code> does not exist, do...?
	 */

	private boolean makeFile( File file ){
		// Currently just print error message and die...
		try
		{
			System.err.println( "The file " + "\"" + file.getCanonicalPath() + "\"" + " cannot be read or doesnot exists." );
		}
		catch( IOException ie )
		{
			System.err.println( "The file " + "\"" + file.getName() + "\"" +
				" cannot be read or doesnot exits." );
		}

		System.exit(1);	//exit, TODO maybe we should create the file?? 
		return true;
	}

	/**
	 *	The contents of the file <code>fileName</code> is returned as a String. 
	 */

	public synchronized String readFile( File fileName )throws IOException{
		//the maximum number of bytes to read each time
		int bytesToRead = 1024;
		//the String buffer
		StringBuffer strBuffer = new StringBuffer(); 
		//the input stream 
		InputStreamReader is = new InputStreamReader( new FileInputStream(fileName) );
		//the buffer to read into
		char[] buffer = new char[bytesToRead];
		//the number of bytes read
		int n = 0;

		//start reading
		while( (n = is.read(buffer)) > 0 )
			strBuffer.append( buffer, 0, n );
			
		is.close();
		// return the file as a string
		return strBuffer.toString();
	}

	/**
	 *	The string content is saved into the file <code>fileName</code> 
	 */

	public synchronized void writeFile( String fileName, String content, boolean mode )throws IOException{
	    //try to open the file
	    OutputStreamWriter os = new OutputStreamWriter( new FileOutputStream(fileName, mode) );
	    os.write( content, 0, content.length() );
	    os.close();
	 }

	/**
	 * Try to associate a file with a user  
	 * returns the owner of the file
	 */

	public String setFileOwner( String file, String userName ){
	  synchronized(lockedFiles){ 
	     String owner;
	     if( (owner = (String)lockedFiles.get(file)) == null){
		 //no one is using the file
		 lockedFiles.put( file, userName );
		 return userName;
	     }
	     else return owner;
	  }
	}				     
	
	/**
	 * realse ownership of a file 
	*/
	public void releaseFileOwner( String file ){
	  synchronized(lockedFiles){
	    lockedFiles.put( file, null );
	  }
	}

	/**
	 * The function listen on <code>portNumber</code> for clients. 
	 * Creats a FRAXUserInterfacServerConnection object for each new client. 
	*/
	
	public void listen() throws IOException{

		//loop here forever listening for a new connection
		while( serverListening )
			new FRAXUserInterfaceServerConnection( 
					this, FUIServerSocket.accept() ).start();

		//clean up
		try
		{
			if( FUIServerSocket != null) 
				FUIServerSocket.close();
		}
		catch( IOException ie )
		{
			System.err.println( "Warning: socket was not closed:" + ie);
		}
	}
}	

