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

#define SYSDEF_ACCESS
#include <math.h>
#include "cssysdef.h"

#include "igraph2d.h"
#include "igraph3d.h"
#include "iimage.h"
#include "isystem.h"
#include "itxtmgr.h"
#include "ivfs.h"
#include "ievent.h"
#include "imetabal.h"
#include "istring.h"
#include "ifontsrv.h"
#include "idatabuf.h"
#include "icfgnew.h"
#include "imspr3d.h"

#include "csengine/texture.h"
#include "csengine/material.h"
#include "csengine/sector.h"
#include "csengine/engine.h"
#include "csengine/csview.h"
#include "csengine/camera.h"
#include "csengine/campos.h"
#include "csengine/light.h"

#include "csutil/util.h"
#include "csparser/csloader.h"
#include "csgfxldr/memimage.h"

#include "ChimeLabeling.h"
#include "ChimeSystemDriver.h"

iTextureHandle *ProcTexture::s_main_logo_tex = NULL;
iTextureManager *ProcTexture::s_proc_txtmgr = NULL;
iTextureHandle *ProcTexture::s_proc_logo_tex = NULL;
int ProcTexture::logo_w;
int ProcTexture::logo_h;
iSystem *ProcTexture::System = NULL;
iTextureManager *ProcTexture::MainTxtMgr = NULL;
csEngine *ProcTexture::Engine = NULL;

ProcTexture::ProcTexture ()
{
  Active = true;
  PTReady = false;
  ptmat = NULL;
  ptG3D = NULL;
  ptG2D = NULL;
  Name = NULL;
  TexFlags = 0;
  FileName = NULL;
  courierFont = NULL;
  key_colour = false;
}

ProcTexture::~ProcTexture ()
{
  delete [] Name;
  delete [] FileName;
  if (courierFont)
    courierFont->DecRef ();
  // DEC_REF (courierFont);
}

bool ProcTexture::Initialize ()
{
  iImage *proc_image;

  if (FileName)
  {
    proc_image = LoadImage (MainTxtMgr, FileName);
    proc_image->Rescale (mat_w, mat_h);
    TexFlags |= CS_TEXTURE_PROC_PERSISTENT;
  }
  else
    proc_image = (iImage*) new csImageMemory (mat_w, mat_h);
  
  csTextureWrapper *tex = Engine->GetTextures ()->NewTexture (proc_image);
  if (!tex) 
    return false;

  if (key_colour)
    tex->SetKeyColor (key_red, key_green, key_blue);

  tex->flags |= TexFlags;
  csMaterial *material = new csMaterial ();
  material->SetTextureWrapper (tex);
  ptmat = Engine->GetMaterials ()->NewMaterial (material);
  
  if (!ptmat) 
    return false;
  
  tex->SetName (Name);
  ptmat->SetName (Name);
  PTReady = true;
  
  return true;
}

void ProcTexture::LoadLogo ()
{
  if (logo_tex != 0)
  {
    iTextureManager *txtmgr = ptG3D->GetTextureManager ();
    iImage *logo = LoadImage (txtmgr, "/lib/std/cslogo.gif");
    logo_w = logo->GetWidth ();
    logo_h = logo->GetHeight ();
    logo_tex = txtmgr->RegisterTexture (logo, CS_TEXTURE_2D);
    logo_tex->SetKeyColor (true);
    logo_tex->SetKeyColor (0,0,255);
    logo_tex->Prepare ();
  }
}

bool ProcTexture::PrepareAnim (csSector* /*room*/)
{
  iTextureHandle* txt_handle = ptmat->GetMaterialHandle ()->GetTexture ();
  ptG3D = txt_handle->GetProcTextureInterface ();

  if (!ptG3D) return false;

  ptG2D = ptG3D->GetDriver2D ();

  iFontServer *fs = ptG2D->GetFontServer ();
  courierFont = fs->LoadFont (CSFONT_COURIER);

  // Is the texture manager dedicated to procedural textures?
  iTextureManager *txtmgr = ptG3D->GetTextureManager ();
  if (MainTxtMgr == txtmgr)
  {
    if (!s_main_logo_tex)
    {
      LoadLogo ();
      s_main_logo_tex = logo_tex;
    }
    else
      logo_tex = s_main_logo_tex;
  }
  else
  {
    // Dedicated procedural texture manager.
    if (!s_proc_logo_tex)
    {
      s_proc_txtmgr = txtmgr;
      LoadLogo ();
      s_proc_logo_tex = logo_tex;
    }
    else
      logo_tex = s_proc_logo_tex;
  }

  iTextureManager *tm = ptG3D->GetTextureManager ();
  write_colour = tm->FindRGB (255, 150, 100);
  clear_colour = tm->FindRGB (50, 75, 100);
  return true;
}

void ProcTexture::PrintToScreen (iGraphics3D *mainG3D, int &y, bool LHS)
{
  if (Active)
  {
    mainG3D->DrawPixmap (ptmat->GetMaterial()->GetTexture(), 
      LHS ? 0 : mainG3D->GetWidth() - mat_w, y, mat_w, mat_h, 0, 0, mat_w, mat_h);
    y += mat_h;
  }
}

void ProcTexture::Write(iGraphics2D *mg2d, int align,int x,int y,int fg,int bg,char *str,...)
{
  va_list arg;
  char b[256], *buf = b;

  va_start (arg,str);
  int l = vsprintf (buf, str, arg);
  va_end (arg);

  if (align != ALIGN_LEFT)
  {
    int rb = 0;

    if (align == ALIGN_CENTER)
    {
      int where;
      sscanf (buf, "%d%n", &rb,&where);
      buf += where + 1;
      l -= where + 1;
    }

    int w, h;
    courierFont->GetDimensions (buf, w, h);

    switch(align)
    {
      case ALIGN_RIGHT:  x -= w; break;
      case ALIGN_CENTER: x = (x + rb - w) / 2; break;
    }
  }

  mg2d->Write (courierFont, x, y, fg, bg, buf);
}

void ProcTexture::WriteShadow(iGraphics2D *mg2d, int align,int x,int y,int fg,char *str,...)
{
  char buf[256];

  va_list arg;
  va_start (arg, str);
  vsprintf (buf, str, arg);
  va_end (arg);

  Write (mg2d, align, x+1, y-1, 0, -1, buf);
  Write (mg2d, align, x, y, fg, -1, buf);
}


//----ripped from csloader------------------------------------------------
iImage* ProcTexture::LoadImage (iTextureManager *txtmgr, const char* name)
{
  iImage *ifile = NULL;
  iVFS *VFS = QUERY_PLUGIN (System, iVFS);
  if (!VFS) return NULL;
  
  iDataBuffer *buf = VFS->ReadFile (name);
  VFS->DecRef ();

  if (!buf || !buf->GetData ())
  {
    System->Printf (MSG_WARNING, "Cannot read image file \"%s\" from VFS\n", name);
    return NULL;
  }

  ifile = csImageLoader::Load 
    (buf->GetUint8(), buf->GetSize(), txtmgr->GetTextureFormat ());
  buf->DecRef ();

  if (!ifile)
  {
    System->Printf (MSG_WARNING, 
	    "'%s': Cannot load image. Unknown format or wrong extension!\n",name);
    return NULL;
  }

  iDataBuffer *xname =  VFS->ExpandPath (name);
  ifile->SetName ((const char*) xname->GetData ());
  xname->DecRef ();

  return ifile;
}

//--------------------------------------------------------------------------------
EngineProcTexture::EngineProcTexture () 
  : ProcTexture ()
{
  Name = strnew ("pt_engine");
  bot_view = NULL;
  TexFlags = CS_TEXTURE_3D | CS_TEXTURE_PROC | CS_TEXTURE_NOMIPMAPS;
  mat_w = mat_h = 125;
}

EngineProcTexture::~EngineProcTexture ()
{
  delete bot_view;

}

bool EngineProcTexture::Initialize ()
{
  if (!ProcTexture::Initialize ())
    return false;
  
  // Load the "standard" library
  if (!csLoader::LoadLibraryFile (Engine, "/lib/std/library"))
    {
      System->Printf (MSG_INITIALIZATION, "Unable to load standard library\n");
      return false;
    }
  
  return true;
}

bool EngineProcTexture::PrepareAnim (csSector* room)
{
    if (!ProcTexture::PrepareAnim (room))
      return false;

    bot_view = new csView (Engine, ptG3D);
	bot_view->SetSector(room);
    csCamera *bot_cam = bot_view->GetCamera ();
    bot_cam->SetPerspectiveCenter (ptG3D->GetWidth()/2,
				   ptG3D->GetHeight()/2);
    bot_cam->SetPosition (csVector3 (4, 0, 0));
    bot_cam->LookAt (csVector3(-1,0,0), csVector3(0,1,0));
    
    bot_view->SetRectangle (0, 0, ptG2D->GetWidth (),
			    ptG2D->GetHeight());

	int width = ptG2D->GetWidth();
	int height = ptG2D->GetHeight();
    bot_fov = 256;

    bot_cam->SetFOV (bot_fov, ptG3D->GetWidth ());

    angle = 0;	

  return true;
}

void EngineProcTexture::Animate (cs_time current_time, cs_time elapsed_time)
{
  (void) current_time;

    angle += elapsed_time * 0.4 / 1000.;
    while (angle >= 2.*3.1415926) angle -= 2.*3.1415926;
    float speed = (elapsed_time / 1000.) * (0.03 * 20);
	
    Engine->SetContext (ptG3D);

    if (ptG3D->BeginDraw (Engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS))
    {
      csCamera *bot_cam = bot_view->GetCamera ();
      bot_cam->SetFOV (bot_fov, ptG3D->GetWidth ());
      
	  bot_cam->Rotate (VEC_ROT_RIGHT, speed);
      bot_view->Draw ();
	

      if (ptG3D->BeginDraw (CSDRAW_2DGRAPHICS))
		ptG2D->Write (courierFont, 2, 120, write_colour, -1, 
		      manual_cam ? "manual_view" : "bot_view");
      
      ptG3D->FinishDraw ();
      ptG3D->Print (NULL);
    }
}

void EngineProcTexture::Report (iGraphics2D *mg2d, 
				const int fw, const int fh, int &offset)
{
  (void) fw;
    WriteShadow (mg2d, ALIGN_LEFT, 10, fh-28, write_colour, "bot_fov blah lah lkjhd : %d", 5);
    offset += 10;
}

