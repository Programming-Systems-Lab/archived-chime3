

import java.lang.*;
import java.util.*;
import java.io.*;
import siena.*;

// Utility methods used by the VEM
// and also by the Dataserver
class VemUtil 
{
    public static VemNode Default [];

	// Constructor 
    public VemUtil ()
    {
		VemReadDefaults ();
    }

    // Returns the default file settings table
    public static VemNode [] GetDefault ()
    {
		return Default;
    }

    // Returns the settings for a specific file or file type
    // Returns null if the file or file type is not found
    // Should be called by the data server
    public static VemNode GetSettings (String file)
    {
		VemNode v = null;
		int i;
	
		for (i = 0; i < Default.length; i++)
		{
		    // Finds the specific file or file type
		    if (Default [i].file.compareTo (file) == 0) // First the file is found in the array
		    {
				v = new VemNode ();
		
				v.file   = file;
				v.type   = Default [i].type;
				v.sub    = Default [i].sub;
				v.object = Default [i].object;
		
				break;
		    }
		}
	
		return v;
    }

    // Reads in all of the default settings from the file "VeM_Default" 
    // and stores them in memory
    public static void VemReadDefaults ()
    {
		File f = null;
	
		f = new File ("C:\\pslroot\\psl\\chime\\vem\\VeM_Default");
	
		if (!f.exists ())
		    System.out.println ("The \"VeM_Default\" file is missing.");
	
		Long temp = new Long (f.length ());
		int fileSize = temp.intValue ();
		char [] tA = new char [fileSize];
		
		try
		{
		    BufferedReader DataInput = new BufferedReader (new FileReader (f));
		    DataInput.read (tA, 0, fileSize);
		}
		catch (IOException ioe)
		{
		    System.err.println ("IO Error: " + ioe);
		}
	
		String fS = new String (tA);
		StringTokenizer tS = new StringTokenizer (fS, " \n\r\t\n");
		
		int tokens = tS.countTokens () / 4;
	
		Default = new VemNode [tokens];
	
		for (int i = 0; i < tokens; i++)
		    Default [i] = new VemNode ();
	
		for (int i = 0; i < tokens; i++)
		{
		    Default [i].file   = tS.nextToken ();
		    Default [i].type   = tS.nextToken ();
		    Default [i].sub    = tS.nextToken ();
		    Default [i].object = tS.nextToken ();	
		}
    }
}