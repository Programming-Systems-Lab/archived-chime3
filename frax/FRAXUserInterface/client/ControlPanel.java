
/**
 * The main control panel for mime and plugs 
 */


package frax.FRAXUserInterface.client;

import frax.FRAXUserInterface.server.FUIConstants;
import frax.FRAXUserInterface.FRAXDoc;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JOptionPane;
import javax.swing.BorderFactory;
import javax.swing.JTextField;
import javax.swing.JList;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.MouseInputAdapter;
import javax.swing.DefaultListModel;
import javax.swing.ListSelectionModel;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.Component;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.GridLayout;
import java.awt.BorderLayout;
import java.awt.Color;
import java.util.StringTokenizer;
import java.lang.StringBuffer;
import org.jdom.Document;
import org.jdom.Element;
import javax.swing.JFileChooser;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.FileInputStream;
import java.util.NoSuchElementException;

//for debug
import org.jdom.output.*;

public class ControlPanel extends JPanel
				    implements ActionListener{
   boolean DEBUG = true;
   private JLabel fileL        = null;
   private JButton logoutB     = null;
   private JButton openB       = null;
   private JButton saveB       = null;
   private JComboBox filesCB   = null;
   //file names. note: file order must be the same as FILES in server/FUIConstants.java
   String[] files = { "Protocols", "Plugs" };
   //a model for left list
   DefaultListModel modelLeft = new DefaultListModel();
   //a list to display the data on left
   JList dataAreaLeft = new JList( modelLeft );
   //a scroll pane for the text area
   private JScrollPane scrollPaneLeft = new JScrollPane( dataAreaLeft );
   //a model for right list
   DefaultListModel modelRight = new DefaultListModel();
   //a list to display the data on left
   JList dataAreaRight = new JList( modelRight );
   //the area to display the data on right
   private JScrollPane scrollPaneRight = new JScrollPane( dataAreaRight );
   //symbolic name for opening a local file
   private String localFileName = "Local";
   //text fields for adding plugs/protocols
   JTextField typeTF = null;
   JTextField handlerTF = null;
   //the buttons for adding/removing plugs/protocols
   JButton addB = null;
   JButton removeB = null;
   //the applet that handles i/o to the server
   private FRAXClient app = null;
   //a document from the server
   private FRAXDoc docFromServer = null;
   //docs to send
   private FileDoc openFile = null;
   private FileDoc saveFile = null;

   //size
   public Dimension getprefferedSize(){
      return
	 new Dimension( 300, 160 );
   }

   public ControlPanel( ActionListener listener ){
      app = (FRAXClient)listener;
      openFile = new FileDoc("getFile");
      saveFile = new FileDoc("putFile");

      //user interface
      buildUI();

      //set action commands
      logoutB.setActionCommand( "logoutB_ControlPanel" );
      logoutB.addActionListener( listener );		     
      openB.addActionListener( this );
      saveB.addActionListener( this );
      dataAreaLeft.addListSelectionListener( new SyncronizeLists() ); 
      dataAreaLeft.addMouseListener( new RemoveFromList() );
      dataAreaRight.addListSelectionListener( new SyncronizeLists() ); 
   }

   /**
    *	build the UI 
    */
   private void buildUI(){
      setLayout( new GridBagLayout() );
      GridBagConstraints c = new GridBagConstraints();
      c.fill = GridBagConstraints.HORIZONTAL;
      c.insets.right = 5;
      fileL = new JLabel( "File:" );
      fileL.setBackground( Color.yellow );
      add( fileL, c, 0, 0, 1, 1 );
      filesCB = new JComboBox();
      for(int i=0; i<files.length; ++i)
	 filesCB.addItem( files[i] );
      //add an option to open a file from the user's computer
      filesCB.addItem( localFileName );		
      filesCB.addActionListener( this );
      add( filesCB, c, 0, 1, 1 , 1 );

      c.insets.right = 0;
      c.anchor = GridBagConstraints.WEST;
      openB = new JButton( "Open" );
      add( openB, c, 1, 0, 1, 1 );
      saveB = new JButton( "Save" );
      add( saveB, c, 1, 1, 1, 1 );
      c.fill = GridBagConstraints.VERTICAL;
      c.anchor = GridBagConstraints.EAST;
      logoutB = new JButton( "Logout" );
      logoutB.setBackground( Color.yellow );
      add( logoutB, c, 3, 0, 1, 2 );
      c.anchor = GridBagConstraints.CENTER;
      c.fill = GridBagConstraints.BOTH;
      c.weighty = 1;
      c.weightx = 1;
      //the data panel
      JPanel dataPanel = new JPanel( new GridLayout(0,2) );
      dataPanel.setBorder(BorderFactory.createTitledBorder("Configurations"));
      //set selection modes for the list
      dataAreaLeft.setSelectionMode( ListSelectionModel.SINGLE_SELECTION );
      dataAreaRight.setSelectionMode( ListSelectionModel.SINGLE_SELECTION );
      dataPanel.add( scrollPaneLeft );
      dataPanel.add( scrollPaneRight );
      //the button panel
      JPanel buttonPanel = new JPanel();
      buttonPanel.setBorder( BorderFactory.createRaisedBevelBorder() );
      buttonPanel.add( new JLabel("Type:", JLabel.RIGHT) );
      buttonPanel.add( typeTF = new JTextField(8) );
      buttonPanel.add( new JLabel("Handler:", JLabel.RIGHT) );
      buttonPanel.add( handlerTF = new JTextField(20) );
      addB = new JButton("Add");
      addB.addActionListener( new AddToList() );
      removeB = new JButton("Remove");
      removeB.addActionListener( new RemoveFromList() );
      buttonPanel.add( addB ); 
      buttonPanel.add( removeB ); 
      //main panel
      JPanel configPanel = new JPanel( new BorderLayout() );
      configPanel.add( dataPanel, "Center");
      configPanel.add( buttonPanel, "South");
      add( configPanel, c, 0, 2, 4, 1);
   }

   /**
    *	adds a component 
    */
   void add( Component c, GridBagConstraints gdc, int x, int y, int w, int h ){
      gdc.gridx = x;
      gdc.gridy = y;
      gdc.gridwidth = w;
      gdc.gridheight = h;
      add( c, gdc );
   }

   /**
    * actions 
    */

   public void actionPerformed( ActionEvent evt ){
      Object source = evt.getSource();
      if( source == openB ){
	 //check if user want to load his own file
	 if( ((String)filesCB.getSelectedItem())
	       .compareToIgnoreCase( localFileName ) == 0
	    ){
		openLocalFile();
		return;
	 }
	 //set the file
	 Element root = openFile.getRootElement();
	 root.getChild("fileName").setText( getFileSelected() );
	 //who is requesting the file?
	 root.getChild("user-name").setText( app.getUserName() );
	 //write to the server
	 if( !app.writeToServer(openFile) )
	    return;
	 //read from server
	 if( (docFromServer = (FRAXDoc)app.readFromServer()) == null )
	    return;
	 //ok, try to append the file to UI
	 root = docFromServer.getRootElement();
	 String action = root.getChild("action").getText();
	 //check for error
	 if( action.equals("error") ){
	    String errorMessage = root.getChild("errorDetails").getText();
	    showError( errorMessage );
	 }
	 else{
	    String text = root.getChild("fileContent").getText();
	    parseFile( text );
	    //we check if the file is in use, if yes alert the user
	    //user-name holds the name of the current owner of this file
	    String userName = root.getChild("user-name").getText();
	    if( !userName.equals(app.getUserName()) )
	       showInfo("The file is currently being used.\n"
		  + "You will not be able to save until "
		  + userName + " closes this file" );
	 }
      }	       
      else if( source == saveB ){
	 //check that the user does not try to save to local 
	 if( ((String)filesCB.getSelectedItem())
	       .compareToIgnoreCase( localFileName ) == 0
	    ){
		showError( "You can not save to \"" + localFileName + "\"");
		return;
	 }

	 //set the file name
	 Element root = saveFile.getRootElement();
	 root.getChild("fileName").setText( getFileSelected() );
	 root.getChild("fileContent").setText( getTextFromPanel() );
	 //who is doing the save?
	 root.getChild("user-name").setText( app.getUserName() );

	 int mode = savingMode();
	 if( mode == JOptionPane.CLOSED_OPTION )
	    return;
	 else if ( mode == JOptionPane.YES_OPTION )
	    root.getChild("mode").setText("append");
	 else
	    root.getChild("mode").setText("overwrite");
	    
	
	 //write to the server
	 if( !app.writeToServer(saveFile) )
	    showError( "File was not saved. Please reconnect." );
	 //read from server
	 if( (docFromServer = (FRAXDoc)app.readFromServer()) == null )
	    return;
	 //check for error
	 root = docFromServer.getRootElement();
	 String action = root.getChild("action").getText();
	 if( action.equals("error") ){
	    String errorMessage = root.getChild("errorDetails").getText();
	    showError( errorMessage );
	 }
      }
	    
   }
   
   /**
    * set the file name from the UI
    */

   private String getFileSelected(){
      String file = (String)filesCB.getSelectedItem();
      int i = 0;
      for( i=0; i<files.length; ++i )
	 if( file.equals(files[i]) )
	   break;
      return FUIConstants.FILES[i];
   }

   /**
    * get the text from UI
    */

   private String getTextFromPanel(){
     //each element on the left needs to match the same line on the right 	       
      StringBuffer buffer = new StringBuffer();
      for( int i=0; i<modelLeft.getSize(); ++i ){
	 buffer.append( (String)modelLeft.getElementAt(i) 
	    + "="
	    +  modelRight.getElementAt(i)  + "\n" );
      }

      return buffer.toString();
   }
	 
   /**
    * parse a file and dispaly it to user 
    */

   private void parseFile( String file ){
      //first clear the old lists
      modelLeft.clear();
      modelRight.clear(); 
      //we are looking for '=' and '\n'
      file = file.replace( '\n', '=' );
      StringTokenizer st = new StringTokenizer( file, "=" );
      //no duplicates are allowed
      boolean foundDuplicate = false;
      //temp string holders
      String type, handler;
      try
      {
	 while( st.hasMoreTokens() ){
	    type = (st.nextToken()).trim().toUpperCase();
	    //this can throw, but we must find a match to 'type'...
	    handler = (st.nextToken()).trim();	//we don't want upper case here!!
	    if( modelLeft.contains(type) )
	       foundDuplicate = true;
	       modelLeft.addElement( type );
	       modelRight.addElement( handler );
	 }
      }
      catch( NoSuchElementException nsee ){
	 showError("There was a mismatch between a 'type' and a 'handler.'\n" +
		     "Displaying partial file content");
      }

      if( foundDuplicate )
      	showError("Duplicate/s type/s were found in this file.\n" 
		     + "Please check the content!");
   }

   
   /**
    *get saving mode
    */
   private int savingMode(){
      Object[] options = {"Append", "Over write"};
      return JOptionPane.showOptionDialog(this,
	 "Would you like to append the new configuration,\n or overwrite the file?",
	 "Save Options",
	 JOptionPane.YES_NO_OPTION,
	 JOptionPane.QUESTION_MESSAGE,
	 null,     //don't use a custom Icon
	 options,  //the titles of buttons
	 options[0]); //default button title
   }

   
   /**
    * Display an error message
    */
   
   private void showError( String error ){
	 JOptionPane.showMessageDialog( this, error,
					  "Error", JOptionPane.ERROR_MESSAGE, null);
   }

   /**
    * Display Info message 
    */
   
   private void showInfo( String error ){
	 JOptionPane.showMessageDialog( this, error,
					  "Info", JOptionPane.INFORMATION_MESSAGE , null);
   }

   /**
    * Opens a file from user file system
    */

   private void openLocalFile(){
	File localFile = getLocalFile();
	if( localFile != null ){
	   if( localFile.isFile() ){ 
	      try
	      {
		 parseFile( getFileAsString(localFile) );
	      }
	      catch( IOException ioe )
	      {
		 showError(" Could not process the file (IOException).");
	      }
	   }
	}
   }

   /**
    * gets file name from local file system
    */

   private File getLocalFile(){
      JFileChooser fd = new JFileChooser();
      fd.setDialogTitle( "Configuration File" );
      int returnVal = fd.showOpenDialog( this );
      if ( returnVal == JFileChooser.APPROVE_OPTION )
	 return fd.getSelectedFile();
      return null;
   }

   /**
    * get a file as a string
    */

    private String getFileAsString( File fileName )throws IOException{
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
    * This class reacts to selections in the lists
    */

  class SyncronizeLists implements ListSelectionListener {
     public void valueChanged( ListSelectionEvent evt ){
      if( !evt.getValueIsAdjusting() ){
	 JList source = (JList)evt.getSource();
	 int index = -1; 
	 if( source == dataAreaLeft ){
	    index = source.getSelectedIndex();
	    if( index != -1 )
	       dataAreaRight.setSelectedIndex( index );
	 }
	 else{
	    index = source.getSelectedIndex();
	    if( index != -1 )
	       dataAreaLeft.setSelectedIndex( index );
	 }
      }
    }  
  }
   /**
    * This class implements the addition of a prot/plug
    */
   
   class AddToList implements ActionListener{
      public void actionPerformed( ActionEvent evt ){
	 //try to get the text fields
	 String type = ((typeTF.getText()).trim()).toUpperCase();
	 String handler  = (handlerTF.getText()).trim();
	 if( type.equals("") || handler.equals("") )
	 	return;		//do nothing if empy boxes

	 //check that this type does not exits in the list
	 int i = 0;
	 while( i<modelLeft.getSize() ){
	    if( ((String)modelLeft.getElementAt(i)).equals(type) )
	       break;
	    ++i;
	 }
	 
	 if( i < modelLeft.getSize() ){
	    showError( "This type already exist.");
	    return;
	 }

	 //add to list
	 int index = dataAreaLeft.getSelectedIndex();
	 int size = modelLeft.getSize();

	 //If no selection or if item in last position is selected,
	 //add the new type to end of list, and select it.
	 if (index == -1 || (index+1 == size)) {
	     modelLeft.addElement( type );
	     modelRight.addElement( handler );
	     dataAreaLeft.setSelectedIndex( size );
	     dataAreaRight.setSelectedIndex( size );

	 //Otherwise insert after the current selection.
	 } 
	 else {
	     modelLeft.insertElementAt( type, index+1);
	     modelRight.insertElementAt( handler, index+1);
	     dataAreaLeft.setSelectedIndex( index+1 );
	     dataAreaRight.setSelectedIndex( index+1 );
	 }

	 //clear the boxes
	 typeTF.setText("");
	 handlerTF.setText("");
      }
   }
 

   /**
    * this class implement the removal of a plug/prot
    */
   
   class RemoveFromList extends MouseInputAdapter implements ActionListener{
      public void mouseClicked( MouseEvent evt ){
	 if( modelLeft.getSize() != 0 ){
	    if( evt.getClickCount() == 2 ){
	       int index = dataAreaLeft.locationToIndex( evt.getPoint() );
	       if( index != -1 ){
		  //remove this type
		  removeType( index );
	       }
	    }
	 }
      }

      public void actionPerformed( ActionEvent evt ) {
	 int index = dataAreaLeft.getSelectedIndex();
	 if ( index  != -1 ) {
	     removeType( index );	
	 }
      }

      private void removeType( int index ){
      //alert the user before removing....
	 if( JOptionPane.YES_OPTION ==
				    JOptionPane.showOptionDialog( null, 
				       "Do you want to remove the selected type?",
				       "Remove a type", JOptionPane.YES_NO_OPTION,
				       JOptionPane.WARNING_MESSAGE, null, null, null)
	    ){
	       modelLeft.remove( index );
	       modelRight.remove( index );
	 }
      }
   }//end of RemoveFromeList

   
}


