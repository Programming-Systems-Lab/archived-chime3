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
#include "chimemenu.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <sys/stat.h>
#include "csutil/csrect.h"

#include "chimeBrowser.h"


//-- CsfEdit ---------------------------------------------------------------

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

}

ChimeMenu::~ChimeMenu ()
{
  
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
  printf (MSG_INITIALIZATION, "ChimeMenu.\n\n");



  csKeyboardAccelerator *ka = new csKeyboardAccelerator (this);
  csMenu *menu = new csMenu (this, csmfsBar, 0);
  menu->id = CSWID_MENUBAR;
  menu->SetFont(mainfont);
  csMenu *submenu = new csMenu (NULL);



  ///////////////////////////////////////////MAIN MENU/////////////////////////////
  (void)new csMenuItem (menu, "~Connection", submenu);

	//popup the connection dialog box
    (void)new csMenuItem (submenu, "~New Connection\tCtrl+N", 66600);





  ////////////////////////////////SUBMENU of FIRST COLUMN//////////////////////////



  (void)new csMenuItem(submenu);

    //get the object menu

	(void)new csMenuItem (submenu, "~Get Object\tCtrl+G", 66601);

    ka->Command ('g', CSMASK_CTRL, 66601);



	//popup the VEM console

	(void)new csMenuItem (submenu, "~VEM Console\tCtrl+C", 66602);

	ka->Command ('v', CSMASK_CTRL, 66602);
		

  

  ////////////////////////////////LAST SUBMENU OF FIRST COLUMN////////////////////////

	(void)new csMenuItem(submenu);

		csMenuItem *mi = new csMenuItem (submenu, "~Quit\tCtrl+Q", 66698);
		HintAdd ("Choose this menu item to quit the program", mi);
		ka->Command ('q', CSMASK_CTRL, 66698);


  /////////////////////////////CREATE SECOND COLUMN/////////////////////////////////
  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Settings", submenu);



	//some crystal space specific setting if needed
    (void)new csMenuItem (submenu, "C~rystal Space Settings\tCtrl+R", 66700);

	 ka->Command ('r', CSMASK_CTRL, 66700);



	 //some siena settings

	(void)new csMenuItem (submenu, "~Siena Settings\tCtrl+S", 66701);

	ka->Command ('s', CSMASK_CTRL, 66701);


	

  int fh; menu->GetTextSize("", &fh);
  menu->SetRect (0, 0, bound.xmax, fh + 8);


   //popup the connection dialog box

  (void)new ConnectDialog(this);


  return true;
}



bool ChimeMenu::HandleEvent (iEvent &Event)
{
  if (csApp::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {

	  case csevKeyDown:

		  if(Event.Key.Code == CSKEY_ESC)  
		  {
			  ((chimeBrowser*)System)->Stop3D();
			  Invalidate(true);				
			  return true;
		  }

		  if(Event.Key.Code == 's') 
		  {
			  ((chimeBrowser*)System)->Start3D();
			  return true;
		  }

	  case csevCommand:
		  switch (Event.Command.Code)			  
		  {
			  
			  //a new connection has been opened    
			  
		  case 66600:
			  
			  {
				  
				  (void)new ConnectDialog(this);
				  
				  return true;
				  
			  }
			  
		  
			  //get a Certain Object dialog is opened
			  
		  case 66601:
			  
			  {
				  
				  (void)new ObjectToRetrieveDialog(this);
				  
				  return true;
				  
			  }			  
			  
			  //a VEM console is requested
			  
		  case 66602:
			  
			  {
				  
				  (void)new VEM(this);
				  
				  return true;
				  
			  }





		//Siena settings need to be updated

        case 66701:

        {

		(void)new SienaDialog(this);

          return true;

        }


        
        case 66698:
        {
		  ShutDown();
          return true;
        }


        //show the list of all active windows

		case 66699:

		{
          WindowList ();
          return true;
		 }

	  }
      break;
  }
	   /* endswitch */

  return false;
}

void ChimeMenu::Refresh3D()
{
	((chimeBrowser*)System)->Refresh3D();
}

void ChimeMenu::Draw ()
{	
	csApp::Draw();
	Refresh3D();
	Invalidate(true);
}

// Define the skin for windowing system
SKIN_DECLARE_DEFAULT (DefaultSkin);

/*
 * Main function
 */
int main (int argc, char* argv[])
{
  chimeBrowser System;

  if (!System.Initialize (argc, argv, "/config/chime.cfg"))
    return -1;

  //if (!System.Open ("CHIME (Columbia Hypermedia Immersion Environment)"))
   // return -1;

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
