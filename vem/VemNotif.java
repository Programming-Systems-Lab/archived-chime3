package psl.chime.vem;

import java.lang.*;
import java.util.*;
import java.io.*;
import siena.*;

// Part of VEM client on the Data Server Side
// Used to parse the recieved events from the
// siena server and then take the appropriate action
class VemNotif implements Notifiable
{
    public static VemNode Default [];
    VemUtil util;
    Siena siena;

    public VemNotif (Siena s)
    {
		util = new VemUtil ();
		Default = util.GetDefault ();
	 	siena = s;
    }

    // Recieves the incoming events from the client
    public void notify (Notification e)
    {
		StringTokenizer t;
		String n, file = "", type = "", sub = "", object = "";

		n = e.toString ();

		n = n.replace ('=', ' ');
		n = n.replace ('"', ' ');
		n = n.replace ('{', ' ');
		n = n.replace ('}', ' ');
		n = n.replace (':', ' ');

		t = new StringTokenizer (n, " \n\t\r");

		if (t.countTokens () != 11)
		    System.out.println ("Incorrect Number of Event Type");
		else
		{
		    // Parse out the four necessary parameters
		    t.nextToken ();
		    t.nextToken ();
		    t.nextToken ();
		    t.nextToken ();
		    file = t.nextToken ();
		    t.nextToken ();
		    object = t.nextToken ();
		    t.nextToken ();
		    sub = t.nextToken ();
		    t.nextToken ();
		    type = t.nextToken ();

		    if (UpdateNode (file, type, sub, object))
				System.out.println ("Update Made"); // Call data server method and pass along new parameters
		    else
				System.out.println ("Update Not Made"); // Do nothing
		}
    }

	// Used when more than one Notification is recieved
    public void notify (Notification [] s)
    {
		for (int i = 0; i < s.length; i++)
	    	notify (s [i]);
    }

    // Updates the Default table when an event is recieved from the client
    // Checks if the recieved info is different from the existing settings
    // Returns true if a change was made and false if no change was made
    public static boolean UpdateNode (String file, String type, String sub, String object)
    {
		boolean flag = false;
		int i;

		for (i = 0; i < Default.length; i++)
		{
		    // First the file is found
		    // Works for both *.any and for file.any
		    if (Default [i].file.indexOf (file) != -1) // First the file is found
		    {
				if (Default [i].type.compareTo (type) != 0) // If type does not match then update
				{
				    Default [i].type = type;
				    flag = true;
				}

				if (Default [i].sub.compareTo (sub) != 0) // If sub-type does not match then update
				{
				    Default [i].sub = sub;
				    flag = true;
				}

				if (Default [i].object.compareTo (object) != 0) // If object does not match then update
				{
				    Default [i].object = object;
				    flag = true;
				}
		    }
		}

		return flag;
    }

}