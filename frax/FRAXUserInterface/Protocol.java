/**
 *	This class is an abstraction for a possible protocol.
 */

package frax.FRAXUserInterface;
import org.jdom.Document;

public abstract class Protocol{
	public abstract Document process( Document doc );
}
