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
//
// $Id$
//
#include <sienaconf.h>

#ifdef USING_WIN32
#include <stdlib.h>
#define sleep _sleep
#else
#include <unistd.h>
#endif

#include <iostream.h>
#include <exception>
						// include Siena headers
#include <siena/Siena.h>
#include <siena/ThinClient.h>

int main (int argc, char *argv[])
{
    try {					// handles cmd-line params
	if (argc != 2) {
	    cerr << "usage: " << argv[0] << " <uri>" << endl;
	    return 1;
	}

	ThinClient siena(argv[1]);		// creates interface to 
						// given master server
	Notification n;
	n["stock"] = "XYZ";
 
	for (int i = 10; i < 200; i += 10) {
	    n["price"] = i;
	    siena.publish(n);
	    sleep(1);
	}
	siena.shutdown();			// closes Siena interface
    } catch (exception &ex) {
	cout << "error: " << ex.what() << endl;
    }
    return 0;
}
