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

#include <stdio.h>

#include <string>
#ifdef USING_WIN32
using namespace std;
#endif

#include <siena/Siena.h>
#include <siena/SienaId.h>
#include <siena/ThinClient.h>
#include <siena/SENP.h>

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

ThinClient::ThinClient(const string & uri): master_uri(uri) 
{
    master = Sender::create_sender(uri);
    my_id = create_siena_id();
}

void ThinClient::publish(const Notification & n) 
{
    if (master != NULL) 
	{
		//
		// master != NULL should actually be an assertion
		// ...work in progress...
		//
		SENPPacket req;
		req.set_param(&n);
		req.method = SENP::PUB;
		req.id = my_id;
		req.to = master_uri;
		string packet;
		SENP::encode_packet(packet, req);
		master->send(packet.data(), packet.size());
    }
}

void ThinClient::set_receiver(Receiver * r) 
{
    receiver = r;
}

Receiver * ThinClient::get_receiver() 
{
    return receiver;
}

void ThinClient::subscribe(const Filter & f) 
{
    if (master != NULL) 
	{
		//
		// master != NULL should actually be an assertion
		// ...work in progress...
		//
		if (receiver == NULL) 
		{
			receiver = new TCPReceiver(1969);
		}

		SENPPacket req;
		req.set_param(&f);
		req.method = SENP::SUB;
		req.id = my_id;
		req.to = master_uri;
		req.handler = receiver->uri();
		string packet;
		SENP::encode_packet(packet, req);
		master->send(packet.data(), packet.size());
    }
}

void ThinClient::unsubscribe(const Filter & f) 
{
    if (master != NULL) 
	{
		//
		// master != NULL should actually be an assertion
		// ...work in progress...
		//
		SENPPacket req;
		req.set_param(&f);
		req.method = SENP::UNS;
		req.id = my_id;
		req.to = master_uri;
		string packet;
		SENP::encode_packet(packet, req);
		master->send(packet.data(), packet.size());
    }
}

void ThinClient::unsubscribe() 
{
    if (master != NULL) 
	{
		//
		// master != NULL should actually be an assertion
		// ...work in progress...
		//
		SENPPacket req;
		req.method = SENP::BYE;
		req.id = my_id;
		req.to = master_uri;
		string packet;
		SENP::encode_packet(packet, req);
		master->send(packet.data(), packet.size());
    }
}

Notification * ThinClient::get_notification() 
{
    if (receiver == NULL) 
		return NULL;
    
	char pktbuf[SENP_MAXPACKETLEN];
    ssize_t pktsize;
    
	for(;;) 
	{
		pktsize = receiver->receive(pktbuf, SENP_MAXPACKETLEN);
		if (pktsize < 0) 
			return NULL;
		SENPPacket packet; 
		SENP::decode_packet(packet, string(pktbuf, pktsize));
		if (packet.method == SENP::PUB && packet.to == my_id && packet.get_param_type() == SENPPacket::P_Notification && packet.get_event() != NULL) 
		{
			Notification * res = new Notification(*packet.get_event());
			packet.destroy_param();
			return res;
		}
		packet.destroy_param();
    }
}

#if defined(NO_INLINE)
#define inline
#include <siena/ThinClient.icc>
#endif

#ifdef HAVE_CXX_NAMESPACE
} // namespace Siena
#endif

