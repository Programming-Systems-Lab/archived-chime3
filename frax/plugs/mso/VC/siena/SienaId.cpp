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

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif


#ifdef USING_WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <sys/param.h>  // MAXHOSTNAMELEN
#endif

#include <stdio.h>
#include <string>
#ifdef USING_WIN32
using namespace std;
#endif

#include <siena/SienaId.h>

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

static unsigned int counter = 0;
static char hname[MAXHOSTNAMELEN];
static bool initialized = false;

SienaId create_siena_id() 
{
    char buf[256];

    if (!initialized) 
	{
		gethostname(hname, MAXHOSTNAMELEN);
		initialized = true;
    }

    sprintf(buf, "%ul.%ul.%s", counter++, time(NULL), hname);
    return string(buf);
}

#ifdef HAVE_CXX_NAMESPACE
} // namespace Siena
#endif

