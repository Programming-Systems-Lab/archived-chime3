/*
    Crystal Space utility library: vector class interface
    Copyright (C) 1998,1999,2000 by Andrew Zabolotny <bit@eltech.ru>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CS_GARRAY_H__
#define __CS_GARRAY_H__

// Common macro for declarations below
#define TYPEDEF_GROWING_ARRAY_EXT(Name, Type, ExtraConstructor, Extra)	\
  class Name								\
  {									\
    Type *root;								\
    int limit;								\
    int length;								\
  public:								\
    int Limit () const							\
    { return limit; }							\
    void SetLimit (int iLimit)						\
    {									\
      if (limit == iLimit) return;					\
      if ((limit = iLimit)!=0)						\
        root = (Type *)realloc (root, limit * sizeof (Type));		\
      else								\
      { if (root) { free (root); root = NULL; } }			\
    }									\
    Name ()								\
    { limit = length = 0; root = NULL; ExtraConstructor; }		\
    ~Name ()								\
    { SetLimit (0); }							\
    int Length () const							\
    { return length; }							\
    void SetLength (int iLength, int iGrowStep = 8)			\
    {									\
      length = iLength;							\
      int newlimit = ((length + (iGrowStep - 1)) / iGrowStep) * iGrowStep;\
      if (newlimit != limit) SetLimit (newlimit);			\
    }									\
    Type &operator [] (int n)						\
    { return root [n]; }						\
    Type &Get (int n)							\
    { return root [n]; }						\
    void Delete (int n)							\
    { memmove (root + n, root + n + 1, (limit - n - 1) * sizeof (Type)); }\
    Type *GetArray ()							\
    { return root; }							\
    void Push (const Type &val, int iGrowStep = 8)			\
    {									\
      SetLength (length + 1, iGrowStep);				\
      memcpy (root + length - 1, &val, sizeof (Type));			\
    }									\
    Extra								\
  }

/**
 * This is a macro that will declare a growable array variable that is able to 
 * contain a number of elements of given type.<p>
 * Methods:
 * <ul>
 * <li>void SetLimit (int) - set max number of values the array can hold
 * <li>int Limit () - query max number of values the array can hold
 * <li>void SetLength (int) - set the amount of elements that are actually used
 * <li>int Length () - query the amount of elements that are actually used
 * <li>operator [] (int) - return a reference to Nth element of the array
 * </ul>
 * Usage examples:
 * <pre>
 * TYPEDEF_GROWING_ARRAY (csLightArray, csLight*);
 * TYPEDEF_GROWING_ARRAY (csIntArray, int);
 * static csLightArray la;
 * static csIntArray ia;
 * </pre>
 */
#define TYPEDEF_GROWING_ARRAY(Name, Type)				\
  TYPEDEF_GROWING_ARRAY_EXT (Name, Type, ;,  ;)

/**
 * Same as TYPEDEF_GROWING_ARRAY but contains additionally an reference
 * counter, so that the object can be shared among different clients.
 * If you do an IncRef each time you make use of it and an DecRef when you're 
 * done, the array will be automatically freed when there are no more 
 * references to it.<p> 
 * Methods:
 * <ul>
 * <li>void IncRef ()/void DecRef () - Reference counter management
 * </ul>
 */
#define TYPEDEF_GROWING_ARRAY_REF(Name, Type)				\
  TYPEDEF_GROWING_ARRAY_EXT (Name, Type, RefCount = 0,			\
    int RefCount;							\
    void IncRef ()							\
    { RefCount++; }							\
    void DecRef ()							\
    {									\
      if (RefCount == 1) SetLimit (0);					\
      RefCount--;							\
    })

/**
 * This is a shortcut for above to declare a dummy class and a single
 * instance of that class.
 * <p>
 * Usage examples:
 * <pre>
 * DECLARE_GROWING_ARRAY (la, csLight*);
 * DECLARE_GROWING_ARRAY (ia, int);
 * </pre>
 */
#define DECLARE_GROWING_ARRAY(Name, Type)				\
  TYPEDEF_GROWING_ARRAY(__##Name,Type) Name

/**
 * Same as above but declares an object which has a reference counter.
 */
#define DECLARE_GROWING_ARRAY_REF(Name, Type)				\
  TYPEDEF_GROWING_ARRAY_REF(__##Name,Type) Name

#endif // __CS_GARRAY_H__
