/*
 * msoFRAXPlug.java
 *
 * Created on February 1, 2002, 9:21 PM
 */

/**
 *
 * @author  Rean Griffith
 * @version 
 */

package psl.chime.frax.plugs.mso;

import java.io.*; // import IO classes
import siena.*;	// import siena classes
import java.lang.Integer; 

public class msoFRAXPlug extends FRAXPlug
{
    /* Siena Constants */
    private final String SIENA_SERVER = "senp://localhost:";
    private final int SIENA_PORT = 1234;
    private final String CONSTRAINT_NAME = "OfficeDoc";
    private final String CONSTRAINT_VALUE = "FRAXOfficeDoc";
    private final String TARGET_DOC = "TargetDoc";
	
    /* Constructor 

    Description:    creates an instance of an msoFRAXPlug
    Parameters:     strURL - URL to file
    Return Value:   n/a
    Error(s):       n/a	

    */
    public msoFRAXPlug( String strURL )
    {
        try
        {	
            if( strURL.length() > 0 )
            {
                ThinClient siena;
                String strSienaParam = SIENA_SERVER + new Integer(SIENA_PORT).toString();
                siena = new ThinClient( strSienaParam );
                Notification n = new Notification();

                // set constraint properties
                n.putAttribute( CONSTRAINT_NAME,CONSTRAINT_VALUE );
                n.putAttribute( TARGET_DOC, strURL );
                // publish     
                siena.publish(n);
            }
         }
         catch(Exception e ) // catch all exception handler 
         {
            e.printStackTrace();
         }
    }// end of constructor
 
    public String toXML() 
    {
	return new String("");
    }
    
    public static void main (String args[]) 
    {
        msoFRAXPlug mso = new msoFRAXPlug( "http://www.xyz.com" );
    }
   
}



