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

#ifndef TGAIMAGE_H
#define TGAIMAGE_H

#include "csgfxldr/csimage.h"

/**
 * The TGA Image Loader.
 */
class csTGAImageLoader : public csImageLoader
{
protected:
  /// Try to load the image
  virtual csImageFile* LoadImage (UByte* iBuffer, ULong iSize, int iFormat);
};

/**
 * An csImageFile subclass for reading TGA files.
 */
class ImageTgaFile : public csImageFile
{
  friend class csTGAImageLoader;

private:
  /// Initialize the image object
  ImageTgaFile (int iFormat) : csImageFile (iFormat) { };
  /// Try to read the PNG file from the buffer and return success status
  bool Load (UByte* iBuffer, ULong iSize);
};

#endif
