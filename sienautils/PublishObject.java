 /*
 * Copyright (c) 2001: The Trustees of Columbia University
 *    in the City of New York.  All Rights Reserved.
 *
 */

// -*- Java -*-
//
//  This file is part of Siena, a wide-area event notification system.
//  See http://www.cs.colorado.edu/serl/siena/
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
// this is an example of an object of interest, that is, a producer of
// notifications
//

package psl.chime.sienautils;
import siena.*;


public class PublishObject {

    HierarchicalDispatcher siena;

    /**
     * Select a location where we can publish an object
     * check the siena server and make sure it is OK
     */
    public PublishObject(String siena_location) {

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
     * publish the component aspect which in my case was
     * FRAX and the data which is being returned by the component
     * this is the XML in my case
     */
    public boolean submit(String component, String data) {
	try {

            Notification e = new Notification();
	    e.putAttribute("component", component);
            e.putAttribute("data", data);

            System.out.println("publishing " + e.toString());
            try {
                siena.publish(e);
            } catch (SienaException ex) {
                System.err.println("Siena error:" + ex.toString());
            }
            System.out.println("shutting down.");
            siena.shutdown();
        } catch (Exception ex) {
            ex.printStackTrace();
            return false;
        }
	return true;
    }

    /**
     * A test routing to check and show how this class works
     */
    public static void main(String[] args) {

	PublishObject po = new PublishObject("senp://localhost:2345");
	po.submit("FRAX", "<Protocol>testing</Protocol>");

    }
}
