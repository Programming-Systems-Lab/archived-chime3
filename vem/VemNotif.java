package psl.chime.vem;

import java.lang.*;
import java.util.*;
import java.io.*;
import siena.*;

// Part of VEM client on the Data Server Side
// Used to parse the recieved events from the
// siena server and then take the appropriate action
public class VemNotif implements Notifiable
{
    public static VemNode Default [];
    VemUtil util;
    public Siena siena;
	private static VemNotif myself;

	//create the Vem Notification buffer
	public static synchronized VemNotif getInstance() {
		if (myself == null)
		    myself = new VemNotif();
		return myself;
	}


	//create the Vem Notification buffer from Christian's Subscriber
	public static synchronized VemNotif getInstance(Siena s) {
			if (myself == null)
			    myself = new VemNotif();

			myself.setSiena(s);
			return myself;
	}

	//create a VEM object - the Singleton object we're going to keep around
	private VemNotif()
	{
		util = new VemUtil ();
		Default = util.GetDefault ();
	}


	//set the siena of this notification
	private void setSiena(Siena s) {
		siena = s;
	}

	//get the shape associated with some object
	//otherwise you will get null
	public VemObject getShape(String protocol, String url) {
		for (int i = 0; i < Default.length; i++) {
			if (url.indexOf(Default[i].type) != -1) {
				VemObject obj = new VemObject();
				obj.setUrl(url);
				obj.setProtocol(protocol);
				obj.setShape(Default[i].object);
				obj.setClasstype(Default[i].type);
				obj.setSubclass(Default[i].sub);
				return obj;
			}
		}


		//return null; this doesn't seem to be adequate according to Shen's standard so I am making a default
		//first object in the list is the default
		VemObject obj = new VemObject();
		obj.setUrl(url);
		obj.setProtocol(protocol);
		obj.setShape(Default[0].object);
		obj.setClasstype(Default[0].type);
		obj.setSubclass(Default[0].sub);
		return obj;
	}


	//get the shape description associated with this object url
	public VemObject getLinkShape(String roomUrl, String objUrl) {
		for (int i = 0; i < Default.length; i++) {
			if (objUrl.indexOf(Default[i].type) != -1) {
				VemObject obj = new VemObject();
				obj.setRoomUrl(roomUrl);
				obj.setObjUrl(objUrl);
				obj.setShape(Default[i].object);
				obj.setClasstype(Default[i].type);
				obj.setSubclass(Default[i].sub);
				return obj;
			}
		}

		//return null; this doesn't seem to be adequate according to Shen's standard so I am making a default
		//first object in the list is the default
		VemObject obj = new VemObject();
		obj.setRoomUrl(roomUrl);
		obj.setObjUrl(objUrl);
		obj.setShape(Default[0].object);
		obj.setClasstype(Default[0].type);
		obj.setSubclass(Default[0].sub);
		return obj;
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