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
#ifndef _ThinClient_h
#define _ThinClient_h

#include <sienaconf.h>

#include <sys/types.h> 
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include <string>

#include <siena/SienaId.h>
#include <siena/Comm.h>

#ifdef HAVE_CXX_NAMESPACE
/** Siena API package. **/
namespace Siena {
#endif

/** implementation of a "pull" interface to Siena
 **/
class ThinClient: public SienaPull {
public:
    /** construct a ThinClient connected to a given server 
    *
    *   the given URI must be a valid uri for an external (active)
    *   Siena server.
    **/
			ThinClient(const string & uri); 
			~ThinClient();

    /** attaches a receiver to this interface 
     *
     *  a ThinClient must have a valid receiver in order to receive
     *  notifications.  Notice that it is necessary to set a receiver
     *  only if the ThinClient is used to subscribe for notifications.
     *  If you use the ThinClient only to publish, then you don't need
     *  to set a receiver.
     *
     *  <p><b>Notice:</b> the ThinClient does not manage a receiver.
     *  In particular, it does not destroy receivers associated with
     *  it.  It is the responsibility of the application to create and
     *  destroy receivers.
     **/
    void		set_receiver(Receiver *);

    /** returns the receiver associated with this ThinClient
     **/
    Receiver *		get_receiver();

    virtual void	publish(const Notification &e);

    virtual void	subscribe(const Filter &f);

    virtual void	unsubscribe(const Filter &f);
    virtual void	unsubscribe();

    virtual Notification *		get_notification();
    virtual NotificationSequence *	get_notification_sequence();

    virtual void	suspend();
    virtual void	resume();

    virtual void	shutdown();

private:
    string master_uri;
    Sender * master;
    SienaId my_id;
    Receiver * receiver;
};

#ifndef NO_INLINE
#include <siena/ThinClient.icc>
#endif

#ifdef HAVE_CXX_NAMESPACE
};
#endif

#endif
