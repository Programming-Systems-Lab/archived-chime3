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
#ifndef _SENP_h
#define _SENP_h

#include <sienaconf.h>

#include <sys/types.h> 
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#else
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#endif

#include <string>
#ifdef USING_WIN32
using namespace std;
#endif

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

#define SENP_VERSION	1

#include <siena/Siena.h>

typedef string SENPIdT;
typedef string SENPHandlerT;

class SENPPacket {
 public:
    uint8_t		version;
    uint8_t		method;
    uint8_t		ttl;
    SENPIdT		to;
    SENPIdT		id;
    SENPHandlerT	handler;
    
    SENPPacket();
    SENPPacket(const SENPPacket &p);

    void destroy_param();

    enum ParamType {
	P_Null			= 0, 
	P_Notification		= 1, 
	P_Filter		= 2,
	P_Pattern		= 3,
	P_NotificationSequence	= 4
    };

    uint8_t			get_param_type()	const;

    const Notification *	get_event()		const;
    const Filter *		get_filter()		const;
    const Pattern *		get_pattern()		const;
    const NotificationSequence*	get_eventsequence()	const;

    void			set_param();
    void			set_param(const Notification *);
    void			set_param(const Filter *);
    void			set_param(const Pattern *);
    void			set_param(const NotificationSequence *);

 private:
    uint8_t			param_type;
    bool			new_param;
    union {
	const void *		param;
	const Notification *	event;
	const Filter *		filter;
	const Pattern	*	pattern;
	const NotificationSequence *	events;
    };
};

//
// the SENP class embodies most of the messages of the SENP protocol
// this is a *very minimal* SENP right now
// ...work in progress... 
//
class SENP {
 public:
    //
    // VC++ doesn't allow (or understatd?) initializers for static
    // const members, therefore I'm going to use some CPP macros
    // instead of static const values.  For example, instead of
    // defining:
    //
    //  static const unsigned int MaxPacketLen = 65536;
    //
    // I will use:
    //
    // #define SENP_MAXPACKETLEN 65536
    //

#define SENP_PROTOCOLVERSION	1
    //
    // header names
    //
    static const string		Version;
    static const string		To;
    static const string		Method;
    static const string		Id;
    static const string		Handler;
    static const string		Ttl;
    static const string		Date;

#define SENP_DEFAULTTTL		30
#define SENP_MAXPACKETLEN	65536

    typedef unsigned char	MethodType;

    enum {
	NOP			= 0,
	PUB			= 1,
	SUB			= 2,
	UNS			= 3,
	ADV			= 4,
	UNA			= 5,
	HLO			= 6,
	BYE			= 7,
	SUS			= 8,
	RES			= 9,
	MAP			= 10,
	WHO			= 11,
	INF			= 12
    };
    //
    // methods and operator names
    //
    static const string 	Methods[];

#define SENP_METHODSCOUNT	13

    static const string 	Operators[];

#define SENP_OPERATORSCOUNT	11    

    //
    // default port numbers
    //
#define SENP_CLIENT_PORT	1936;
#define SENP_SERVER_PORT	1969
#define SENP_DEFAULT_PORT	1969;

    static const string		KwdSeparator;
    static const string		KwdSenp;
    static const string		KwdNotification;
    static const string		KwdNotificationSequence;
    static const string		KwdFilter;
    static const string		KwdPattern;
    static const string		KwdLParen;
    static const string		KwdRParen;
    static const string		KwdEquals;
    static const string		KwdTrue;
    static const string		KwdFalse;
    static const string		KwdNull;

    static string &		encode_packet(string &, const SENPPacket &);
    static SENPPacket &		decode_packet(SENPPacket &, const string &);
};

class SENPInvalidFormat: virtual public exception {
    const string & _what;
public:
    SENPInvalidFormat(const string & s): _what(s) {};

    virtual const char * what() const { return _what.c_str(); };
};

#ifndef NO_INLINE
#include <siena/SENP.icc>
#endif

#ifdef HAVE_CXX_NAMESPACE
}; // namespace Siena
#endif

#endif
