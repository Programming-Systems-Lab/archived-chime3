/**
 * The login panel for login in
 */
package frax.FRAXUserInterface.client;

import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPasswordField;
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import org.jdom.Document;
import org.jdom.Element;
import java.awt.Graphics;
import javax.swing.BorderFactory;
import javax.swing.border.Border;

public class LoginPanel extends JPanel{
   private JLabel userNameL = null;
   private JLabel passwordL = null;
   private JLabel hostL     = null;
   private JLabel portL     = null;
   private JTextField userNameTF = null;
   private JPasswordField passwordTF = null;
   private JTextField hostTF     = null;
   private JTextField portTF     = null;
   private JButton loginB        = null;
   //the main panel
   private JPanel loginPanel = new JPanel();
   //the user data panel
   private JPanel userDataPanel = new JPanel();
   
   //size
   public Dimension getprefferedSize(){
      return
	 new Dimension( 100, 75 );
   }

   public LoginPanel( ActionListener listener ){
      userNameL = new JLabel( "Username:", JLabel.RIGHT );
      passwordL = new JLabel( "Password:", JLabel.RIGHT );
      hostL     = new JLabel( "Host:", JLabel.RIGHT );
      portL     = new JLabel( "Port:", JLabel.RIGHT );
      passwordTF = new JPasswordField( "", 20 );
      userNameTF = new JTextField( "", 20 );
      hostTF     = new JTextField( "", 20 );
      portTF     = new JTextField( "", 20 );
      loginB     = new JButton( "Login" );

      //echo off on the password TF
      passwordTF.setEchoChar( '#' );


      loginPanel.setLayout( new BorderLayout(2, 0) );
      userDataPanel.setLayout( new GridLayout(4, 2, 3, 2) );
      
      userDataPanel.add( userNameL );
      userDataPanel.add( userNameTF);
      userDataPanel.add( passwordL );
      userDataPanel.add( passwordTF);
      userDataPanel.add( hostL );
      userDataPanel.add( hostTF );
      userDataPanel.add( portL );
      userDataPanel.add( portTF );
      
      loginPanel.add( userDataPanel, "Center" );
      loginB.setActionCommand( "loginB_LoginPanel" );
      loginB.addActionListener( listener );		     
      JPanel hold = new JPanel();
      hold.add( loginB );
      loginPanel.add( hold , "South" );

      //border, to make it nice...
      Border raised = BorderFactory.createRaisedBevelBorder();
      loginPanel.setBorder( raised );

      setBackground( new Color(103, 172, 241) );
      //add the new login
      add( loginPanel );
      
   }

   /**
    *	sets the Panel to editable or not editble
    */
   public void setEditable( boolean key ){
      if( key ){
	 userNameTF.setEditable( key );
	 passwordTF.setEditable( key);
	 hostTF.setEditable( key );
	 portTF.setEditable( key );
	 loginB.setEnabled( key );
      }
      else{
	 userNameTF.setEditable( key );
	 passwordTF.setEditable( key);
	 hostTF.setEditable( key );
	 portTF.setEditable( key );
	 loginB.setEnabled( key );
      }

      validate();
   }

   /**
    *	read the values which were intered by the user 
    */
  public void getInfo( LoginDoc doc ){
     Element root = doc.getRootElement();
     root.getChild( "user-name" ).setText( userNameTF.getText() );
     root.getChild( "password" ).setText( new String(passwordTF.getPassword()) );
     root.getChild( "host" ).setText( hostTF.getText() );
     root.getChild( "port" ).setText( portTF.getText() );
  } 
   
  public void blank(){
     userNameTF.setText("");
     passwordTF.setText("");
     hostTF.setText("");
     portTF.setText("");
  }
}
     
