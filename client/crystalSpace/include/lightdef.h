/*
    Copyright (C) 1998 by Jorrit Tyberghein
  
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

#ifndef __LIGHTDEF_H__
#define __LIGHTDEF_H__

/// Light level that is used when there is no light on the texture.
#define DEFAULT_LIGHT_LEVEL 20
/// Light level that corresponds to a normally lit texture.
#define NORMAL_LIGHT_LEVEL 128

/// Max number of polygons that can be lit by one light. (bad practice !!!@@@)
#define MAX_NUM_LIGHTMAP 2000

#endif // __LIGHTDEF_H__
