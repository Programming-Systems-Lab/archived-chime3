/*
    Copyright (C) 1999 by Andrew Zabolotny <bit@eltech.ru>
    Copyright (C) 2001 by W.C.A. Wijngaards

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

#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "csfedit.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <sys/stat.h>
#include "csutil/csrect.h"



//-- ChimeMenu ---------------------------------------------------------------

// Scroll bar class default palette
static int palette_CsfEdit[] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};

ChimeMenu::ChimeMenu (iSystem *System, csSkin &Skin) : csApp (System, Skin)
{
  int pal = csRegisterPalette (palette_CsfEdit, sizeof (palette_CsfEdit) / sizeof (int));
  SetPalette (pal);
  mainfont = NULL;
  tinyfont = NULL;
  editfont = NULL;
  saveitem = NULL;
  closeitem = NULL;
}

ChimeMenu::~ChimeMenu ()
{
  /// first delete the editfont - might save itself.
  if(editfont) delete editfont;

  if (mainfont) mainfont->DecRef ();
  if (tinyfont) tinyfont->DecRef ();
}

bool ChimeMenu::Initialize (const char *iConfigName)
{
  if (!csApp::Initialize (iConfigName))
    return false;

  mainfont = LoadFont (CSFONT_LARGE);
  tinyfont = LoadFont (CSFONT_COURIER);
  SetFont(mainfont);

  // CSWS apps are a lot more performant with a single-buffered canvas
  GetG2D ()->DoubleBuffer (false);

  printf (MSG_INITIALIZATION, "Crystal Space version %s (%s).\n", CS_VERSION, CS_RELEASE_DATE);
  printf (MSG_INITIALIZATION, "CSF Editor.\n\n");

  csKeyboardAccelerator *ka = new csKeyboardAccelerator (this);
  csMenu *menu = new csMenu (this, csmfsBar, 0);
  menu->id = CSWID_MENUBAR;
  menu->SetFont(mainfont);
  csMenu *submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Connection", submenu);
    (void)new csMenuItem (submenu, "~New Connection\tCtrl+N", 66600);
	(void)new csMenuItem (submenu, "~VEM Console\tCtrl+C", 66601);

    (void)new csMenuItem (submenu);
    csMenuItem *mi = new csMenuItem (submenu, "~Quit\tCtrl+Q", 66698);
    HintAdd ("Choose this menu item to quit the program", mi);
    ka->Command ('n', CSMASK_CTRL, 66600);
    ka->Command ('q', CSMASK_CTRL, 66698);

  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Settings", submenu);
    (void)new csMenuItem (submenu, "~Crystal Space Settings", 66700);
	(void)new csMenuItem (submenu, "~Siena Settings", 66701);


  int fh; menu->GetTextSize("", &fh);
  menu->SetRect (0, 0, bound.xmax, fh + 8);

  return true;
}


void ChimeMenu::SetEditFont(csEditFont *f)
{
  editfont = f;
  if(saveitem!=NULL)
    saveitem->SetState(CSS_SELECTABLE, editfont!=0);
  if(closeitem!=NULL)
    closeitem->SetState(CSS_SELECTABLE, editfont!=0);
}


bool ChimeMenu::HandleEvent (iEvent &Event)
{
  if (csApp::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {
    case csevCommand:
      switch (Event.Command.Code)
      {


		//a new connection has been opened
        case 66600:
        {
	  if(editfont){ delete editfont; editfont = 0; }
	  SetEditFont(new csEditFont(this));
	  (void)new ConnectDialog(this, editfont);
          return true;
        }


		//a VEM console is requested
		case 66601:
        {
	  if(editfont){ delete editfont; editfont = 0; }
	  SetEditFont(new csEditFont(this));
	  (void)new VEM(this, editfont);
          return true;
        }
		 {


		//Siena settings need to be updated
        case 66701:
        {
		if(editfont){ delete editfont; editfont = 0; }
		SetEditFont(new csEditFont(this));
		(void)new SienaDialog(this, editfont);
          return true;
        }


        case 66698:
        {
		  ShutDown();
          return true;
        }

        //show the list of all active windows
		case 66699:
          WindowList ();
          return true;
		 }
	  }
      break;
  }
	   /* endswitch */

  return false;
}

// Define the skin for windowing system
SKIN_DECLARE_DEFAULT (DefaultSkin);

/*
 * Main function
 */
int main (int argc, char* argv[])
{
  SysSystemDriver System;

  if (!System.Initialize (argc, argv, "/config/cswstest.cfg"))
    return -1;

  if (!System.Open ("CHIME (Columbia Hypermedia Immersion Environment)"))
    return -1;

  // Look for skin variant from config file
  DefaultSkin.Prefix = System.GetOptionCL ("skin");
  if (!DefaultSkin.Prefix)
    DefaultSkin.Prefix = System.GetConfig ()->GetStr ("CSWS.Skin.Variant", NULL);

  // Create our application object
  ChimeMenu app (&System, DefaultSkin);

  if (app.Initialize ("/lib/csws/csws.cfg"))
    System.Loop ();

  return 0;
}
