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
#ifndef _Siena_h
#define _Siena_h

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

#include <vector>
#include <map>
#include <exception>

#ifdef WITH_DATE
#include <siena/Date.h>
#endif

#include <siena/SienaId.h>

#ifdef HAVE_CXX_NAMESPACE
/** Siena API package
 **/
namespace Siena {
#endif

/** \mainpage Siena C++ API Documentation
 *
 * This documentation describes the C++ application programming
 * interface to the <a
 * href="http://www.cs.colorado.edu/serl/siena/">Siena</a> event
 * notification service.  The abstract interface of Siena is described
 * by SienaBase, SienaPull, and SienaPush.  A client application,
 * being a publisher or a subscriber or both, accesses the Siena
 * service through an implementation of an abstract interface.  The
 * current API provides one implementation, given by the ThinClient
 * class.
 *
 * <p>The data model of Siena is documented in Notification and
 * AttributeValue.  The filter model is documented in Filter and
 * AttributeConstraint.
 * 
 * <p>See the <a href="examples.html">Example Section</a> for code
 * examples and compilation instruction.
 **/

/** \example subscriber.cc
 * 
 *  This is an example of how to program a simple subscriber
 *  application.  To compile this example on a GNU/Linux system or on
 *  other Unix-like systems, assuming Siena has been installed in
 *  $prefix, execute:
 * 
 *  <pre>
 *  c++ subscriber.cc -o subscriber -I$prefix/include -L$prefix/lib -lsiena
 *  </pre>
 *
 *  On some platforms (notably Solaris), you will have to link
 *  additional libraris (e.g., -lsocket -lnsl).
 * 
 *  <p>To compile this example on a win32 system, using Visual C++, assuming
 *  Siena has been installed in \%prefix\%, execute:
 * 
 *  <pre>
 *  cl -I\%prefix\%\include -TP -GX -c subscriber.cc
 *  link -out:subscriber subscriber.obj \%prefix\%\siena.lib wsock32.lib
 *  </pre>
 **/

/** \example publisher.cc 
 *  This is an example of how to program a simple subscriber
 *  application.  To compile this example on a GNU/Linux system or on
 *  other Unix-like systems, assuming Siena has been installed in
 *  $prefix, execute:
 * 
 *  <pre>
 *  c++ publisher.cc -o publisher -I$prefix/include -L$prefix/lib -lsiena
 *  </pre>
 *
 *  On some platforms (notably Solaris), you will have to link
 *  additional libraris (e.g., -lsocket -lnsl).
 * 
 *  <p>To compile this example on a win32 system, using Visual C++, assuming
 *  Siena has been installed in \%prefix\%, execute:
 * 
 *  <pre>
 *  cl -I\%prefix\%\include -TP -GX -c publisher.cc
 *  link -out:publisher publisher.obj \%prefix\%\siena.lib wsock32.lib
 *  </pre>
 **/

class SienaBase;
class SienaPush;
class SienaPull;
class Notifiable;
class AttributeValue;
class Notification;
class AttributeConstraint;
class Filter;
class Pattern;
class NotificationSequence;

class BadType;

/** minimal publish-only interface to Siena.
 *
 *  this interface (virtual class) represents a minimal interface to
 *  the Siena service that is limited to publications.  In practice,
 *  this interface does not allow subscriptions.
 **/
class SienaBase {
public:
    /** publish a notification 
     **/
    virtual void	publish(const Notification &e)			= 0;

    /** advertises a class of notifications
     **/
    virtual void	advertise(const Filter &f, const SienaId &);

    /** cancels a set of advertisements
     **/
    virtual void	unadvertise(const Filter &, const SienaId &);
    /** cancels all advertisements for the same id 
     **/
    virtual void	unadvertise(const SienaId &);

    /** closes the interface to the Siena service 
     **/
    virtual void	shutdown();
};

/** asynchronous ("push") interface to Siena
 *
 *  this is a complete interface to the Siena service.  This interface
 *  uses the "push" style to notify subscribers.  It is asynchronous
 *  in the sense that a notification is passed to a subscriber through
 *  the asynchronous execution of the <em>call-back</em> method
 *  <code>Notifiable::notify()</code> on the subscriber object.
 *  Subscriptions and unsubscriptions are managed on a per-notifiable
 *  basis.
 **/
class SienaPush: public SienaBase {
public:
    virtual void	subscribe(const Filter &f, Notifiable *n)	= 0;
    virtual void	subscribe(const Pattern &p, Notifiable *n);

    virtual void	unsubscribe(const Filter &f, Notifiable *n)	= 0;
    virtual void	unsubscribe(const Pattern &p, Notifiable *n);
    virtual void	unsubscribe(Notifiable *)			= 0;

    virtual void	suspend(Notifiable *n)				= 0;
    virtual void	resume(Notifiable *)				= 0;
};

/** synchronous ("pull") interface to Siena
 *
 *  this is a complete interface to the Siena service.  This interface
 *  uses the "pull" style to notify subscribers.  In practice,
 *  subscribers receive notifications by polling the service, through
 *  the <code>get_notification()</code> method of this interface.
 *  Notifications are therefore synchronous with respect to the
 *  execution of subscribers.  Subscriptions and unsubscriptions are
 *  global to each <code>SienaPull</code> object.
 **/
class SienaPull: public SienaBase {
public:
    /** subscribes for a class of notifications 
     **/
    virtual void	subscribe(const Filter &f)			= 0;
    /** subscribes for a class of sequences of notifications 
     **/
    virtual void	subscribe(const Pattern &p);

    /** cancels subscriptions to notifications 
     **/
    virtual void	unsubscribe(const Filter &f)			= 0;
    /** cancels subscriptions to sequences notifications 
     **/
    virtual void	unsubscribe(const Pattern &p);
    /** cancel all subscriptions 
     **/
    virtual void	unsubscribe()					= 0;

    virtual void	suspend()					= 0;
    virtual void	resume()					= 0;

    /** receives a notification 
     *  
     *  receives a notification.  This method is synchronous, meaning
     *  that it suspends waiting for incoming notifications.
     **/
    virtual Notification *		get_notification()		= 0;
    virtual NotificationSequence *	get_notification_sequence()	= 0;
};

/** subscriber object
 *
 *  A <code>Notifiable</code> represents an event consumer.
 *  Notifications are delivered to a <code>Notifiable</code> by
 *  calling its <code>notify</code> method.  A <code>Notifiable</code>
 *  implement its specific response to event notifications by
 *  specializing <code>notify</code>.
 *
 *  <p>Example:
 *  <pre>
 *      class Ticker : public Notifiable {
 *          virtual void notify(Notification &e) { 
 *              cout << e["price"].int_value() << endl; 
 *          }
 *      };
 *      // ...
 *      Filter f;
 *      Ticker t;
 *      f.add_constraint("price", SX_lt, 500);
 *      S->subscribe(f, t);
 *      // ...
 *  </pre>
 **/
class Notifiable {
 public:
    /** response to a notification of an event
     *
     *	The event service calls this method to notify the interested
     *	party. The interested party specializes this method to
     *	implement its response to the event notification.
     *
     *	@param e notified event
     **/
    virtual void	notify(Notification &e)				= 0;

    /** response to a notification of a sequence of events
     *	
     *	The event service calls this method to notify the interested
     *	party. The interested party specializes this method to
     *	implement its response to the event sequence notification.
     *
     *	@param s notified sequence
     **/
    virtual void	notify(NotificationSequence &s)			= 0;
};

/** type identifier
 *
 *  Siena has some of the most common data types. <code>SXType</code>
 *  enumerates the available types in Siena.
 *
 *  <p>Example:
 *  <pre>
 *      Notification e;
 *      // ...
 *      SXType t;
 *      t = e["quantity"].type();
 *      switch(t) {
 *          case SX_integer: 
 *          // ...
 *          case SX_string:
 *          // ...
 *      }
 *  </pre>
 **/
enum SXType {
    /** undefined null type 
     **/
    SX_null			= 0,
    /** string 
     **/
    SX_string			= 1,
    /** signed integer number  
     **/
    SX_integer			= 2,
    /** boolean value 
     **/
    SX_bool			= 3,
    /** time/date
     *
     *  <b><em>not yet implemented!</em></b> 
     **/
    SX_date			= 4,
    /** floating point number 
     **/
    SX_double			= 5,
};

/** Textual representation of type identifiers 
 *
 *    strings representing type identifiers. Its values are
 *    <code>null</code>, <code>string</code>, <code>integer</code>,
 *    <code>boolean</code>, <code>data</code>, <code>float</code>, and
 *    <code>binary</code>. The mappings are obvious with respect to the
 *    values of <code>SXType</code>, for example
 *    <code>SXTypeDescription[SX_bool] == "boolean"</code>.  
 *
 *    @see SXType
 **/
extern const string	SXTypeDescription[];

/** value of an attribute in a notification
 *
 *    An <code>AttributeValue</code> stores the value of an
 *    attribute. Attributes can be of a pre-defined set of types
 *    (SXType).  <code>AttributeValue</code> provides methods for
 *    accessing and assigning values in the form of C++ types.  In
 *    particular, an <code>AttributeValue</code> of type
 *    <code>SX_integer</code> can be assigned to and from an
 *    <code>int</code>, an <code>AttributeValue</code> of type
 *    <code>SX_string</code> can be assigned to and from a C++
 *    standard <code>string</code>, etc. 
 *    <p>
 *
 *    <i>Example</i>:
 *    <pre>
 *    AttributeValue x, y;
 *    //
 *    // now x.type() == y.type() == SX_null
 *    //
 *    x = true;
 *    y = x;
 *    //
 *    // now x.type() == y.type() == SX_bool
 *    //
 *    int i = 10;
 *    if (x.bool_value()) y = i + 10;
 *    x = i + 20;
 *    cout << x.int_value() + y.int_value() << endl;
 *    i = x;
 *    y = "ciao";
 *    cout << y.string_value() << endl;
 *    </pre>
 **/
class AttributeValue {
 public:
    
    /** constructs a null value 
     **/
			AttributeValue();
    /** copy constructor 
     **/
			AttributeValue(const AttributeValue &v);
    /** construct from a string
     **/
			AttributeValue(const string &);
    /** construct from a C-style string 
     **/
			AttributeValue(const char *);
    /** construct from an int
     **/
			AttributeValue(int);
    /** construct from an short
     **/
			AttributeValue(short);
    /** construct from an long
     **/
			AttributeValue(long);
    /** construct from a bool
     **/
			AttributeValue(bool);
    /** construct from a double
     **/
			AttributeValue(double);
    /** construct from a Date
     **/
#ifdef WITH_DATE
			AttributeValue(const Date &);
#endif
			~AttributeValue();

    /** returns the actual type of this value 
     **/
    SXType		type()					const;

    /** returns a reference to the value as an integer
     *
     *	returns a reference to the value as an integer
     *	<p>
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = 20;
     *	    cout << x.int_value();
     *	    x.int_value()++;
     *	    cout << x.int_value();
     *	</pre>
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_integer</code> 
     *	 
     **/
    int32_t &		int_value();

    /** returns a reference to the value as a boolean 
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_bool</code> 
     **/
    bool &		bool_value();

    /** returns a reference to the value as a string 
     *		   
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = "ciao";
     *	    cout << x.string_value();
     *	    x.string_value() += " ciao";
     *	    cout << x.string_value();
     *	</pre>
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_string</code>
     **/
    string &		string_value();

    /** returns a reference to the value as a double 
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_double</code> 
     **/
    double &		double_value();
#ifdef WITH_DATE
    /** returns a reference to the value as a date 
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_date</code> 
     **/
    Date &		date_value();
#endif
    /** returns a reference to the value as an integer
     *
     *	@exception BadType exception is generated when
     *	           <code>type() != SX_integer</code>
     **/
    const int32_t &	int_value()				const;
    /** returns a copy of the value as a boolean 
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_bool</code>  
     **/
    const bool &	bool_value()				const;
    /** returns a copy of the value as a string 
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_string</code>  
     **/
    const string &	string_value()				const;
    /** returns a copy of the value as a double 
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_double</code>  
     **/
    const double &	double_value()				const;
#ifdef WITH_DATE
    /** returns a copy of the value as a date 
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_date</code>  
     **/
    const Date &	date_value()				const;
#endif

    /** conversion operator to <code>long</code>
     *
     *	returns a copy of the value as an integer whenever the
     *	<code>AttributeValue</code> is used in place of an
     *	<code>int</code>. 
     *	<p>
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = 20;
     *	    long l = x;
     *	</pre>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_int</code> 
     **/
			operator long ()			const;

    /** conversion operator to <code>int</code>
     *
     *	returns a copy of the value as an integer whenever the
     *	<code>AttributeValue</code> is used in place of an
     *	<code>int</code>. 
     *	<p>
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = 20;
     *	    int i = x;
     *	</pre>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_int</code> 
     **/
			operator int ()			const;

    /** conversion operator to <code>short</code>
     *
     *	returns a copy of the value as an integer whenever the
     *	<code>AttributeValue</code> is used in place of an
     *	<code>int</code>. 
     *	<p>
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = 20;
     *	    short s = x;
     *	</pre>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_int</code> 
     **/
			operator short ()			const;

    /** conversion operator to <code>string</code>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_string</code> 
     *
     *	@doc 
     *
     *	returns a copy of the value as a string whenever the
     *	<code>AttributeValue</code> is used in place of a
     *	<code>string</code>. 
     *	<p>
     *	<i>Example</i>:
     *	<pre>
     *	    AttributeValue x = "Siena";
     *	    string s = x;
     *	</pre>
     **/
			operator string ()			const;
    /** conversion operator to <code>bool</code>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_bool</code> 
     *
     *	@doc conversion operator to <code>bool</code>
     **/
			operator bool ()			const;
    /** conversion operator to <code>double</code>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_double</code> 
     *
     *	@doc conversion operator to <code>double</code>
     **/
			operator double ()			const;
#ifdef WITH_DATE
    /** conversion operator to <code>Date</code>
     *
     *  @exception BadType exception is generated when
     *	           <code>type() != SX_date</code> 
     *
     *	@doc conversion operator to <code>Date</code>
     **/
			operator Date ()			const;
#endif
    /** copy assignment
     **/
    AttributeValue &	operator = (const AttributeValue &);
    /** from string 
     **/
    AttributeValue &	operator = (const string &);
    /** from C-style string 
     **/
    AttributeValue &	operator = (const char *);
    /** from int 
     **/
    AttributeValue &	operator = (int);
    /** from short
     **/
    AttributeValue &	operator = (short);
    /** from long
     **/
    AttributeValue &	operator = (long);
    /** from bool 
     **/
    AttributeValue &	operator = (bool);
    /** from double 
     **/
    AttributeValue &	operator = (double);
#ifdef WITH_DATE
    /** from Date 
     **/
    AttributeValue &	operator = (const Date &);
#endif
    /** equality relation 
     **/
    bool		operator == (const AttributeValue &)	const;
    /** ordering relation 
     **/
    bool		operator < (const AttributeValue &)	const;

 protected:
    SXType		sxtype;
    union {
	string *	str;
	int32_t		num;
	bool		bln;
	double		dbl;
#ifdef WITH_DATE
	Date *		date;
#endif
    };
};

/** event notification
 *
 *    @see AttributeSet
 *    @doc
 *
 *    A notification in <em>Siena</em> is a set of named attributes.
 *    <code>Notification</code> represents an event notification. Attribute
 *    <em>names</em> are standard C++ <code>string</code>s while
 *    attribute <em>values</em> are <code>AttributeValue</code>s. A
 *    <em>name</em> uniquely identifies an attribute in an
 *    <code>Notification</code>.
 *    <p>
 *
 *    An <code>Notification</code> is implemented as a <code>map<string,
 *    AttributeValue></code>, therefore All the functions of the
 *    standard <code>map</code> template can be used to manipulate an
 *    <code>Notification</code>. For instance:
 *    
 *    <pre>
 *        Notification e;
 *        // ...
 *        // prints out attribute names of e
 *        //
 *        for(Notification::iterator i = e.begin(); i != e.end(); ++i)
 *            cout << (*i).first;
 *    </pre>
 *
 *    Of course, every operation on an <code>Notification</code> can take
 *    advantage of the access functions and assignment operators of
 *    <code>AttributeValue</code>.
 *
 *    <pre>
 *        Notification e;
 *
 *        e["stock"] = "XYZ";
 *        e["price"] = 12.34
 *        e["quantity"] = 567;
 *
 *        int q = e["quantity"];
 *    </pre>
 *
 *    The structure of names and types of an <code>Notification</code> is
 *    completely arbitrary for <em>Siena</em>. In other words,
 *    <em>Siena</em> does not require any pre-defined attribute. It is
 *    up to applications to agree on sets of names, types, and semantics
 *    of attributes. 
 **/
class Notification : public map<string, AttributeValue> {
 public:
    /** construct an empty notification 
     **/
			Notification();
    /** copy constructor 
     **/
			Notification(const Notification &);
};

/** operator identifier for <em>Siena</em>
 *
 *  Siena defines some of the most common operators (or binary
 *  relations). <code>SXOperator</code> is the type of operator
 *  identifiers.
 **/
enum SXOperator {
    /** equals 
     **/ 
    SX_eq		= 1, 
    /** less than.
     *
     *  less than. Integers and doubles are ordered as usual, strings
     *  are sorted in lexicographical order. For booleans,
     *  <code>false</code> < <code>true</code>.
     **/
    SX_lt		= 2, 
    /** greater than 
     **/
    SX_gt		= 3, 
    /** greater or equal 
     **/
    SX_ge		= 4, 
    /** less or equal 
     **/
    SX_le		= 5, 
    /** suffix
     *	
     *	suffix. <em>x</em> <code>SX_sf</code> <em>y</em> is true if
     *	<em>y</em> is a suffix of <em>x</em>.  For example,
     *	<code>"software" SX_sf "ware" == true</code>, while
     *	<code>"software" SX_sf "w" == false</code>. <code>SX_sf</code>
     *	is defined for strings only.
     **/
    SX_sf		= 7,
    /** prefix
     *
     *	prefix.  <em>x</em> <code>SX_pf</code> <em>y</em> is true if
     *	<em>y</em> is a prefix of <em>x</em>.  For example,
     *	<code>"software" SX_pf "soft" == true</code>, while
     *	<code>"software" SX_pf "of" ==
     *	false</code>. <code>SX_pf</code> is defined for strings only.
     **/
    SX_pf		= 6,
    /** <em>any</em> (don't care)
     *
     *	<em>any</em> (don't care).  Always <code>true</code>.
     **/
    SX_any		= 8, 
    /** not equal 
     **/
    SX_ne		= 9, 
    /** contains substring
     *
     *	contains substring. <em>x</em> <code>SX_ss</code> <em>y</em>
     *	is true if <em>y</em> is a substring of <em>x</em>.  For
     *	example, <code>"software" SX_ss "war" == true</code>, while
     *	<code>"software" SX_ss "hard" ==
     *	false</code>. <code>SX_ss</code> is defined for strings only.
     **/
    SX_ss		= 10 
};

extern const string	SXOperatorDescription[];

/** semantics of Siena operators: applies an operator to two values.
 *
 *  Applies an operator to two values.  Note that Siena operators are
 *  all binary relations.  Operands are applied in the given order, so
 *  <code>apply_operator(op, x, y)</code> evaluates <em>x op y</em>.
 *  For example: <code>apply_operator(SX_lt, x, y)</code> corresponds
 *  to <em>x < y</em>
 *  
 *  <p> This function implements some minimal type-conversion (or type
 *  compatibility). In practice, integers are always compatible to
 *  doubles.  The result in case of incompatible types does not depend
 *  on the actual values of <code>x</code> and <code>y</code>.
 *  Specifically, it is always <code>true</code> if <code>op ==
 *  SX_xx</code> (don't care) or <code>op == SX_ne</code> (not
 *  equal). Otherwise it's <code>false</code>.
 *  
 *  Note also that some operators (relations) are defined over a
 *  limited set of types. In particular <code>SX_pf</code>,
 *  <code>SX_sf</code>, and <code>SX_ss</code> are defined for strings
 *  (<code>SX_string</code>) operators only.
 **/
extern bool		apply_operator(SXOperator op, 
				       const AttributeValue & x, 
				       const AttributeValue & y);

/** attribute constraint
 *
 *  An <code>AttributeConstraint</code> represents a condition posed
 *  on an attribute value. It stores an operator plus a comparison
 *  value.  The meaning of an attribute constraint is defined by its
 *  method <code>apply_to</code>
 *
 *  @see #AttributeValue
 **/
class AttributeConstraint {
 public:
    /** the constraint operator 
     **/
    SXOperator	op;
    /** the comparison value 
     **/
    AttributeValue	value;

    /** default constructor
     *
     *	<code>op</code> defaults to <code>SX_eq</code>, the
     *	comparison value is <em>null</em>. 
     **/
			AttributeConstraint();
    /** construct from an AttributeValue
     *
     *	<code>op</code> defaults to <code>SX_eq</code>
     *	(equality), the comparison value is initialized with
     *	<code>v</code>.
     *
     *	@param v initializes the comparison value
     **/
			AttributeConstraint(const AttributeValue & v);

    /** construct from an AttributeValue and a SXOperator
     *
     *	@param o the operator <code>op</code>
     *	@param v the comparison value 
     **/
			AttributeConstraint(SXOperator o,
					    const AttributeValue & v);
    /** copy constructor 
     **/
			AttributeConstraint(const AttributeConstraint & c);

    /** copy assignment 
     **/
    AttributeConstraint &	operator = (const AttributeConstraint & c);

    /** assignment from an AttributeValue
     *
     *	assigns the comparison value using <code>SX_eq</code>
     *  as the comparison operator 
     *
     *	@param v comparison value
     **/
    AttributeConstraint &	operator = (const AttributeValue & v);

    /** equality operator 
     **/
    bool		operator == (const AttributeConstraint & c) const;

    /** applies this constraint to an attribute value
     *
     *	@param v value to verify against <code>this</code> constraint
     **/
    bool		apply_to(const AttributeValue & v);
};

/** expression filtering notifications
 *
 *  A <code>Filter</code> represents a boolean expression to be
 *  evaluated against an <code>Notification</code>. It consists of a
 *  set of constraints. Each constraint poses a condition on an
 *  attribute of the <code>Notification</code>. A <code>Filter</code>
 *  can pose multiple constraints on the same attribute.  
 *
 *  <p>A <code>Filter</code> is implemented by means of a
 *  <code>multimap<string, AttributeConstraint></code>, therefore a
 *  <code>Filter</code> can be manipulated using all the methods of a
 *  standard <code>multimap</code>. Two additional utility methods are
 *  provided to add constraints.
 *
 *  @see AttributeConstraint
 **/
class Filter : public multimap<string, AttributeConstraint> {
 public:
    /** default constructor 
     *  
     *  creates an empty filter (always false)
     **/
			Filter();
    /** copy constructor 
     **/
			Filter(const Filter &);

    /** equality operator 
     **/
    bool		operator == (const Filter &)		const;

    /** adds a constraint to this filter
     **/
    iterator		add_constraint(const string &, 
				       SXOperator, const AttributeValue &);

    /** adds a constraint to this filter
     **/
    iterator		add_constraint(const string &, 
				       const AttributeConstraint &);

    Filter &		reduce();
};

/** expression of filters matching a sequence of notifications 
 *
 *  (<b>not yet implemented!</b>). 
 **/
class Pattern: public vector<Filter> {
//
// a Pattern is simply a list of (event) filters. This might be
// changed in the future, requiring a more complex data structure
//
 public:
    /** construct an empty pattern 
     **/
			Pattern();
    /** copy constructor 
     **/
			Pattern(const Pattern &);
};

class NotificationSequence: public vector<Notification> {
//
// a Pattern is simply a list of (event) filters. This might be
// changed in the future, requiring a more complex data structure
//
 public:
			NotificationSequence();
			NotificationSequence(const NotificationSequence &);
};

/** generic exception 
 **/
class SienaException: public exception {
    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

/** exception: service unavailable 
 **/
class ServiceUnavailable: public exception {
    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

/** model exception
 **/
class NotificationModelException: public SienaException {
    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

/** exception: attribute type mismatch 
 **/
class BadType: public NotificationModelException {
 public:
    /** wrong type 
     **/
    SXType			type;

    /** constructor with offending type 
     **/
				BadType(SXType);

    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

/** exception operator mismatch 
 **/
class BadOperator: virtual public NotificationModelException {
public:
    /** wrong operator 
     **/
    SXOperator		op;
    /** constructor with offending operator 
     **/
				BadOperator(SXOperator); 

    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

/** exception: false filter (defines an empty set of events) 
 **/
class NullFilter: virtual public NotificationModelException {
public:
    /** attribute name 
     **/
    string			name;
    /** conflicting constraint 
     **/
    AttributeConstraint		af1;
    /** conflicting constraint 
     **/
    AttributeConstraint		af2;

    /** constructor with two contradictory constraints 
     **/
				NullFilter(const AttributeConstraint &,
					   const AttributeConstraint &);
    /** constructor with two contradictory constraints and attribute name 
     **/
				NullFilter(const string &,
					   const AttributeConstraint &,
					   const AttributeConstraint &);

    /** C-style string representation of what went wrong 
     **/
    virtual const char *	what()				const;
};

#ifndef NO_INLINE
#include <siena/Siena.icc>
#endif

#ifdef HAVE_CXX_NAMESPACE
};
#endif

#endif
