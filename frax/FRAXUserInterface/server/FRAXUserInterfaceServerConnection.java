/**
 * This class implements a connection to a client.
 *
 */

package frax.FRAXUserInterface.server;
import frax.FRAXUserInterface.Protocol;
import java.net.Socket;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.InterruptedIOException; 
import org.jdom.input.SAXBuilder;
import org.jdom.output.XMLOutputter;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.JDOMException;
import org.jdom.output.*;

public class FRAXUserInterfaceServerConnection extends Thread implements FUIConstants{
	// the server
	FRAXUserInterfaceServer server = null;
	// the socket to the client
	private Socket FUIClientSocket = null;
	// the protocol that this client uses
	private Protocol fuiProt = null;
	// the last time that the client sent data 
	private long lastConnectionTime = System.currentTimeMillis(); 
	// a lock to suspends connectionTimer Timer from executing its task
	private boolean suspendTimer = false;
	//current file that that this user owns
	private String file = "";

	/**
	 * Construcotr. Takes a server and a client socket.
	 */

	FRAXUserInterfaceServerConnection( FRAXUserInterfaceServer server,
						  Socket FUIClientSocket ){
		super( "FUIServerThread" );
		this.server = server;
		this.FUIClientSocket = FUIClientSocket;

		//establish a protocol for this client
		fuiProt = new FUIProtocol( this );

		if(DEBUG)
			System.out.println("New Connection...in FUISC");
	}

	/**
	 * returns the last time a client sent a message.
	 */

	public synchronized long getLastConnection(){
		return lastConnectionTime;
	}

	/**
	 * sets the last time a client sent a message.
	 */

	 public synchronized void setLastConnection( long currentTime ){
		lastConnectionTime = currentTime;
	}

	/**
	 * suspends the timer for this client; i.e. locks <code>suspendTimer</code>.
	 */

	 public synchronized void suspendConnectionTimer(){
		suspendTimer = true;
	}

	/**
	 * resumes the timer for this client; i.e. unlocks <code>suspendTimer</code>.
	 */

	public synchronized void resumeConnectionTimer(){
		suspendTimer = false;
	}

	/**
	 * check the if ConnectionTimer is suspended
	 */

	public synchronized boolean isSuspended(){
		return suspendTimer;
	}

	public String getFile(){
		return file;
	}

	public void setFile( String f ){
		file = f;
	}
		
	public void run(){
		// the doc recieved from client	
		Document docFromClient = null;
		// the doc to send to the client
		Document docFromServer = null;
		// stream to read from
		ObjectInputStream in = null;
		// stream to write to
		ObjectOutputStream out = null;

		// start monintering connection-time for this client
		server.connectionTimer.schedule( new MonitorConnection(this),
			IDLE_CONNECTION_TIME, IDLE_CONNECTION_TIME );
		
		try
		{
			in = new ObjectInputStream( FUIClientSocket.getInputStream() );
			out = new ObjectOutputStream( FUIClientSocket.getOutputStream() ); 
			
			/***************************************
			wait here to process requests from the client
			***************************************/

			while( (docFromClient = (Document)in.readObject()) !=null ){
				//we have input...suspend the timer
				suspendConnectionTimer();

				//process the input
				docFromServer = fuiProt.process( docFromClient );

				//send result back to client
				out.writeObject( docFromServer );

				//update the last connection time
				setLastConnection( System.currentTimeMillis() );

				//finished processing...resume the timer
				resumeConnectionTimer();
			
				Thread.sleep( 1000 );//not really necessary...we will block on I/O
			}//end while

		}
		catch(ClassNotFoundException e)
		{
			if(DEBUG)
				System.out.println("ClassNotFound in FUISC");
		}
		catch(InterruptedException e)
		{
			if(DEBUG)
				System.out.println("interrupted in FUISC");
		}
		catch(InterruptedIOException e)
		{
			if(DEBUG)
				System.out.println("interrupted IO in FUISC");
		}
		catch(IOException e)
		{
			System.out.println( e );
			if(DEBUG)
				System.out.println( "IOException in FUISC");
		}
		finally
		{
			try
			{
				if( in != null)
					in.close();
				if( out != null)
					out.close();
				FUIClientSocket.close();
			}
			catch(IOException e)
			{
				System.err.println( e );
			}
			//release any locked files
			if( !file.equals("") )
				server.releaseFileOwner( file );

			if(DEBUG)
				System.out.println("Leaving finally, in FRAXUserInterfaceConnection");
		}
	} //end of run()
}
