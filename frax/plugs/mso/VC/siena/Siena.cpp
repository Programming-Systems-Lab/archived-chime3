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

#ifdef HAVE_CXX_NAMESPACE
namespace Siena {
#endif

#if defined(NO_INLINE)
#define inline
#include <siena/Siena.icc>
#endif

const string SXTypeDescription[] = 
{
    "null",
    "string",
    "integer",
    "boolean",
    "data",
    "float",
    "binary"
};

const string SXOperatorDescription[] = 
{
    "unknown operator",
    "equal",
    "suffix",
    "prefix",
    "less than",
    "greater than",
    "less equal",
    "greater equal",
    "any value",
    "not equal",
    "contains"
};

AttributeValue & AttributeValue::operator = (const AttributeValue &x) 
{
    if (sxtype == SX_string && str != NULL) 
	{
		if (x.sxtype == SX_string) 
		{
			if (x.str != NULL) 
			{
				*str = *x.str;
			} 
			else 
			{
				*str = "";			// this should never happen
			}
	    return *this;
		} 
		else 
		{
			delete(str);
		}
    } 
#ifdef WITH_DATE
    else if (sxtype == SX_date && date != NULL) 
	{
  		if (x.sxtype == SX_date) 
		{
  			if (x.date != NULL) 
			{
  				*date = *x.date;
  			} 
			else 
			{
  				*date = Date();			// this should never happen
  			}
  			return *this;
  		} 
		else 
		{
  			delete(date);
  		}
    }
#endif
    sxtype = x.sxtype;
    switch(sxtype) 
	{
		case SX_null: break;
		case SX_string: if (x.str != NULL)
							str = new string(*x.str);
						else str = new string();			// this should never happen
						break;
		#ifdef WITH_DATE
		case SX_date:
  		if (x.date != NULL)
  			date = new Date(*x.date);
  		else 
  			date = new Date();			// this should never happen
  		break;
#endif
		case SX_integer:
					num = x.num;
					break;
		case SX_bool:
					bln = x.bln;
		break;
		case SX_double:
					dbl = x.dbl;
					break;
	//
	// to be continued (with other types) ... work in progress ...
	//
		default:
				BadType bt(sxtype);
				throw( bt );
				break;
    }
    return *this;
}

AttributeValue::AttributeValue(const AttributeValue &x) : sxtype(x.sxtype)
{
    switch(sxtype) 
	{
		case SX_null: break;
		case SX_string:
						if (x.str != NULL)
							str = new string(*x.str);
						else 
						str = new string();			// this should never happen
						break;
	#ifdef WITH_DATE
		case SX_date:
  						if (x.date != NULL)
  							date = new Date(*x.date);
  						else 
  						date = new Date();			// this should never happen
  						break;
#endif
    case SX_integer:
						num = x.num;
						break;
    case SX_bool:
						bln = x.bln;
						break;
    case SX_double:
						dbl = x.dbl;
						break;
	//
	// to be continued (with other types) ... work in progress ...
	//
    default:
				BadType bt(sxtype);
				throw( bt );
				break;
    }
}

AttributeValue::~AttributeValue()
{
    switch(sxtype) 
	{
		case SX_string: if(str != NULL) delete(str); break;
		#ifdef WITH_DATE
		case SX_date: if(date != NULL) delete(date); break;
		#endif
		default: break;
    }
}

bool is_suffix(const string &x, const string &y)
{
    // true iff y is a suffix of x, e.g.:
    // x = "pippotto"
    // y = "otto"
    //
    string::const_reverse_iterator xri = x.rbegin();
    string::const_reverse_iterator yri = y.rbegin();
    while(yri != y.rend()) 
	{
		if (xri == x.rend() || *xri != *yri) 
			return false;
	
		++xri;
		++yri;
    }
    return true;
}

bool is_prefix(const string &x, const string &y)
{
    // true iff y is a prefix of x, e.g.:
    // x = "testolina"
    // y = "test"
    //
    string::const_iterator xi = x.begin();
    string::const_iterator yi = y.begin();
    while(yi != y.end()) 
	{
		if (xi == x.end() || *xi != *yi) 
			return false;
	
		++xi;
		++yi;
    }
    return true;
}

bool is_substr(const string &x, const string &y)
{
    // true iff y is a substring of x, e.g.:
    // x = "software"
    // y = "war"
    //
    return x.find(y) != x.npos;
}

/** semantics of attribute constraints: applies an operator to two values.
 *
 *    This function implements some minimal type-conversion (or type
 *    compatibility). In practice, it only considers integers as
 *    doubles.  Actually, I'm not even sure the extended semantics would
 *    make sense, e.g., would it be reasonable to write: <code>int x =
 *    12; string y = "30"</code>; and then ask <code>x &lt; y</code> ?
 *    What's the right answer to this?  We still need to specify these
 *    details 
 **/
bool apply_operator(SXOperator op, 
		    const AttributeValue &x, const AttributeValue &y)
{
    //
    // ...work in progress...
    //
    if (x.type() == SX_null)
	//
	// I'm not sure what the best semantics would be here
	//
	return y.type() == SX_null;

    switch(op) 
	{
		case SX_any: 
					return true;	
		case SX_eq: 
					switch(x.type()) 
					{
						case SX_string: return y.type() == SX_string && x.string_value() == y.string_value();
						case SX_bool: return y.type() == SX_bool && x.bool_value() == y.bool_value();
						case SX_integer: return (y.type() == SX_integer && x.int_value() == y.int_value()) || (y.type() == SX_double && x.int_value() == y.double_value());
						case SX_double: return (y.type() == SX_integer && x.double_value() == y.int_value()) || (y.type() == SX_double && x.double_value() == y.double_value());
						#ifdef WITH_DATE
  						case SX_date: return y.type() == SX_date
									  && x.date_value() == y.date_value();
						#endif
						default:					// I should probably
							return false;				// throw an exception
					}
		case SX_ne: 
		switch(x.type()) 
		{
			case SX_string: return y.type() != SX_string || x.string_value() != y.string_value();
			case SX_bool: return y.type() != SX_bool || x.bool_value() != y.bool_value();
			case SX_integer: switch(y.type()) 
							 {
								case SX_integer: return x.int_value() != y.int_value();
								case SX_double: return x.int_value() != y.double_value();
								default: return true;
							 }
			case SX_double:  
							switch(y.type()) 
							{
								case SX_integer: return x.double_value() != y.int_value();
								case SX_double: return x.double_value() != y.double_value();
								default: return true;
							}
			#ifdef WITH_DATE
  			case SX_date: return y.type() != SX_date || x.date_value() != y.date_value();
			#endif
			default:					// I should probably
					return false;				// throw an exception
		}
		case SX_ss: switch (x.type()) 
					{
						case SX_string: return y.type() == SX_string && is_substr(x.string_value(),y.string_value());
						default: return false;				// I should probably
					}						// throw an exception
		case SX_sf: 
					switch (x.type()) 
					{
						case SX_string: return y.type() == SX_string && is_suffix(x.string_value(), y.string_value());
						default: return false;				// I should probably
					}						// throw an exception
		case SX_pf:
					switch (x.type()) 
					{
						case SX_string: return y.type() == SX_string && is_prefix(x.string_value(), y.string_value());
						default: return false;				// I should probably
					}						// throw an exception
		case SX_lt: 
					switch(x.type()) 
					{
						case SX_string: return y.type() == SX_string && x.string_value() < y.string_value();
						case SX_integer: return (y.type() == SX_integer && x.int_value() < y.int_value()) || (y.type() == SX_double && x.int_value() < y.double_value());
						case SX_bool: return y.type() == SX_bool && x.bool_value() < y.bool_value();
						case SX_double: return (y.type() == SX_integer && x.double_value() < y.int_value()) || (y.type() == SX_double && x.double_value() < y.double_value());
						#ifdef WITH_DATE
  						case SX_date: return y.type() == SX_date && x.date_value() < y.date_value();
						#endif
						default:					// I should probably
						return false;				// throw an exception
					}
		case SX_gt: 
					switch(x.type()) 
					{
						case SX_string: return y.type() == SX_string && x.string_value() > y.string_value();
						case SX_integer: return (y.type() == SX_integer && x.int_value() > y.int_value()) || (y.type() == SX_double && x.int_value() > y.double_value());
						case SX_bool: return y.type() == SX_bool && x.bool_value() > y.bool_value();
						case SX_double: return (y.type() == SX_integer && x.double_value() > y.int_value()) || (y.type() == SX_double && x.double_value() > y.double_value());
						#ifdef WITH_DATE
  						case SX_date: return y.type() == SX_date && x.date_value() > y.date_value();
						#endif
						default:					// I should probably
						return false;				// throw an exception
					}
		case SX_le: 
					switch(x.type()) 
					{
						case SX_string: return y.type() == SX_string && x.string_value() <= y.string_value();
						case SX_integer: return (y.type() == SX_integer && x.int_value() <= y.int_value()) || (y.type() == SX_double && x.int_value() <= y.double_value());
						case SX_bool: return y.type() == SX_bool && x.bool_value() <= y.bool_value();
						case SX_double: return (y.type() == SX_integer && x.double_value() <= y.int_value()) || (y.type() == SX_double && x.double_value() <= y.double_value());
						#ifdef WITH_DATE
  						case SX_date: return y.type() == SX_date && x.date_value() <= y.date_value();
						#endif
						default:					// I should probably
									return false;				// throw an exception
					}
		case SX_ge: 
					switch(x.type()) 
					{
						case SX_string: return y.type() == SX_string && x.string_value() >= y.string_value();
						case SX_integer: return (y.type() == SX_integer && x.int_value() >= y.int_value()) || (y.type() == SX_double && x.int_value() >= y.double_value());
						case SX_bool: return y.type() == SX_bool && x.bool_value() >= y.bool_value();
						case SX_double: return (y.type() == SX_integer && x.double_value() >= y.int_value()) || (y.type() == SX_double && x.double_value() >= y.double_value());
						#ifdef WITH_DATE
  						case SX_date: return y.type() == SX_date && x.date_value() >= y.date_value();
						#endif
						default:					// I should probably
								return false;				// throw an exception
					}
		default:
					return false;					// exception ?
    }
    return false;
}

//
// Exceptions
//
const char * SienaException::what() const 
{
    static const string _descr = "unknown Siena exception";
    return _descr.c_str();
}

const char * NotificationModelException::what() const 
{
    static const string _descr = "event model exception";
    return _descr.c_str();
}

const char * BadType::what() const 
{
    static const string _descr = "bad type";
    return _descr.c_str();
}

const char * BadOperator::what() const 
{
    static const string _descr = "bad operator";
    return _descr.c_str();
}

const char * NullFilter::what() const 
{
    static const string _descr = "null filter";
    return _descr.c_str();
}

#ifdef HAVE_CXX_NAMESPACE
} // namespace Siena
#endif

