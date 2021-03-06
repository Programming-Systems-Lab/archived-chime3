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

#ifndef __CS_PLATFORM_H__
#define __CS_PLATFORM_H__

// Include volatile.h which contains the volatile configuration macros
#if defined (WIN32_VOLATILE)
#  include "cssys/win32/volatile.h"
#elif defined (MACOS_VOLATILE)
#  include "cssys/mac/volatile.h"
#else
#  include "volatile.h"
#endif

#endif // __CS_PLATFORM_H__
