 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

// -*- Java -*-
//
//  This file is part of Siena, a wide-area event notification system.
//  See http://www.cs.colorado.edu/serl/siena
//
//  Author: Antonio Carzaniga <carzanig@cs.colorado.edu>
//  See the file AUTHORS for full details.
//
//  Copyright (C) 1998-1999 University of Colorado
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
//  USA, or send email to serl@cs.colorado.edu.
//
//
// $Id$
//

//
// this is an example of an interested party, that is, a consumer of
// notifications
//

package psl.chime.sienautils;
import psl.chime.sienautils.gui.*;
import siena.*;


/**
 * This is a callback class which is called when an object
 * of interest is published on Siena
 */
class SimpleNotif implements Notifiable {
    Siena siena;

    public SimpleNotif(Siena s) {
        siena = s;
    }

    /**
     * Is called when a notification is received from the Siena bus
     */
    public void notify(Notification e) {
          System.out.println("local notifiable: " + e.toString() + "\n\n");
//            new SimpleDialog("Subscriber received Siena Event", e.toString());
/*         try {
             siena.unsubscribe(this);
         } catch (SienaException ex) {
             ex.printStackTrace();
         }*/
    }

    /**
     * Same as above but when an array of notifications is returned
     */
    public void notify(Notification [] s) {
	for (int i = 0; i < s.length; i++) {
		notify(s[i]);
	}
    }
}


public class SubscribeObject {

    HierarchicalDispatcher siena;

    /**
     * Check if the siena server is really running where the
     * user has told it is running
     */
    public SubscribeObject(String siena_location) {

	Runtime.getRuntime().addShutdownHook(new Thread() {
      	public void run() {
		System.err.println("SLT shutting down");
		siena.shutdown();
      } });

	try {
	    siena = new HierarchicalDispatcher();
	    if (siena_location != null) {
		siena.setMaster(siena_location);
	    } else {
		System.err.println("can't set empty siena location");
		throw new SienaException();
	    }
	}catch (Exception e) {
	    System.err.println("Problem init Siena:" + e.getMessage());
	}
    }


    /**
     * setup a subscriber which in our case is just a module/component
     * which is interested in what another module/component is publishing
     */
    public void alert(String component) {

	Filter f = new Filter();
	f.addConstraint("component", Op.EQ, component); //alert: that mess for me
	SimpleNotif sn = new SimpleNotif(siena);


	System.out.println("subscribing for " + f.toString());
	try {
	    siena.subscribe(f, sn);
	    try {
		Thread.sleep(1800000);       // sleeps for thirty minutes
	    } catch (java.lang.InterruptedException ex) {
		System.out.println("interrupted");
	    }
	    System.out.println("unsubscribing");
	    siena.unsubscribe(f, sn);
	} catch (SienaException ex) {
	    System.err.println("Siena error:" + ex.toString());
	}
	System.out.println("shutting down.");
	siena.shutdown();
	System.exit(0);
    }

    /**
     * Basic testing routing for this class and an example
     * of how to use it
     */
    public static void main(String[] args) {
        if(args.length != 2) {
            System.err.println("Usage: SubscribeObject <server-uri> <component>");
            System.exit(1);
        }

	SubscribeObject so = new SubscribeObject(args[0]);
	so.alert(args[1]);
    }
}

