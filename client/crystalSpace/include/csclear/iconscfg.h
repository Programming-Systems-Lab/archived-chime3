/*
    Copyright (C) 2000 by Michael Long

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

#ifndef __CC_ICONSOLECONFIG_H__
#define __CC_ICONSOLECONFIG_H__

#include "csutil/scf.h"

SCF_VERSION(iConsoleConfig, 0, 0, 1);

struct iConsoleConfig : public iBase
{

  /// See if the console is active (receiving input)
  virtual bool GetActive() const = 0;

  /// Set the active state
  virtual void SetActive(bool active) = 0;

  /// Get the key code for toggling the console (default is 0, no toggle key)
  virtual int GetToggleKey() const = 0;

  /// Set the key code for toggling the console (setting it to 0 prevents any toggles)
  virtual void SetToggleKey(int key) = 0;

};

#endif __CC_ICONSOLECONFIG_H__
