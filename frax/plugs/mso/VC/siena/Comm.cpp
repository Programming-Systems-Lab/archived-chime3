// -*- C++ -*-
//
//  This file is part of Siena, a wide-area event notification system.
//  See http://www.cs.colorado.edu/serl/siena/
//
//  Author: Antonio Carzaniga <carzanig@cs.colorado.edu>
//  See the file AUTHORS for full details. 
//
//  Copyright (C) 1998-2001  University of Colorado
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

#include <stdio.h>	// sprintf()

#include <string>
#ifdef USING_WIN32
using namespace std;
#endif

#ifdef USING_WIN32
#include <winsock.h>
#include <stdio.h>
#else
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/param.h>
#endif

#include <siena/Comm.h>

#ifdef USING_WIN32
int initWinSockBullShit() 
{
    WORD wVersionRequested = MAKEWORD(1,1);
    WSADATA wsaData;
    int nRet = WSAStartup(wVersionRequested, &wsaData);
    
	if (wsaData.wVersion != wVersionRequested) 
	{	
		fprintf(stderr,"Wrong Winsock version\n");
		throw (wsaData.wVersion);
    }
    return nRet;
}
static int uselessVariable = initWinSockBullShit();
#endif

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

#ifdef NO_INLINE
#define inline
#include <siena/Comm.icc>
#endif

const char * BadSender::what() const 
{ 
    return "invalid Sender";
}

const char * BadSenderSchema::what() const 
{ 
    return "invalid Sender: unknown schema";
}

const char * UnknownHost::what() const 
{ 
    return "invalid Sender: unknown host";
}

const char * CommException::what() const 
{ 
    return "communication execption";
}

const char * CommError::what() const 
{ 
    return "communication error";
}

const char * CommInterrupted::what() const 
{ 
    return "interruption during communication";
}

class SocketError: public CommError 
{
public:
    enum Type 
	{
		Unknown			= 0,
		Create			= 1,
		Bind			= 2,
		Listen			= 3,
		GetHostName		= 4,
		Connect			= 5,
		Accept			= 6,
		NotConnected		= 7,
		GetSockName		= 8,
		GetPeerName		= 9,
		GetAddressReuse		= 10,
		SetAddressReuse		= 11,
		SendTo			= 12,
		ReceiveFrom		= 13,
		Write			= 14,
		Read			= 15,
	};
    
	Type			type;
    int				errno_value;
	
	SocketError( Type t=Unknown, int e = 0 );

    virtual const char *	what()			const;
};

inline SocketError::SocketError(Type t, int e): type(t), errno_value(e) {};

const char * SocketError::what() const 
{
    static const char * _descr[] = 
	{ 
		"unknown socket exception", 
		"couldn't create socket",
		"couldn't bind socket",
		"couldn't listen to socket",
		"couldn't get hostname",
		"couldn't connect",
		"couldn't accept connections",
		"socket not connected",
		"couldn't get local address",
		"couldn't get peer address",
		"couldn't get address reuse option",
		"couldn't set address reuse option",
		"couldn't send message",
		"couldn't receive message",
		"couldn't write data to socket",
		"couldn't read data from socket"
	};

    return _descr[type];
}

class SocketInterrupted: public CommInterrupted 
{
public:
    enum Type 
	{
		Unknown			= 0,
		Create			= 1,
		Bind			= 2,
		Listen			= 3,
		GetHostName		= 4,
		Connect			= 5,
		Accept			= 6,
		SendTo			= 7,
		ReceiveFrom		= 8,
		Write			= 9,
		Read			= 10,
	};

    Type			type;
	SocketInterrupted(Type t=Unknown);

    virtual const char *	what()			const;
};

inline SocketInterrupted::SocketInterrupted(Type t): type(t) {};

const char * SocketInterrupted::what() const 
{ 
    static const char * _descr[] = 
	{ 
		"socket interruption", 
		"interrupted while creating socket",
		"interrupted while binding socket",
		"interrupted while listening to socket",
		"interrupted while geting hostname",
		"interrupted while connecting",
		"interrupted while accepting connections",
		"interrupted while sending message",
		"interrupted while receiveing message",
		"interrupted while writing data to socket",
		"interrupted while reading data from socket"
    };

    return _descr[type];
}

static const char * TCPSenderSchema = "senp:";
static const char * UDPSenderSchema = "udp+senp:";
static const char * HostPortURIFormat = "%s//%s:%d";

#ifndef HAVE_INET_NTOP
#include <stdio.h>

static const char *inet_ntop(int af, const void *src, char *dst, size_t cnt) 
{
    switch(af) 
	{
		case AF_INET: 
			{
				struct in_addr * addr = (struct in_addr *)src;
				#ifdef USING_WIN32
					_snprintf(dst, cnt, "%d.%d.%d.%d", 
						  (addr->s_addr & 0xff000000) >> 24,
						  (addr->s_addr & 0x00ff0000) >> 16,
						  (addr->s_addr & 0x0000ff00) >> 8,
						  (addr->s_addr & 0x000000ff));
				#else
					snprintf(dst, cnt, "%d.%d.%d.%d", 
						 (addr->s_addr & 0xff000000) >> 24,
						 (addr->s_addr & 0x00ff0000) >> 16,
						 (addr->s_addr & 0x0000ff00) >> 8,
						 (addr->s_addr & 0x000000ff));
				#endif
			}
		//
		// ..to be continued...
		//
			default: break;
    }
    return dst;
}
#endif

string::const_iterator match_prefix(const string &x, const string &y)
{
    // true iff y is a prefix of x, e.g.:
    // x = "testolina"
    // y = "test"
    //
    // true ==> return valid iterator
    // false ==> return x.end()
    //
    string::const_iterator xi = x.begin();
    string::const_iterator yi = y.begin();
    while(yi != y.end() && xi != x.end()) {
	if (*xi != *yi) return x.end();
	++xi;
	++yi;
    }
    return xi;
}

#define DEFAULT_SENP_PORT 1969

void parse_host_port(string & hname,
		     in_port_t & port,
		     string::const_iterator curs, 
		     string::const_iterator end)
{
    //
    // parses and construct a SENP URI:
    // //host[:port][/path...]
    //
    port = DEFAULT_SENP_PORT;
    
    if (curs == end || *curs++ != '/') 
		throw BadSender();
    
	if (curs == end || *curs++ != '/') 
		throw BadSender();
    //
    // the hostname may contain letters, digits, 
    // dots (domain separators), and hyphens (see RFC1034)
    //
    while(curs != end && (isalnum(*curs) || *curs == '.' || *curs == '-'))
		hname += *curs++;
  
    if (curs != end && *curs == ':') 
	{
		++curs;
		port = 0;
		while(curs != end && isdigit(*curs))
			port = port*10 + *curs++ - '0';
    }
}

Sender * Sender::create_sender(const string & uri) 
{
    string host;
    in_port_t port;
    string::const_iterator i;
    
	if ((i = match_prefix(uri, TCPSenderSchema)) != uri.end()) 
	{
		parse_host_port(host, port, i, uri.end());
		return new TCPSender(host,port);
    } 
	else if ((i = match_prefix(uri, UDPSenderSchema)) != uri.end()) 
	{
		parse_host_port(host, port, i, uri.end());
		return new UDPSender(host,port);
    } 
	else 
	{
		throw BadSenderSchema();
    }
}

in_addr_t resolve_hostname(sockaddr_in * a, const char * host_name) 
{
    if ((a->sin_addr.s_addr = inet_addr(host_name)) == INADDR_NONE) 
	{
		#ifdef USING_WIN32
			LPHOSTENT he = gethostbyname(host_name);
			if (he == NULL) return INADDR_NONE;
			a->sin_addr = *((LPIN_ADDR)*he->h_addr_list);
		#else
			hostent* hp = gethostbyname(host_name);
			if (hp == 0) return INADDR_NONE;
			memcpy(&(a->sin_addr), hp->h_addr, hp->h_length);
		#endif
    }
    return a->sin_addr.s_addr;
}

in_addr_t resolve_hostname(const char * hostname) 
{
    sockaddr_in a;
    resolve_hostname(&a, hostname);
    return a.sin_addr.s_addr;
}

void do_write(int fd, const char * buf, size_t s) 
{
    ssize_t res;
    while(s > 0) 
	{
		#ifdef USING_WIN32
			res = send(fd, buf, s, 0);
		#else
			res = write(fd, buf, s);
		#endif
			if (res < 0) 
			{
				switch(errno) 
				{
					case EAGAIN: break;
					case EINTR: throw SocketInterrupted(SocketInterrupted::Write);
					default: throw SocketError(SocketError::Write, errno);
				}
			} 
			else 
			{
				buf += res;
				s -= res;
			}
    }
}

size_t do_read(int fd, char * buf, size_t s) 
{
    ssize_t res;
    size_t total = 0;
    while(s > 0) 
	{
		#ifdef USING_WIN32
			res = recv(fd, buf, s, 0);
		#else
			res = read(fd, buf, s);
		#endif
		if (res > 0) 
		{
			buf += res;
			total += res;
			s -= res;
		} 
		else if (res == 0) 
		{
			return total;
		} else 
		{
			switch(errno) 
			{
				case EAGAIN: break;
				case EINTR: throw SocketInterrupted(SocketInterrupted::Read);
				default: throw SocketError(SocketError::Read, errno);
			}
		}
    }
    return total;
}

TCPSender::TCPSender(const string &host, in_port_t p): port(p) 
{
    ip = resolve_hostname(host.c_str());
    if (ip == INADDR_NONE) 
	throw UnknownHost();
}

TCPSender::TCPSender(in_addr_t i, in_port_t p): ip(i), port(p) {};

#ifdef USING_WIN32

void TCPSender::send(const char * buf, size_t s) throw (CommException) 
{
    SOCKET sock_fd;
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
    
	if (sock_fd == INVALID_SOCKET) 
		throw SocketError(SocketError::Create, errno);

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    
	if (connect(sock_fd, (LPSOCKADDR)(& addr), sizeof(struct sockaddr)) == SOCKET_ERROR) 
	{
	    closesocket(sock_fd);
	    throw SocketError(SocketError::Connect, 0);
	}

    if (::send(sock_fd, buf, s, 0) == SOCKET_ERROR) 
	{
		closesocket(sock_fd);
		throw SocketError(SocketError::SendTo, 0);
    }

    closesocket(sock_fd);
}

#else

void TCPSender::send(const char * buf, size_t s) throw (CommException) 
{
    int sock_fd;
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
	if (sock_fd < 0)
		throw SocketError(SocketError::Create, errno);

    sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    
	if(connect(sock_fd, (struct sockaddr *)(& addr), sizeof(addr)) < 0) 
		switch (errno) 
		{
			case EINTR: throw SocketInterrupted(SocketInterrupted::Connect);
			default: throw SocketError(SocketError::Connect, errno);
	}

    do_write(sock_fd, buf, s);

    if (close(sock_fd) < 0) 
	{
		//
		// ...work in progress...
		//
    }
}
#endif

UDPSender::UDPSender(const string &host, in_port_t p): port(p), 
#ifdef USING_WIN32
    sock_fd(INVALID_SOCKET) 
#else
    sock_fd(-1) 
#endif
{
    ip = resolve_hostname(host.c_str());
    if (ip == INADDR_NONE) 
	throw UnknownHost();
}

UDPSender::UDPSender(in_addr_t i, in_port_t p): ip(i), port(p), 
#ifdef USING_WIN32
    sock_fd(INVALID_SOCKET) 
#else
    sock_fd(-1) 
#endif
{};

#ifdef USING_WIN32
void UDPSender::shutdown() 
{
    if (sock_fd != INVALID_SOCKET) 
	{
		closesocket(sock_fd);
		sock_fd = INVALID_SOCKET;
		}
}
#else
void UDPSender::shutdown() {
    if (sock_fd >= 0) 
	{
		if (close(sock_fd) < 0) 
		{
			//
			// ...work in progress...
			//
		}
		sock_fd = -1;
    }
}
#endif
#ifdef USING_WIN32
void UDPSender::send(const char * buf, size_t s) throw (CommException) 
{
    if (sock_fd == INVALID_SOCKET) 
	{
		sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock_fd == INVALID_SOCKET)
			throw SocketError(SocketError::Create, errno);
    }

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if(sendto(sock_fd, buf, s, 0, (LPSOCKADDR)&addr, sizeof(struct sockaddr)) == SOCKET_ERROR) 
	throw SocketError(SocketError::SendTo, 0);
}
#else

void UDPSender::send(const char * buf, size_t s) throw (CommException) 
{
    if (sock_fd < 0) 
	{
		sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock_fd < 0)
			throw SocketError(SocketError::Create, errno);

		sockaddr_in addr;
		addr.sin_addr.s_addr = ip;
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;
		if(connect(sock_fd, (struct sockaddr *)(& addr), sizeof(addr)) < 0) 
			throw SocketError(SocketError::Connect, errno);
    }
    do_write(sock_fd, buf, s);
}
#endif

//
// this function `guesses' an IP address for this host.  It does
// that by doing a DNS lookup on the host name (returned by
// gethostname()).  WARNING: this is *NOT* a reliable way of
// obtaining a current valid address of this host.  In fact, I
// believe that this is `configuration' information that should be
// provided by the user.
//
string this_hostname() 
{
    char hostname[MAXHOSTNAMELEN];
    gethostname(hostname, MAXHOSTNAMELEN - 1);
    sockaddr_in addr;
    resolve_hostname(&addr, hostname);
    //
    // I reuse the hostname buffer.  
    //
    inet_ntop(PF_INET, &addr.sin_addr, hostname, MAXHOSTNAMELEN);
    return hostname;
}

#ifdef USING_WIN32
int bind_port(SOCKET sock_fd, in_port_t port, bool reuse) 
{
    int value = reuse ? 1 : 0;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, 
		   (char *)&value, sizeof(value)) == SOCKET_ERROR) 
	return SOCKET_ERROR;

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(sock_fd, (LPSOCKADDR)&addr, sizeof(struct sockaddr));
}
#else
int bind_port(int sock_fd, in_port_t port, bool reuse) 
{
    int value = reuse ? 1 : 0;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, 
		   (char *)&value, sizeof(value)) < 0)
	return -1;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    return bind(sock_fd, (sockaddr *)&addr, sizeof(addr));
}
#endif

#ifdef USING_WIN32
TCPReceiver::TCPReceiver(in_port_t p, const char * hostname, bool reuse, int maxconn): port(p), sock_fd(-1) 
{
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
	if (sock_fd == INVALID_SOCKET)
		throw SocketError(SocketError::Create, 0);

    if (bind_port(sock_fd, p, reuse) == SOCKET_ERROR) 
	{
		closesocket(sock_fd);
		throw SocketError(SocketError::Bind, 0);
    };

    if (listen(sock_fd, (maxconn < 0) ? SOMAXCONN : maxconn) == SOCKET_ERROR) 
	{
		closesocket(sock_fd);
		throw SocketError(SocketError::Listen, errno);
    }
    
    //
    // QUICK HACK!  ...work in progress...
    //
    char buf[256];
    sprintf(buf, HostPortURIFormat, TCPSenderSchema,
	    (hostname == NULL) ? this_hostname().c_str() : hostname, port);
    _uri = buf;
}
#else 
TCPReceiver::TCPReceiver(in_port_t p, const char * hostname, 
			 bool reuse, int maxconn): port(p), sock_fd(-1) 
{
    sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
	if (sock_fd < 0)
		throw SocketError(SocketError::Create, errno);

    if (bind_port(sock_fd, p, reuse) < 0) 
	{
		close(sock_fd);
		throw SocketError(SocketError::Bind, errno);
    };

    if (listen(sock_fd, (maxconn < 0) ? SOMAXCONN : maxconn) < 0) 
	{
		close(sock_fd);
		throw SocketError(SocketError::Listen, errno);
    }
    
    //
    // QUICK HACK!  ...work in progress...
    //
    char buf[256];
    sprintf(buf, HostPortURIFormat, TCPSenderSchema,
	    (hostname == NULL) ? this_hostname().c_str() : hostname, port);
    _uri = buf;
}
#endif

#ifdef USING_WIN32
UDPReceiver::UDPReceiver(in_port_t p, const char * hostname, bool reuse): port(p), sock_fd(INVALID_SOCKET) 
{
    sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
	if (sock_fd == INVALID_SOCKET)
		throw SocketError(SocketError::Create, errno);

    if (bind_port(sock_fd, p, reuse) < 0) 
	{
		closesocket(sock_fd);
		throw SocketError(SocketError::Bind, errno);
    };

    //
    // QUICK HACK!  ...work in progress...
    //
    char buf[256];
    sprintf(buf, HostPortURIFormat, UDPSenderSchema,
	    (hostname == NULL) ? this_hostname().c_str() : hostname, port);
    _uri = buf;
}
#else
UDPReceiver::UDPReceiver(in_port_t p, const char * hostname, bool reuse): port(p), sock_fd(-1) 
{
    sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
	if (sock_fd < 0)
		throw SocketError(SocketError::Create, errno);

    if (bind_port(sock_fd, p, reuse) < 0) 
	{
		close(sock_fd);
		throw SocketError(SocketError::Bind, errno);
    };

    //
    // QUICK HACK!  ...work in progress...
    //
    char buf[256];
    sprintf(buf, HostPortURIFormat, UDPSenderSchema,
	    (hostname == NULL) ? this_hostname().c_str() : hostname, port);
    _uri = buf;
}
#endif

size_t UDPReceiver::receive(char * buf, size_t s) throw (CommException) 
{
    ssize_t res;

    while ((res = recvfrom(sock_fd, buf, s, 0, NULL, 0)) < 0) 
	{
		switch (errno) 
		{
			case EAGAIN: break;
			case EINTR: throw SocketInterrupted(SocketInterrupted::ReceiveFrom);
			default: throw SocketError(SocketError::ReceiveFrom, errno);
		}
    }
    return res;
}

string UDPReceiver::uri() 
{
    return _uri;
}

string & UDPReceiver::uri(string & s) 
{
    s = _uri;
    return s;
}

string TCPReceiver::uri() 
{
    return _uri;
}

string & TCPReceiver::uri(string & s) 
{
    s = _uri;
    return s;
}

#ifdef USING_WIN32
size_t TCPReceiver::receive(char * buf, size_t s) throw (CommException) 
{
    SOCKET nfd;
    if ((nfd = accept(sock_fd, NULL, NULL)) == INVALID_SOCKET) 
	throw SocketError(SocketError::Accept, errno);
    
    size_t res = do_read(nfd, buf, s);
    closesocket(nfd);
    return res;
}
#else
size_t TCPReceiver::receive(char * buf, size_t s) throw (CommException) 
{
    int nfd;
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    
	while((nfd = accept(sock_fd, (sockaddr *)&addr, &len)) < 0) 
	{
		switch (errno) 
		{
			case EAGAIN: break;
			case EINTR: throw SocketInterrupted(SocketInterrupted::Accept);
			default: throw SocketError(SocketError::Accept, errno);
		}
    }
    
    size_t res = do_read(nfd, buf, s);
    if (close(nfd) < 0) 
	{
		//
		// ...work in progress...
		//
    }
    return res;
}
#endif

#ifdef USING_WIN32
void TCPReceiver::shutdown() 
{
    if (sock_fd != INVALID_SOCKET) 
	{
		closesocket(sock_fd);
		sock_fd = INVALID_SOCKET;
    }
}
#else
void TCPReceiver::shutdown() 
{
    if (sock_fd >= 0) 
	{
		if (close(sock_fd) < 0) 
		{
			//
			// ...work in progress...
			//
		}
    } 
    sock_fd = -1;
}
#endif
#ifdef USING_WIN32
void UDPReceiver::shutdown() 
{
    if (sock_fd != INVALID_SOCKET) 
	{
		closesocket(sock_fd);
		sock_fd = INVALID_SOCKET;
    }
}
#else
void UDPReceiver::shutdown() 
{
    if (sock_fd >= 0) 
	{
		if (close(sock_fd) < 0) 
		{
			//
			// ...work in progress...
			//
		}
    } 
    sock_fd = -1;
}
#endif

#ifdef HAVE_CXX_NAMESPACE
}; // namespace Siena
#endif
