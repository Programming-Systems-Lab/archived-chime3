/**
 * main applet
 */

package frax.FRAXUserInterface.client;

import javax.swing.JApplet;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.util.*;
import org.jdom.Element;
import org.jdom.Document;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Dimension;
import frax.FRAXUserInterface.client.LoginPanel;
import frax.FRAXUserInterface.client.ControlPanel;
import frax.FRAXUserInterface.client.SocketOpener;
import frax.FRAXUserInterface.client.LoginDoc;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.InterruptedIOException;
//for debug
import org.jdom.output.*;

public class FRAXClient extends JApplet 
                                      implements 
                                                 ActionListener {
   boolean DEBUG = true;
   //streams
   private ObjectOutputStream os = null;
   private ObjectInputStream is = null;
   //a socket to the server
   Socket socket = null;
   //port to connect too
   int port = -1; 
   // connection to server flag
   boolean connected = false;
   //a document for getting user login info
   LoginDoc loginDoc = null;
   //the document that the server sent
   Document docFromServer = null;
   //interface panels
   LoginPanel login     = null;			
   ControlPanel control = null;
   //content pane of this applet
   JPanel contentPane = null;
   //host name
   String host = "";
   //user name
   String userName = "";

   public void init() {
        //Get the address of the host we came from.
        //host = getCodeBase().getHost();
	//login doc
	loginDoc  = new LoginDoc();
	//buid UI
        buildUI();
        setVisible( true );
      }

    public void start(){
	login.setVisible( true );
	control.setVisible( false );
	contentPane.revalidate();
    }

    public void stop() {
	 connected = false;
	 try 
	 { //Close input stream.
            if (is != null) {
                is.close();
                is = null;
	    }
         }
         catch (Exception e) {} //Ignore exceptions.

         try
	 { //Close output stream.
            if (os != null) {
                os.close();
                os = null;
	    }
         }
         catch (Exception e) {} //Ignore exceptions.

	 try
	 { //Close socket.
            if (socket != null) {
                socket.close();
                socket = null;
	    }
         }
	 catch (Exception e) {} //Ignore exceptions
     }
            

    public synchronized void actionPerformed( ActionEvent event ) {
      String command = event.getActionCommand();
      if( command.equals("loginB_LoginPanel") ){
	 login.setEditable( false );
	 //check the data from the panel
	 if( !getLoginData() ){
	    login.setEditable( true );
	    return;
	 }
	 if( !connected )//open a connection
	    openConnection();
	 //check if connection succeded
	 if( !connected ){
	    login.setEditable( true );
	    return;
	 }
	
	 if(DEBUG)
	    System.out.println("About to send data");
	 //send data
	 if( !writeToServer(loginDoc) ){
	    login.setEditable( true );
	    return; 
	 }
	 if( (docFromServer = readFromServer()) == null ){
	    login.setEditable( true );
	    return;
	 }
	 //check the return doc
	 if(DEBUG)
	    System.out.println("About to check the returned document");
	 Element root = docFromServer.getRootElement();
	 String status = root.getChild( "status" ).getText();
	 System.out.println("status:"+status+":");
	 if( status.equals("ok") ){
	    login.setVisible( false );
	    login.blank();
	    control.setVisible( true );
	    contentPane.validate();
	 }
	 login.setEditable( true );
      }//end login..
      else if ( command.equals("logoutB_ControlPanel") ){
	 /****** we disconnect here!!! *****/
	 stop();
	 start();
      }
   }

   /**
    * get the data from the login panel and check it
    */

   private boolean getLoginData(){
      StringBuffer error = new StringBuffer();
      //get the data
      login.getInfo( loginDoc );
      Element root = loginDoc.getRootElement();
      if( ((root.getChild("user-name")).getText()).trim().equals("") )
	 error.append( "Enter a user name\n"  );
      if( ((root.getChild("password")).getText()).trim().equals("") )
	 error.append( "Enter a password\n" );
      host = (root.getChild("host")).getText().trim();
      if( host.equals("") )
	 error.append( "Enter a host name\n" );
      try
      {
	 port = Integer.parseInt( (root.getChild("port")).getText() );
	 if( port < 0 )
	    error.append( "Enter a positive port number" );
      }
      catch ( NumberFormatException e ) 
      {
	 error.append( "Enter a positive port number" );
      }

     if( error.length() > 0 ){ 
	showError( error.toString() );
	return false;
	}
      else{
	 userName = ((root.getChild("user-name")).getText()).trim(); 
	 return true;
      }
   }

   /**
    * this function tries to make a connection to the server
    */
   
   synchronized private void openConnection(){
      //initialized the socket; time out after 10 seconds
      socket = SocketOpener.openSocket( host, port , 10000 );
      if( socket == null ){
	showError( "Could not open a socket\n"  
		     + "(check hostname and port number)" );
	return;
      }
      //open the streams
      try
      {
	 os = new ObjectOutputStream( socket.getOutputStream() );
	 is = new ObjectInputStream( socket.getInputStream() );
      }
      catch( IOException ioe )
      {
	 showError( "Opened a socket, but can't open a stream on it");
	 stop();
	 return;
      }
      
      if ( (os != null) & (is != null) ){
	 if (DEBUG) {
	     System.out.println("socket = " + socket);
	     System.out.println("output stream = " + os);
	     System.out.println("input stream = " + is);
	 }
	 //set a the connection flag
	 connected = true;
      }else{
	 showError("Valid port, but communication failed. Try again.");
      }
   }

   /**
    * write data to the server
    * return true on success, otherwise false
    */
   synchronized public boolean writeToServer( Document doc ){
      if(DEBUG){
        System.out.println(":::::: in writeToServer ::::::::");
      	try{
	     XMLOutputter out = new XMLOutputter();
	     out.output( doc, System.out);
	}catch(Exception e){}
      }
      //try to write
      try
      {
	 os.writeObject( doc );
	 os.flush();
	 os.reset();
      }
      catch( IOException ioe )
      {
	 showError("Failed to write to the socket.");
	 stop();
	 return false;
      }
      catch( NullPointerException ie )
      {
	 showError("No output stream!");
	 stop();
	 return false;
      }
      
      return true;
   }
   
   /**
    * read data from the server
    * return a doc on success, otherwise null
    */

   synchronized public Document readFromServer(){
      Document doc = null;
      //set a time out for socket!
      try
      {
	 socket.setSoTimeout( 20000 );
      }
      catch( SocketException se )
      {
	 showError( "Could not set timeout on the socket");
	 return doc;
      }
   //read
      try
      {
	 doc = (Document)is.readObject();
      }
      catch( ClassNotFoundException cnf )
      {
	 //this should not happend. We always should read a jdom object!
	 return null;
      }
      catch( InterruptedIOException ie )
      {
	 showError("Timed out. Failed to read from the socket");
	 return null;
      }
      catch( IOException ioe )
      {
	 showError( "Failed to read from the socket");
	 return null;
      }

      return doc;
   }

   /**
    * Build UI 
    */
   
   private void buildUI(){
	contentPane = new BackgroundPanel();
	contentPane.setLayout( new BorderLayout() );
        contentPane.setBackground( new Color(103, 172, 241) );
	//initialize panels
        login = new LoginPanel( this );
	control = new ControlPanel( this );
	contentPane.add( login, "North" );
	contentPane.add( control, "Center" );
	//control.setVisible( false );
	setContentPane( contentPane );
    } 

   /**
    * get the user name 
    */
   public String getUserName(){
      return userName;
   }

   /**
    * Display an error message
    */
   
   private void showError( String error ){
	 JOptionPane.showMessageDialog( this, error,
					  "Error", JOptionPane.ERROR_MESSAGE, null);
   }
}

/**
 * the class acts as a background panel for the FraxClient.
 */

class BackgroundPanel extends JPanel {
    public Insets getInsets() {
        return new Insets(4,4,5,5);
    }

    public void paintComponent(Graphics g) {
	super.paintComponent( g );
        Dimension d = getSize();
        g.setColor( getBackground() );
        g.draw3DRect(0, 0, d.width - 1, d.height - 1, true);
        g.draw3DRect(3, 3, d.width - 7, d.height - 7, false);
    }
}
