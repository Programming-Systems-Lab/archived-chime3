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
#ifndef _Comm_h
#define _Comm_h

#include <sienaconf.h>

#ifdef USING_WIN32
#include <winsock.h>
#else
#include<sys/socket.h>
#include<netinet/in.h>
#endif

#include <exception>
#include <string>

#ifdef USING_WIN32
using namespace std;
#endif

#include <stdio.h>

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

/** communication exception 
 **/
class CommException: public exception {
public:
    virtual const char *what() const;
};

/** communication error 
 **/
class CommError: public CommException {
public:
    virtual const char *what() const;
};

/** communication interrupted 
 **/
class CommInterrupted: public CommException {
public:
    virtual const char *what() const;
};

/** invalid URI
 **/
class BadSender: public exception {
public:
    virtual const char *what() const;
};

/** URI schema not supported
 **/
class BadSenderSchema: public BadSender {
public:
    virtual const char *what() const;
};

/** URI schema not supported
 **/
class UnknownHost: public BadSender {
public:
    virtual const char *what() const;
};

/** abstract sender mechanism 
 *
 *  a <code>Sender</code> represents a generic level-2 output
 *  communication mechanism.  In practice, a <code>Sender</code>
 *  allows one to send packets.  This is a virtual class,
 *  specializations of this class will implement specific
 *  communication mechanisms.
 **/
class Sender { 
 public:
    /** sender factory
     *
     *  constructs a Sender based on the <em>URI</em> of the receiver.
     **/
    static Sender *	create_sender(const string &);

    virtual		~Sender();

    /** sends a message
     *
     *	@throws CommException
     **/
    virtual void	send(const char *, size_t) throw (CommException) = 0;

    /** closes this sender 
     **/
    virtual void	shutdown();
};

/** abstract receiver mechanism 
 *
 *  a <code>Receiver</code> represents a generic level-2 input
 *  communication mechanism.  In practice, a <code>Receiver</code>
 *  allows one to receive packets.  This is a virtual class,
 *  specializations of this class will implement specific
 *  communication mechanisms.
 **/
class Receiver { 
 public:
    /** destructor 
     **/
    virtual		~Receiver();

    /** URI for this receiver 
     *
     *  a URI is an external representation of the point of contact of
     *  this receiver.  This URI can be used to create senders to
     *  contact this receiver.
     **/
    virtual string	uri() = 0;

    /** exernally visible URI for this receiver 
     *
     *  a URI is an external representation of the point of contact of
     *  this receiver.  This URI can be used to create senders to
     *  contact this receiver.
     **/
    virtual string &	uri(string &) = 0;

    /** receives a message 
     *
     *	@throws CommException
     **/
    virtual size_t	receive(char *, size_t)	throw (CommException) = 0;

    /** closes this sender 
     **/
    virtual void	shutdown();
};

/** TCP-based sender
 *
 *  this sender connects to a TCP port, writes a packet on the new
 *  connection, and then closes the connection.
 **/
class TCPSender: public Sender {
public:
			TCPSender(const string &host, in_port_t port);
			TCPSender(in_addr_t ip, in_port_t port);

    virtual void	send(const char *, size_t) throw (CommException);

private:
    in_port_t port;
    in_addr_t ip;
};

/** TCP-based sender
 *
 *  this sender uses TCP connections to send packets. It opens a new
 *  connection to a given TCP port, writes a packet on the new
 *  connection, and then closes the connection.
 **/
class UDPSender: public Sender {
public:
			UDPSender(const string &host, in_port_t port);
			UDPSender(in_addr_t ip, in_port_t port);

    virtual void	shutdown();

    virtual void	send(const char *, size_t) throw (CommException);
private:
    in_port_t port;
    in_addr_t ip;
#ifdef USING_WIN32
    SOCKET sock_fd;
#else
    int sock_fd;
#endif
};

/** TCP-based receiver
 *
 *  this receiver uses a TCP port to receive packets.  It creates a
 *  local TCP port when initialize.  Then, in order to receive a
 *  packet, it accepts a new connection from its TCP port, reads a
 *  packet from the connection, and closes the connection.
 **/
class TCPReceiver: public Receiver {
public:
    /** constructs a TCPReceiver
     *
     * @param port  port number
     *
     * @param hostname  externally visible name for this host.
     *        Although it is possible for this library to figure out a
     *        hostname, the default value is not 100% reliable,
     *        especially in cases in which the host uses
     *        dynamically-assigned IP addresses, or in the presence of
     *        several network interfaces.  
     *
     * @param reuse determines whether the given port number should be
     *        immediately re-used, if recently bound to another
     *        application.
     *
     * @param maxconn sets the length of the OS-level queue for
     *        incoming connections
     **/
			TCPReceiver(in_port_t port, 
				    const char * hostname = NULL, 
				    bool reuse = true, 
				    int maxconn = -1 /* default */);

    virtual size_t	receive(char *, size_t) throw (CommException);
    virtual void	shutdown();

    virtual string	uri();
    virtual string &	uri(string &);

private:
    in_port_t port;
    string _uri;
#ifdef USING_WIN32
    SOCKET sock_fd;
#else
    int sock_fd;
#endif
};

/** TCP-based receiver
 *
 *  this receiver uses a TCP port to receive packets.  It creates a
 *  local TCP port when initialize.  Then, in order to receive a
 *  packet, it accepts a new connection from its TCP port, reads a
 *  packet from the connection, and closes the connection.
 **/
class UDPReceiver: public Receiver {
public:
    /** constructs a TCPReceiver
     *
     * @param port  port number
     *
     * @param hostname  externally visible name for this host.
     *        Although it is possible for this library to figure out a
     *        hostname, the default value is not 100% reliable,
     *        especially in cases in which the host uses
     *        dynamically-assigned IP addresses, or in the presence of
     *        several network interfaces.  
     *
     * @param reuse determines whether the given port number should be
     *        immediately re-used, if recently bound to another
     *        application.
     **/
			UDPReceiver(in_port_t p, 
				    const char * hostname = NULL, 
				    bool reuse = true);

    virtual size_t	receive(char *, size_t) throw (CommException);
    virtual void	shutdown();

    virtual string	uri();
    virtual string &	uri(string &);

private:
    in_port_t port;
    string _uri;
#ifdef USING_WIN32
    SOCKET sock_fd;
#else
    int sock_fd;
#endif
};

#ifndef NO_INLINE
#include <siena/Comm.icc>
#endif

#ifdef HAVE_CXX_NAMESPACE
}; // namespace Siena
#endif

#endif
