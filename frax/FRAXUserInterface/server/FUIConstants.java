/**
 * Constants for FUI 
 */

package frax.FRAXUserInterface.server;
import java.io.File;

public interface FUIConstants{
	// for debuging
   final boolean DEBUG = true;
	//the default port number
	public static final int DEFAULT_PORT = 6677;
	//the minimum port number
	public static final int MIN_PORT = 0x0;
	//the maximum port number 
	public static final int MAX_PORT = 0xffff;
	//the maximum idle connection time allowed for a client(in miliseconds)
	public static final long IDLE_CONNECTION_TIME = 30*60*1000;
	//configuration files of FRAX
	public static final String[] FILES = new String[] {  "prot.conf", "plug.conf" };
	//Path to the prot.conf and plug.conf from the FRAXUserInterface/Server
	public static final String FILES_PATH = 
											".." + File.separator + ".." + File.separator;
	//index of the configuration file in FILES
	public static final int PROT_INDEX = 0;
	public static final int PLUG_INDEX = 1;
	
}
