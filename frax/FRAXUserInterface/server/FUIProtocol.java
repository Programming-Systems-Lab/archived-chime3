/**
 	This class defines the protocol for FUI.
  
  The class processes a FRAXDoc doucment(a representation of an XML data in java)
  (using process()) that was sent from a FUI client. 
  
  The document that is expected has the following format:

  <FUIMessage>
		<action> an action describtion </action>
		(details)
  </FUIMessage>

  The root elemet must be FUIMessage.
  The docement must contain an action element.
  (details): are further element that are needed for processing the docuement.
  For example: <fileName> name of a file</fileName>.

  porcess() returns a document in the same format.
*/
 
package frax.FRAXUserInterface.server;
import frax.FRAXUserInterface.Protocol;
import frax.FRAXUserInterface.ErrorDoc;
import org.jdom.Document;
import org.jdom.Element;
import java.io.IOException;
import java.io.File;
import org.jdom.output.*;

class FUIProtocol extends Protocol implements FUIConstants{
     //a reference to a FRAXUserInterfaceServerConnection
     FRAXUserInterfaceServerConnection fuisc = null;
     Document outputDoc = null; 

     /**
      * Constructor takes an instance to FRAXUserInterfaceServerConnection object
      */

     public FUIProtocol( FRAXUserInterfaceServerConnection fuisc ){
	     this.fuisc = fuisc;
     }

     /**
      * processes a login request
      */

     private Document processLogin( Document inputDoc ){
	     //this functin needs to use CHIME login capability
	     //for now return ok status...
	     Element root = inputDoc.getRootElement();
	     String user = root.getChild( "user-name" ).getText();
	     String password = root.getChild( "password" ).getText();
	     user = user.trim(); password=password.trim();
	     if( user.equals("Mrx") & password.equals("xxx") )
		root.getChild( "status" ).setText( "ok" ) ;
	     else if( user.equals("Mry") & password.equals("yyy") )
		root.getChild( "status" ).setText( "ok" ) ;
	     else
		root.getChild( "status" ).setText( "bad" );
	     return inputDoc;
     }
	     
     /**
      * process a file request
      */

     private Document processGetFile( Document inputDoc ){
	     try
	     {
		     Element root = inputDoc.getRootElement();
		     String fileName = root.getChild( "fileName" ).getText();
		     fileName = fileName.toLowerCase().trim();
		     String userName = root.getChild( "user-name" ).getText();

		     ////////////////////////////////////////////////////////
                     // locking a file
                     ////////////////////////////////////////////////////////

		     //release any file ownership first
		     String myFile = fuisc.getFile();
                     if( !myFile.equals("") ){	
			     fuisc.server.releaseFileOwner( myFile );
			     fuisc.setFile("");
		     }
			     
		     //try to set ownership on this file
		     String owner = fuisc.server.setFileOwner( fileName, userName );
		     if( owner.equals(userName) )
			//ownership established, keep track of it 
			fuisc.setFile( fileName );
		     else
			//notify to the user who is the owner
			root.getChild( "user-name" ).setText( owner );

		     ///////////end of locking //////////////////////////////

		     //holds file content
		     String fileContent = "";
		     try
		     {
			     int i = 0;
			     //try to get the file content
			     search_file:
			     for( i = 0; i < FILES.length; ++i )
				     if( FILES[i].equals(fileName) ){
					     fileContent = fuisc.server.readFile( 
					     new File( FILES_PATH + fileName) );
					     break search_file;
				     }

			     //build a document
			     if( i < FILES.length ){
				     //add the content of this file
				     root.getChild("fileContent")
				     .setText(fileContent) ;
				     return inputDoc;
			     }
			     else
				      return  new ErrorDoc( "file " + fileName + " was not found" );
		     }
		     catch( IOException e )
		     {
			     return new ErrorDoc( "file " + fileName + " could not be read" );
		     }

	     }
	     catch( NullPointerException npe)
	     {
		     //opps no fileName element
		     return new ErrorDoc( "fileName element doesnot exist" );
	     }
     }		


     /**
      *  the function tries to save the content of a file into a file.
      */
      
      private Document processPutFile( Document inputDoc ){
      try
	     {
		     Element root = inputDoc.getRootElement();
		     String fileName = root.getChild( "fileName" ).getText();
		     fileName = fileName.toLowerCase().trim();
		     //holds file content
		     String fileContent = root.getChild("fileContent").getText();
		     String userName = root.getChild("user-name").getText();
		     //get mode
		     boolean mode = false;
		     if( ((root.getChild("mode").getText())).equals("append") )
			mode = true;

		     /////////////////////////////////////////////////////////////////////
		     //    before saving the file check that the user owns the file     //
		     /////////////////////////////////////////////////////////////////////
		     //try to set ownership on this file
		     String owner = fuisc.server.setFileOwner( fileName, userName );
		     if( owner.equals(userName) )
			//ownership established, keep track of it 
			fuisc.setFile( fileName );

		     String file = fuisc.getFile();
		     if( !file.equals(fileName) )
			return  new ErrorDoc( "You do not own this file. The file was not saved.\n"
						+ "(" + owner + " is the current owner)" );

		     try
		     {
			     int i = 0;
			     //try to get the file content
			     search_file:
			     for( i = 0; i < FILES.length; ++i )
				     if( FILES[i].equals(fileName) ){
					     fuisc.server.writeFile( 
						FILES_PATH + fileName,
						fileContent,
						mode );
					     break search_file;
				     }

			     // 
			     if( i < FILES.length ){
				     //add the content of this file
				     root.getChild("status")
				     .setText("ok") ;
				     return inputDoc;
			     }
			     else
				      return  new ErrorDoc( "file " + fileName + " was not found" );
		     }
		     catch( IOException e )
		     {
			     return new ErrorDoc( "file " + fileName + " could not be written" );
		     }

	     }
	     catch( NullPointerException npe)
	     {
		     //opps no fileName element
		     return new ErrorDoc( "fileName element doesnot exist" );
	     }
      }
	     
     /**
      * this function prcess a Document from a client
      */
      
     public Document process( Document inputDoc ){
	     try
	     {
		     Element fuiMessage = inputDoc.getRootElement();
		     String action = fuiMessage.getChild( "action" ).getText();
		     action = action.toLowerCase().trim();
	     if(DEBUG)
	       System.out.println(" action: " + action );
		     //process the action
		     if( action.equals("login") )
			     outputDoc = processLogin( inputDoc );	
		     else if( action.equals("getfile") )
			     outputDoc = processGetFile( inputDoc );	
		     else if( action.equals("putfile") )
			     outputDoc = processPutFile( inputDoc );
		     else
			     outputDoc = new ErrorDoc( "action was not recognized" );

		     return outputDoc;
	     }
	     catch( NullPointerException npe )
	     {
		     //the document is not in a fui format

		     if(DEBUG)
			     System.err.println( "Caught Null exception in method process" );

		     return new ErrorDoc( "action element doesnot exist" );
	     }
     }
}
