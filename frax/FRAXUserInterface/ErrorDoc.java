/**
 * this document is for reporting an error
 */

package frax.FRAXUserInterface;

import org.jdom.Document;
import org.jdom.Element;
import frax.FRAXUserInterface.FRAXDoc;

public class ErrorDoc extends FRAXDoc{
   public ErrorDoc( String errDetails ){
      //add new elements
      Element root = super.getRootElement();
      root.addContent( new Element("errorDetails")
			.setText(errDetails) );

      //set the action
      root.getChild("action").setText("error" ); 
   }
}
      
