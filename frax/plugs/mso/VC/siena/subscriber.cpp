// -*- C++ -*-
//
//  This file is part of Siena, a wide-area event notification system.
//  See http://www.cs.colorado.edu/serl/siena/
//
//  Author: Antonio Carzaniga <carzanig@cs.colorado.edu>
//  See the file AUTHORS for full details. 
//
//  Copyright (C) 1998-2001 University of Colorado
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
// $Id$
//
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <exception>
						// include Siena headers
#include <siena/Siena.h>
#include <siena/ThinClient.h>

static void print_usage(const char * progname) {
    cerr << "usage: " << progname 
	 << " [-udp] [-port <num>] [-host <host>] <uri-master>" << endl;
}

int main (int argc, char *argv[])
{
    bool udp = false;
    unsigned short port = 1969;
    const char * master = NULL;
    const char * thishost = NULL;
    
    try {
        int i;
	for(i=1; i < argc; ++i) {		// parse cmd-line params
	    if(strcmp(argv[i], "-udp")==0) {
		udp = true;
	    } else if (strcmp(argv[i], "-port")==0) {
		if (++i < argc) {
		    port = atoi(argv[i]);
		} else {
		    print_usage(argv[0]);
		    return 1;
		}
	    } else if (strcmp(argv[i], "-host")==0) {
		if (++i < argc) {
		    thishost = argv[i];
		} else {
		    print_usage(argv[0]);
		    return 1;
		}
	    } else {
		master = argv[i];
	    }
	}
	if (master == NULL) {
	    print_usage(argv[0]);
	    return 1;
	}

	ThinClient siena(master);		// create interface to
	Receiver * r;				// given master server
	if (udp) {
	    r = new UDPReceiver(port, thishost);// create receiver for 
	} else {				// this interface
	    r = new TCPReceiver(port, thishost);
	}
	siena.set_receiver(r);			// set receiver

	Filter f;				// create subscription filter
	f.add_constraint("stock", SX_eq, "XYZ");
	f.add_constraint("price", SX_lt, (long)100);
	siena.subscribe(f);			// subscribe

	Notification * n;

	for (i = 1; i < 20; ++i) {		// read incoming notifications
	    n = siena.get_notification();
	    if (n != NULL)  {
		cout << "stock=" << (*n)["stock"].string_value()
		     << " price=" << (*n)["price"].int_value() << endl;
		delete(n);
	    }
	}
	siena.unsubscribe();			// unsubscribe and shutdown 
	siena.shutdown();			// interface
	delete(r);
    } catch (exception &ex) {
	cout << "error: " << ex.what() << endl;
    }
    return 0;
}
