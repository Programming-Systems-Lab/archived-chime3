// -*- C++ -*-
//
//  This file is part of Siena, a wide-area event notification system.
//  See http://www.cs.colorado.edu/serl/dot/siena.html
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
#ifndef _SENPTokenizer_h
#define _SENPTokenizer_h
//
// this is a separate file basically only for testing purposes,
// meaning that some of my test programs will include this file
// (separately from SENP.h) in order to test my tokenizer.
//
#include <sienaconf.h>

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#else
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#endif

#include <string>

#include <siena/Siena.h>

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

class SENPTokenizer {
public:
    enum TokenType {
	T_EOF		= 1,
	T_UNKNOWN	= 2,
	T_ID		= 3,
	T_STR		= 4,
	T_INT		= 5,
	T_DOUBLE	= 6,
	T_BOOL		= 7,
	T_OP		= 8,
	T_LPAREN	= 9,
	T_RPAREN	= 10,
	T_DATE		= 11
    };

    SXOperator		oval;
    string		sval;
    int32_t		ival;
    bool		bval;
    double		dval;
#ifdef WITH_DATE
    Date		tval;
#endif
			SENPTokenizer(const string & s);

    TokenType		nextToken();   

private:
    const string &		buf;
    string::const_iterator	pos;

    unsigned char read_octal();
    TokenType decode_date_rfc1123();
    TokenType read_string();
    TokenType read_int();
    TokenType read_number();
    double read_decimal();
};


#ifdef HAVE_CXX_NAMESPACE
}; // namespace Siena
#endif

#endif
