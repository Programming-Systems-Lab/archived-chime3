/*
    Copyright (C) 2000 by Samuel Humphreys

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

#ifndef __PTTEX_H__
#define __PTTEX_H__

#include <stdarg.h>

class csMaterialWrapper;
class csEngine;
class csSector;

class csView;
struct iGraphics2D;
struct iGraphics3D;
struct iSystem;
struct iMetaBalls;
struct MetaParameters;
struct iTextureManager;
struct iTextureHandle;
struct iMaterialHandle;
struct iImage;
struct iFont;

enum TextAlignmentModes {ALIGN_LEFT,ALIGN_RIGHT,ALIGN_CENTER};

class ProcTexture
{
 protected:

  static iTextureManager *s_proc_txtmgr;
  static iTextureHandle *s_proc_logo_tex;
  static iTextureHandle *s_main_logo_tex;
  static int logo_w;
  static int logo_h;
  bool Active;
  bool PTReady;
  int TexFlags;
  char *Name;
  char *FileName;
  iTextureHandle *logo_tex;
  csMaterialWrapper* ptmat;
  int mat_w, mat_h; 
  iGraphics3D *ptG3D;
  iGraphics2D *ptG2D;

  iFont *courierFont;
  int write_colour;
  int clear_colour;

  bool key_colour;
  int key_red, key_green, key_blue;
 public:
  static iSystem *System;
  static csEngine *Engine;
  static iTextureManager *MainTxtMgr;


  ProcTexture ();
  virtual ~ProcTexture ();

  virtual bool Initialize ();

  virtual bool PrepareAnim (csSector* room);

  virtual void Animate (csTicks current_time, csTicks elapsed_time) = 0;

  virtual bool EatKeyPress (int /*key*/, bool /*shift*/, 
			    bool /*alt*/, bool /*ctrl*/)  
  { return false; }

  virtual void Report (iGraphics2D* /*mg2d*/, 
		       const int /*fw*/, const int /*fh*/, int& /*offset*/) 
  {};

  void PrintToScreen (iGraphics3D *mainG3D, int &y, bool LHS);

  char *GetName ()
  { return Name; }

 protected:
  void WriteShadow (iGraphics2D *mg2d, 
		    int align, int x, int y, int fg, char *str,...);
  void Write (iGraphics2D *mg2d, 
	      int align, int x, int y, int fg, int bg, char *str,...);

  iImage* ProcTexture::LoadImage (iTextureManager *txtmgr, const char* name);

  void LoadLogo ();
};

class EngineProcTexture : public ProcTexture
{
 protected:
  csView* bot_view;
  csSector *Room;
  int bot_fov;
  bool manual_cam;
  bool animate_bots;
  float angle;

 public:
  EngineProcTexture ();
  virtual ~EngineProcTexture ();
   virtual bool Initialize ();
  virtual bool PrepareAnim (csSector* room);
  virtual void Animate (csTicks current_time, csTicks elapsed_time);
   virtual void Report (iGraphics2D *mg2d, 
		       const int fw, const int fh, int &offset);

};

#endif