/**
 * this document holds login info
 */

package frax.FRAXUserInterface.client;

import org.jdom.Document;
import org.jdom.Element;
import frax.FRAXUserInterface.FRAXDoc;

public class LoginDoc extends FRAXDoc{
   public LoginDoc(){
      //add new elements
      Element root = super.getRootElement();
      root.addContent( new Element("status") )
		  .addContent( new Element("user-name") )
		  .addContent( new Element("password") )
		  .addContent( new Element("host") )
		  .addContent( new Element("port") );

      //set the action
      root.getChild("action").setText("login"); 
   }
}
      
