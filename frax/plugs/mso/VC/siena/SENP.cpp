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


#include <string>
#ifdef USING_WIN32
using namespace std;
#endif

#include <cctype>
#include <cstdlib>
#include <cmath>

#include <stdio.h>

#include <siena/Siena.h>
#include <siena/SENP.h>

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

#if defined(NO_INLINE)
#define inline
#include <siena/SENP.icc>
#endif

const string	SENP::Version		= "version";
const string	SENP::To		= "to";
const string	SENP::Method		= "method";
const string	SENP::Id		= "id";
const string	SENP::Handler		= "handler";
const string	SENP::Ttl		= "ttl";

//
//  WARNING:  don't mess up the order of method names in this array
//            it must correspond to the definitions of SENP::NOP,
//            SENP::PUB, SENP::SUB, etc.
// 
const string  SENP::Methods[]		= 
{
						"NOP",
						"PUB",
						"SUB",
						"UNS",
						"ADV",
						"UNA",
						"HLO",
						"BYE",
						"SUS",
						"RES",
						"MAP",
						"WHO",
						"INF"
};

const unsigned int MethodsCount			= 14;

//
//  WARNING:  don't mess up the order of operators in this array
//            it must correspond to the definitions of Siena.EQ,
//            Siena.LT, etc.
// 
const string	SENP::Operators[]		= 
{ 
						"?", // unknown
						"=",
						"<",
						">",
						">=",
						"<=",
						">*",
						"*<",
						"any",
						"!=",
						"*"
};

const string SENP::KwdSeparator			= " ";
const string SENP::KwdSenp			= "senp";
const string SENP::KwdNotification		= "event";
const string SENP::KwdNotificationSequence	= "events";
const string SENP::KwdFilter			= "filter";
const string SENP::KwdPattern			= "pattern";
const string SENP::KwdLParen			= "{";
const string SENP::KwdRParen			= "}";
const string SENP::KwdEquals			= "=";
const string SENP::KwdTrue			= "true";
const string SENP::KwdFalse			= "false";
const string SENP::KwdNull			= "null";

static const string PE_literal = "expecting literal value";
static const string PE_operator = "expecting operator";
static const string PE_lparen = "expecting `{'";
static const string PE_anamerparen = "expecting attribute name or `}'";
static const string PE_filter = "expecting keyword `filter'";
static const string PE_equal = "excpecting `='";
static const string PE_event = "expecting keyword `event'";
static const string PE_senp = "expecting keyword `senp'";
static const string PE_pattern = "expecting keyword `pattern'";
static const string PE_filterrparen = "expecting keyword `filter' or `}'";
static const string PE_efps = "expecting keyword `event' or `filter' or `pattern' or `events'";

#include "SENPTokenizer.h"

SENPTokenizer::SENPTokenizer(const string & s): buf(s) 
{
    pos = s.begin();
}

inline bool is_first_ident_char(char c) 
{
    return (c >= 0x41 && c <= 0x5a)	// 'A' -- 'Z'
	|| (c >= 0x61 && c <= 0x7a)	// 'a' -- 'z'
	|| c == 0x5f;			// '_'
}

inline bool is_other_ident_char(char c) 
{
    return (c >= 0x41 && c <= 0x5a)	// 'A' -- 'Z' 
	|| (c >= 0x61 && c <= 0x7a)	// 'a' -- 'z'
	|| (c >= 0x30 && c <= 0x39)	// '0' -- '9'
	|| c == 0x5f || c == 0x24	// '_', '$' 
	|| c == 0x2e || c == 0x2f;	// '.', '/' 
}

inline bool is_octal_char(char c) 
{
    return c >= '0' && c <= '7';
}

inline bool is_decimal_char(char c) 
{
    return c >= '0' && c <= '9';
}

inline unsigned char decimal_value(char c) 
{
    return c - '0';
}

inline char char_value(unsigned int i) 
{
    return i + '0';
}

inline bool not_printable_char(char c) 
{
    return c < 0x20 || c >= 0x7F;
}

unsigned char SENPTokenizer::read_octal() 
{ 
    //
    // reads the following characters in buf interpreting them as an
    // octal number representing a byte.
    //
    // PRECONDITION: is_octal_char(*pos);
    //
    register unsigned char res = 0;
    register int i = 3;
    do 
	{
		res = res * 8 + decimal_value(*pos++);
    } while(--i > 0 && pos != buf.end() && is_octal_char(*pos));
    
	return res;
}

#ifdef WITH_DATE
SENPTokenizer::TokenType SENPTokenizer::decode_date_rfc1123() 
{
    //
    // ...to be continued...
    // ...work in progress...
    //
    return T_UNKNOWN;
}
#endif

SENPTokenizer::TokenType SENPTokenizer::read_string() 
{
    //
    // reads the following characters in buf interpreting them as a
    // string.  Returns either SENPTokenizer::T_UNKNOWN or
    // SENPTokenizer::T_STR.
    //
    // PRECONDITION: *pos == '"'
    //
    sval.erase();
    while(++pos != buf.end())
		switch (*pos) 
		{
		case '"': ++pos; return T_STR;
		case '\\': if (++pos == buf.end()) return T_UNKNOWN;
						switch (*pos) 
						{
							case 'v': sval += '\v';break;
							case 'f': sval += '\f';break;
							case 'r': sval += '\r';break; 
							case 'n': sval += '\n'; break;
							case 't': sval += '\t'; break;
							case 'b': sval += '\b'; break;
							case 'a': sval += '\a'; break;
							default: if (is_octal_char(*pos)) 
									 {
										sval += read_octal();
									 } 
									 else 
									 {
										sval += *pos;
									 }
							}
					break;
		default: sval += *pos;
	}
    return T_UNKNOWN;
}

SENPTokenizer::TokenType SENPTokenizer::read_int() 
{
    bool negative = false;
    //
    // here *pos is either a digit or '-'
    //
    if (*pos == '-') 
	{
		negative = true;
		ival = 0;
		if (++pos == buf.end() || !is_decimal_char(*pos))
			return T_UNKNOWN;
    } 
	else 
	{
		ival = decimal_value(*pos);
		if (++pos == buf.end() || !is_decimal_char(*pos))
			return T_INT;
    }
    do 
	{
		ival = ival * 10 + decimal_value(*pos);
    } while (++pos != buf.end() && is_decimal_char(*pos));

    if (negative) ival = -ival;
		return T_INT;
}

SENPTokenizer::TokenType SENPTokenizer::read_number() 
{
    bool negative = false;
    //
    // here *pos is either a digit or '-'
    //
    if (*pos == '-') 
	{
		negative = true;
		if (++pos == buf.end() || !is_decimal_char(*pos))
			return T_UNKNOWN;
    }
    TokenType type;
    if (read_int() == T_UNKNOWN) 
		return T_UNKNOWN;
    
	type = T_INT;
    dval = ival;
    if (pos != buf.end() && *pos == '.') 
	{
		type = T_DOUBLE;
		if (++pos == buf.end() || !is_decimal_char(*pos)) 
		{
			return T_UNKNOWN;
		} 
		else 
		{
			dval += read_decimal();
		}
    }

    if (pos != buf.end())
		if (*pos == 'e' || *pos == 'E') 
		{
			type = T_DOUBLE;
			if (++pos == buf.end() || ((!is_decimal_char(*pos)) && *pos != '-'))
				return T_UNKNOWN;
			if (read_int() == T_UNKNOWN) 
				return T_UNKNOWN;
			dval *= pow(10,ival);
		}
    
	if (negative) 
	{
		if (type == T_INT) 
		{
			ival = -ival;
		} 
		else 
		{
			dval = -dval;
		}
    }
    return type;
}

double SENPTokenizer::read_decimal() 
{
    //
    // here *pos is a digit
    //
    long intpart = 0;
    long divisor = 1;
    
	do 
	{
		intpart = intpart * 10 + decimal_value(*pos);
		divisor *= 10;
    } while(++pos != buf.end() && is_decimal_char(*pos));

    return (1.0 * intpart) / divisor;
}

SENPTokenizer::TokenType SENPTokenizer::nextToken() 
{
    while (pos != buf.end()) 
	{
		switch(*pos) 
		{
			case '"': return read_string();
			case '{': ++pos; return T_LPAREN;
			case '}': ++pos; return T_RPAREN;
			case '!':
						if (++pos != buf.end() && *pos == '=') 
						{
							oval = SX_ne; ++pos; return T_OP;
						} 
						else 
						{
							return T_UNKNOWN;
						}
			case '*':
						if (++pos != buf.end() && *pos == '<') 
						{
							oval = SX_sf; ++pos; return T_OP;
						} 
						else 
						{
							oval = SX_ss; return T_OP;
						}
			case '=': 
						oval = SX_eq; ++pos; return T_OP;
			case '>': 
						if (++pos != buf.end()) 
						{
							switch(*pos) 
							{
								case '*': 
											oval = SX_pf; ++pos; return T_OP;
								case '=':   oval = SX_ge; ++pos; return T_OP;
								default: oval = SX_gt; return T_OP;
							}
						} 
						else 
						{
							oval = SX_gt; return T_OP;
						}
				case '<': 
							if (++pos != buf.end() && *pos == '=') 
							{
								oval = SX_le; ++pos; return T_OP;
							} 
							else 
							{
								oval = SX_lt; return T_OP;
							}
				default: if (is_decimal_char(*pos) || *pos == '-') 
						 {
							return read_number();
						 } 
						 else if (is_first_ident_char(*pos)) 
						 { 
							sval.erase();
							do 
							{
								sval += *pos++;
							} while (pos != buf.end() && is_other_ident_char(*pos));
							return T_ID;
						} else 
						 {
							//
							// I simply ignore characters that I don't understand 
							//
							++pos;
						}
		}
    }
    return T_EOF;
}

string & encode_octal(string & buf, unsigned char x) 
{
    buf += char_value((x >> 6 & 3));
    buf += char_value((x >> 3 & 7));
    buf += char_value(x & 7);
    return buf; 
}

string & encode(string & buf, const string & s) 
{
    buf += '"';
    for(string::const_iterator i = s.begin(); i != s.end(); ++i) 
	switch(*i) 
	{
		case '\v':	buf += '\\';
					buf += 'v'; 
					break;
		case '\f':  buf += '\\';
					buf += 'f'; 
					break;
		case '\r':  buf += '\\';
					buf += 'r'; 
					break;
		case '\n':  buf += '\\';
					buf += 'n'; 
					break;
		case '\t':  buf += '\\';
					buf += 't'; 
					break;
		case '\b': 
					buf += '\\';
					buf += 'b'; 
					break;
		case '\a': 
					buf += '\\';
					buf += 'a'; 
					break;
		case '"':
					buf += '\\';
					buf += '"'; 
					break;
		case '\\':
					buf += '\\';
					buf += '\\'; 
					break;
		default:
					if (not_printable_char(*i)) 
					{
						//
						// here I handle other non-printable characters with
						// the \xxx octal notation
						//
						buf += '\\';
						encode_octal(buf, *i);
					} 
					else 
					{
						buf += *i;
					}
	}
    buf += '"';
    return buf;
}

#ifdef WITH_DATE
string & encode_rfc1123(string & buf, Date d) 
{
    struct tm t;
    d.get_tm(t);
    buf.append(Date::WeekDays[t.tm_wday]);
    buf.append(", ");
    buf += char_value(t.tm_mday / 10);
    buf += char_value(t.tm_mday % 10);
    buf += ' ';
    buf.append(Date::Months[t.tm_mon]);
    buf += ' ';
    int y = t.tm_year + 1900;
    buf += char_value(y / 1000);
    y %= 1000;
    buf += char_value(y / 100);
    y %= 100;
    buf += char_value(y / 10);
    buf += char_value(y % 10);
    buf += ' ';
    buf += char_value(t.tm_hour / 10);
    buf += char_value(t.tm_hour % 10);
    buf += ':';
    buf += char_value(t.tm_min / 10);
    buf += char_value(t.tm_min % 10);
    buf += ':';
    buf += char_value(t.tm_sec / 10);
    buf += char_value(t.tm_sec % 10);
    buf.append(" GMT");
    return buf;
};
#endif

#define SENP_MAX_NUM_REP_LEN 64

string & encode(string & buf, int32_t x) 
{
    //
    // QUICK HACK! ...work in progress...
    // 
    char tmp[SENP_MAX_NUM_REP_LEN];
    char *curs = &tmp[SENP_MAX_NUM_REP_LEN - 1];
    *curs = '\0';
    bool neg = false;
    if (x<0) 
	{
		x = abs(x);
		neg = true;
    }
    do 
	{
		--curs;
		*curs = char_value(x % 10);
		x /= 10;
		//
		// here I should check for boudary conditions (curs == tmp)
		// ... or maybe not.  ...work in progress...
		//
    } while (x > 0);
    
	if (neg) 
	{
		--curs;
		*curs = '-';
    }
    
	buf.append(curs);
    return buf;
}

string & encode(string & buf, double x) 
{
    //
    // I should probably implement this myself... no time now...
    // ...work in progress...
    //
    char tmpbuf[SENP_MAX_NUM_REP_LEN];
    sprintf(tmpbuf, "%g", x);
    buf.append(tmpbuf);
    return buf;
}

string & encode(string & buf, const AttributeValue & a) 
{
    switch (a.type()) 
	{
		case SX_string: return encode(buf, a.string_value());
		
		#ifdef WITH_DATE
		case SX_date: buf += '@'; return encode_rfc1123(buf, a.date_value());
		#endif

		case SX_integer: return encode(buf, a.int_value());
		case SX_bool: 
						if (a.bool_value()) 
						{
							buf.append(SENP::KwdTrue);
						} 
						else 
						{
							buf.append(SENP::KwdFalse);
						}
						return buf;
		case SX_double: return encode(buf, a.double_value());
    
		case SX_null: 
    
		default: // ...quick hack!...
			 buf.append(SENP::KwdNull);
		return buf;
    }
}

string & encode(string & buf, const AttributeConstraint & c) 
{
    if (c.op == SX_any) buf.append(SENP::KwdSeparator);
    	buf.append(SENP::Operators[c.op]);
    
	if (c.op == SX_any) return buf;
		return encode(buf, c.value);
}


string & encode(string & buf, const Notification & e) 
{
    buf.append(SENP::KwdNotification);
    buf.append(SENP::KwdLParen);
    for(Notification::const_iterator i = e.begin(); i != e.end(); ++i) 
	{
		buf.append(SENP::KwdSeparator);
		buf.append((*i).first);
		buf.append(SENP::KwdEquals);
		encode(buf, (*i).second);
    }
    buf.append(SENP::KwdRParen);
    return buf;
}

string & encode(string & buf, const NotificationSequence & s) 
{
    buf.append(SENP::KwdNotificationSequence);
    buf.append(SENP::KwdLParen);
    
	for(NotificationSequence::const_iterator i = s.begin(); i != s.end(); ++i) 
		encode(buf, *i);
    
	buf.append(SENP::KwdRParen);
    return buf;
}

string & encode(string & buf, const Filter & f) 
{
    buf.append(SENP::KwdFilter);
    buf.append(SENP::KwdLParen);
    
	for(Filter::const_iterator i = f.begin(); i != f.end(); ++i) 
	{
		buf.append(SENP::KwdSeparator);
		buf.append((*i).first);
		encode(buf, (*i).second);
    }

    buf.append(SENP::KwdRParen);
    return buf;
}

string & encode(string & buf, const Pattern & p) 
{
    buf.append(SENP::KwdPattern);
    buf.append(SENP::KwdLParen);
    
	for(Pattern::const_iterator i = p.begin(); i != p.end(); ++i) 
		encode(buf, *i);

    buf.append(SENP::KwdRParen);
    return buf;
}

string & SENP::encode_packet(string & buf, const SENPPacket & p) 
{
    buf.append(SENP::KwdSenp);
    buf.append(SENP::KwdLParen);

    buf.append(Version);
    buf.append(SENP::KwdEquals);
    encode(buf, (int32_t)p.version);
    buf.append(SENP::KwdSeparator);

    buf.append(Method);
    buf.append(SENP::KwdEquals);
    encode(buf, Methods[p.method]);
    buf.append(SENP::KwdSeparator);

    buf.append(Ttl);
    buf.append(SENP::KwdEquals);
    encode(buf, (int32_t)p.ttl);

    if (!p.id.empty()) 
	{
		buf.append(SENP::KwdSeparator);
		buf.append(Id);
		buf.append(SENP::KwdEquals);
		encode(buf, p.id);	    
    }

    if (!p.to.empty()) 
	{
		buf.append(SENP::KwdSeparator);
		buf.append(To);
		buf.append(SENP::KwdEquals);
		encode(buf, p.to);	    
    }

    if (!p.handler.empty()) 
	{
		buf.append(SENP::KwdSeparator);
		buf.append(Handler);
		buf.append(SENP::KwdEquals);
		encode(buf, p.handler);	    
    }

    buf.append(SENP::KwdRParen);

    switch(p.get_param_type()) 
	{
		case SENPPacket::P_Notification: 
											buf.append(SENP::KwdSeparator);
											encode(buf, *p.get_event()); 
											break;
		case SENPPacket::P_Filter:
										buf.append(SENP::KwdSeparator);
										encode(buf, *p.get_filter()); 
										break;
		case SENPPacket::P_Pattern: 
										buf.append(SENP::KwdSeparator);
										encode(buf, *p.get_pattern()); 
										break;
		
		case SENPPacket::P_NotificationSequence: 
										buf.append(SENP::KwdSeparator);
										encode(buf, *p.get_eventsequence()); 
										break;
		default:
		break;
			//
		// ...work in progress...
		//
	}
    return buf;
}

AttributeValue & decode_attribute(AttributeValue & v, SENPTokenizer & t) 
{
    switch(t.nextToken()) 
	{
		case SENPTokenizer::T_ID:
									if (t.sval == SENP::KwdTrue) 
									{
										v = true; 
									} 
									else if (t.sval == SENP::KwdFalse) 
									{
										v = false;
									} 
									else 
									{
										v = t.sval;
									}
		return v;
		case SENPTokenizer::T_STR: v = t.sval; return v;
		case SENPTokenizer::T_INT: v = t.ival; return v;
		case SENPTokenizer::T_BOOL: v = t.bval; return v;
		case SENPTokenizer::T_DOUBLE: v = t.dval; return v;
		default:
		{
			SENPInvalidFormat ex(PE_literal);
			throw(ex);
		}
    }
}

AttributeConstraint & decode_constraint(AttributeConstraint & c, SENPTokenizer & t) 
{
    switch (t.nextToken()) 
	{
		case SENPTokenizer::T_ID: 
		if (t.sval == SENP::Operators[SX_any]) 
		{
			c.op = SX_any;
		} 
		else 
		{
			{
				SENPInvalidFormat ex(PE_operator);
				throw(ex);
			}
		}
		break;
		case SENPTokenizer::T_OP: 
									c.op = t.oval;
									decode_attribute(c.value, t);
		break;
		default:
		{
			SENPInvalidFormat ex(PE_operator);
			throw(ex);
		}
    }
    return c;
}

Filter & decode_filter(Filter & f, SENPTokenizer & tokenizer) 
{
    if (tokenizer.nextToken() != SENPTokenizer::T_LPAREN) 
	{
	    SENPInvalidFormat ex(PE_lparen);
	    throw(ex);
	}
    f.clear();
    int ttype;
    while ((ttype = tokenizer.nextToken()) != SENPTokenizer::T_RPAREN) 
	{
		if (ttype != SENPTokenizer::T_ID && ttype != SENPTokenizer::T_STR)
	    {
			SENPInvalidFormat ex(PE_anamerparen);
			throw(ex);
	    }
		AttributeConstraint ac;
		string name = tokenizer.sval;
		f.add_constraint(name, decode_constraint(ac, tokenizer));
    }
    return f;
}

Notification & decode_event(Notification & e, SENPTokenizer & tokenizer) 
{
    if (tokenizer.nextToken() != SENPTokenizer::T_LPAREN) 
	{
	    SENPInvalidFormat ex(PE_lparen);
	    throw(ex);
	}

    e.clear();
    int ttype;
    while ((ttype = tokenizer.nextToken()) != SENPTokenizer::T_RPAREN) 
	{
		if (ttype != SENPTokenizer::T_ID && ttype != SENPTokenizer::T_STR)
	    {
			SENPInvalidFormat ex(PE_anamerparen);
			throw(ex);
	    }
		string name = tokenizer.sval;
		if (tokenizer.nextToken() != SENPTokenizer::T_OP || tokenizer.oval != SX_eq)
	    {
			SENPInvalidFormat ex(PE_operator);
			throw(ex);
	    }
		decode_attribute(e[name], tokenizer);
    }
    return e;
}

NotificationSequence & decode_sequence(NotificationSequence & s, SENPTokenizer & tokenizer) 
{
    if (tokenizer.nextToken() != SENPTokenizer::T_LPAREN) 
	{
	    SENPInvalidFormat ex(PE_lparen);
	    throw(ex);
	}

    s.clear();
    int ttype;

    while ((ttype = tokenizer.nextToken()) != SENPTokenizer::T_RPAREN) 
	{
		if (ttype != SENPTokenizer::T_ID && ttype != SENPTokenizer::T_STR && tokenizer.sval != SENP::KwdNotification)
	    {
			SENPInvalidFormat ex(PE_event);
			throw(ex);
	    }
		Notification nn;
		s.push_back(nn);
		decode_event(s[s.size()-1], tokenizer);
    }
    return s;
}

Pattern decode_pattern(Pattern &p, SENPTokenizer & tokenizer) 
{
    if (tokenizer.nextToken() != SENPTokenizer::T_LPAREN) 
	{
	    SENPInvalidFormat ex(PE_lparen);
	    throw(ex);
	}

    p.clear();
    int ttype;

    while ((ttype = tokenizer.nextToken()) != SENPTokenizer::T_RPAREN) 
	{
	
		if (ttype != SENPTokenizer::T_ID && ttype != SENPTokenizer::T_STR && tokenizer.sval != SENP::KwdFilter)
	    {
			SENPInvalidFormat ex(PE_filter);
			throw(ex);
	    }
		Filter ff;
		p.push_back(ff);
		decode_filter(p[p.size()-1], tokenizer);
    }
    return p;
}

static const string ErrAttrName = "<attribute-name>";

SENP::MethodType decode_method(const string & method) 
{
    static map<string, SENP::MethodType> mmap;
    if (mmap.empty())
		for(SENP::MethodType i = 0; i < SENP_METHODSCOUNT; ++i) 
			mmap[SENP::Methods[i]] = i;
    
	map<string, SENP::MethodType>::iterator i = mmap.find(method);
    if (i == mmap.end()) 
	{
		return SENP::NOP;
    } 
	else 
	{
		return (*i).second;
    }
}

SENPPacket & SENP::decode_packet(SENPPacket & res, const string & strpkt) 
{
    SENPTokenizer tokenizer(strpkt);

    if (tokenizer.nextToken() != SENPTokenizer::T_ID || tokenizer.sval != SENP::KwdSenp) 
	{
		{
			SENPInvalidFormat ex(PE_senp);
			throw(ex);
		}
    }
    if (tokenizer.nextToken() != SENPTokenizer::T_LPAREN) 
	{
	    SENPInvalidFormat ex(PE_lparen);
	    throw(ex);
	}

    int ttype;
    string name;
    while ((ttype = tokenizer.nextToken()) != SENPTokenizer::T_RPAREN) 
	{
		string attrname;
		AttributeValue av;
		if (ttype != SENPTokenizer::T_ID)
	    {
			SENPInvalidFormat ex(PE_anamerparen);
			throw(ex);
	    }
		attrname = tokenizer.sval;
		if (tokenizer.nextToken() != SENPTokenizer::T_OP || tokenizer.oval != SX_eq)
	    {
			SENPInvalidFormat ex(PE_operator);
			throw(ex);
	    }
		decode_attribute(av, tokenizer);
		if (attrname ==  SENP::Method) 
		{
	    
			if (av.type() == SX_string) 
			{
				res.method = decode_method(av.string_value());
			} 
			else 
			{
			//
			// ...work in progress...
			//
			}
		} 
		else if (res.ttl == SENP_DEFAULTTTL && (attrname ==  Ttl)) 
		{
			if (av.type() == SX_integer) 
			{
				if (av.int_value() > SENP_DEFAULTTTL) 
				{
					res.ttl = SENP_DEFAULTTTL;
				} 
				else if (av.int_value() < 0) 
				{
					res.ttl = 0;
				} 
				else 
				{
					res.ttl = (uint8_t)av.int_value();
				}
			}
		} 
		else if ((attrname ==  SENP::Version)) 
		{
			if (av.type() == SX_integer) 
				res.version = av.int_value();
		} 
		else if (attrname ==  SENP::Id) 
		{
			if (av.type() == SX_string) 
				res.id = av.string_value();
		} 
		else if (attrname ==  SENP::To) 
		{
			if (av.type() == SX_string) 
				res.to = av.string_value();
		} 
		else if (attrname == SENP::Handler) 
		{
			if (av.type() == SX_string) 
				res.handler = av.string_value();
		} 
		else 
		{
			//
			// ...work in progress...
			//
		}
	}
    //
    // now reads the optional parameter: either a filte or an event
    //
    switch (tokenizer.nextToken()) 
	{
		case SENPTokenizer::T_EOF: return res;
		case SENPTokenizer::T_ID: 
									if ((tokenizer.sval ==  SENP::KwdFilter)) 
									{
										Filter * f = new Filter();
										try 
										{
											decode_filter(*f, tokenizer);
										} 
										catch (exception &ex) 
										{
											delete(f);
											throw ex;
										}
										res.set_param(f);
										return res;
									} 
									else if ((tokenizer.sval == SENP::KwdNotification)) 
									{
										Notification * n = new Notification();
										try 
										{
											decode_event(*n, tokenizer);
										} 
										catch (exception & ex) 
										{
											delete(n);
											throw ex;
										}
										res.set_param(n);
										return res;
									} 
									else if ((tokenizer.sval ==  SENP::KwdNotificationSequence)) 
									{
										NotificationSequence * ns = new NotificationSequence();
										try 
										{
											decode_sequence(*ns, tokenizer);
										} 
										catch (exception & ex) 
										{
											delete(ns);
											throw ex;
										}
										res.set_param(ns);
										return res;
									} 
									else if ((tokenizer.sval ==  SENP::KwdPattern)) 
									{
										Pattern * p = new Pattern();
										try 
										{
											decode_pattern(*p, tokenizer);
										} 
										catch (exception & ex) 
										{
											delete(p);
											throw ex;
										}
										res.set_param(p);
										return res;
									}
									default:
											{
												SENPInvalidFormat ex(PE_efps);
												throw(ex);
											}
    }
}

#ifdef HAVE_CXX_NAMESPACE
} // namespace Siena
#endif

