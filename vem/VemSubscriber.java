package psl.chime.vem;

import java.lang.*;
import java.io.*;
import siena.*;

public class VemSubscriber
{
    public static void main (String [] args)
    {
    	if (args.length != 2)
    		System.err.println ("Incorrect Usage: java VemSubsriber <host> <port>");
    	else
    	{
    		String sienaHost;
    		Vem V = new Vem ();

	    	sienaHost = "senp://" + args [0] + ":" + args [1];

			V.VemSetHost (sienaHost);

			V.start ();
		}
    }
}