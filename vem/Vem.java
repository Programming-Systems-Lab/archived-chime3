package psl.chime.vem;

import java.lang.*;
import java.util.*;
import java.io.*;
import siena.*;

// Main Vem class
public class Vem extends Thread
{
    public static HierarchicalDispatcher siena;

    // Constructor
    public Vem () {}

    // Checks if the passed along host exists and sets it
    public static void VemSetHost (String uri)
    {
		Runtime.getRuntime ().addShutdownHook (new Thread ()
		{
		    public void run ()
		    {
				System.err.println ("VemSubscriber shutting down");
				siena.shutdown ();
		    }
		}
	);

	try
	{
	    siena = new HierarchicalDispatcher ();

	    if (uri != null)
			siena.setMaster (uri);
	    else
	    {
			System.err.println ("can't set empty siena location");
			throw new SienaException ();
	    }
	}
	catch (Exception e)
		{System.err.println ("Problem init Siena:" + e.getMessage ());}
    }

    // Subscribes to events of type "VEM"
    // begins recieving the notifications
    public void run ()
    {
		Filter f = new Filter ();
		f.addConstraint ("component", Op.EQ, "VEM");
	  	VemNotif sn = new VemNotif (siena);

		try
		{
		    siena.subscribe (f, sn);

		    try
		    {
				Thread.sleep (1000000000);
		    }
		    catch (java.lang.InterruptedException ex)
		    	{System.out.println ("interrupted");}

		    System.out.println ("unsubscribing");
		    siena.unsubscribe (f, sn);
		}
		catch (SienaException ex)
			{System.err.println ("Siena error:" + ex.toString ());}

		System.out.println ("shutting down.");
		siena.shutdown ();
		System.exit (0);
    }
}

