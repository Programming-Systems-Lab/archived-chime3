/*
** Siena configuration file for Visual C++ under win32
** 
**  This file is part of Siena, a wide-area event notification system.
**  See http://www.cs.colorado.edu/serl/siena/
**
**  Author: Antonio Carzaniga <carzanig@cs.colorado.edu>
**  See the file AUTHORS for full details. 
**
**  Copyright (C) 1998-2001 University of Colorado
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
**  USA, or send email to serl@cs.colorado.edu.
** 
** 
** $Id$
*/
#ifndef _SIENACONF_H_
#define _SIENACONF_H_

#define VERSION "0.4"

/*
 *  types
 */
#define socklen_t int

#define uint8_t unsigned char
#define int32_t long

#define in_addr_t unsigned long
#define in_port_t unsigned short

#define ssize_t long

/*
 *  macros
 */
#define MAXHOSTNAMELEN 256

/* #undef INADDR_NONE */

/* 
 *  have/don't have stuff
 */
/* #undef HAVE_SIZE_T */
/* #undef HAVE_SSIZE_T */

/* #undef HAVE_STDINT_H */
/* #undef HAVE_INTTYPES_H */

/* #define TIME_WITH_SYS_TIME 1 */
/* #undef HAVE_SYS_TIME_H */

/* #undef HAVE_CXX_NAMESPACE */
#define HAVE_CXX_EXCEPTIONS 1

/* #undef HAVE_INET_NTOP */

#ifdef WITH_DATE
#undef WITH_DATE
#endif

#ifndef USING_WIN32
#define USING_WIN32
#endif

/*******************/

/* Ignore VC++ 6 compiler warning for truncated symbol names when debugging */

#ifdef USING_VC6
#pragma warning( disable : 4786 )
#endif

/********************/

#ifdef _MSC_EXTENSIONS          // _MSC_EXTENSIONS is defined by VC++ when
#define DLLIMPORT __declspec (dllimport)
#else
#define DLLIMPORT
#endif

#endif
