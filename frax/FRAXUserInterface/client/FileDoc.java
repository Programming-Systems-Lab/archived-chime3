/**
 * this document is for getting, sending a file 
 */

package frax.FRAXUserInterface.client;

import org.jdom.Document;
import org.jdom.Element;
import frax.FRAXUserInterface.FRAXDoc;

public class FileDoc extends FRAXDoc{
   public FileDoc( String action ){
      //add new elements
      Element root = super.getRootElement();
      root.addContent( new Element("status") )
		  .addContent( new Element("fileName") )
		  .addContent( new Element("fileContent") )
		  .addContent( new Element("user-name") ) //used for file locking verification  
		  .addContent( new Element("mode") );//used only for saving

      //set the action
      root.getChild("action").setText( action ); 
   }
}
      
