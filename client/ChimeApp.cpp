
#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "ChimeApp.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <sys/stat.h>
#include "csutil/csrect.h"

#include "ChimeSystemDriver.h"
#include "ChimeWorldView.h"
#include "ChimeSector.h"

// Scroll bar class default palette
static int palette_CsfEdit[] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};

ChimeApp::ChimeApp (iSystem *System, csSkin &Skin) : csApp (System, Skin)
{
  int pal = csRegisterPalette (palette_CsfEdit, sizeof (palette_CsfEdit) / sizeof (int));
  SetPalette (pal);

}

ChimeApp::~ChimeApp ()
{

}

bool ChimeApp::Initialize (const char *iConfigName, InfoStorer *info)
{
  
  ChimeApp::info = info;

  if (!csApp::Initialize (iConfigName))
    return false;

  mainfont = LoadFont (CSFONT_LARGE);
  tinyfont = LoadFont (CSFONT_COURIER);
  SetFont(mainfont);

  // CSWS apps are a lot more performant with a single-buffered canvas
  GetG2D ()->DoubleBuffer (false);

  printf (MSG_INITIALIZATION, "Crystal Space version %s (%s).\n", CS_VERSION, CS_RELEASE_DATE);
  printf (MSG_INITIALIZATION, "ChimeApp.\n\n");

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

	//the VEM console
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

  
  csWindow *w = new csWindow (this, "3D View", CSWS_DEFAULTVALUE & ~(CSWS_BUTCLOSE | CSWS_MENUBAR));
  engine_views.Push (new ChimeWorldView (w, (ChimeSystemDriver*)System));
  w->SetRect (bound.Width () / 4, 0, bound.Width (), bound.Height () / 4 * 3);
  w->Hide();

  //put in a chat window
  chatWindow = new ChatWindow(this);
  chatWindow->AddLocalUsers(((ChimeSystemDriver*)System)->GetCurChimeSector()->GetUserList());

  //put in a history window
  historyWindow = new HistoryWindow(this);

  //put in a VEM window
  (void) new VEMWindow(this);

  //put in the Start/Stop window
  (void) new StartStopWindow(this);

  //popup the connection dialog box
  (void) new ConnectWindow(this);

  
  return true;
}



bool ChimeApp::HandleEvent (iEvent &Event)
{
  //handle an event from the application
  if (csApp::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {
	  case csevKeyDown:

		  if(Event.Key.Code == CSKEY_ESC)
		  {
			  ((ChimeSystemDriver*)System)->Stop3D();
			  Invalidate(true);
			  return true;
		  }

		  if(Event.Key.Code == 'w')
		  {
			  ((ChimeSystemDriver*)System)->Start3D();
			  return true;
		  }

	  case csevCommand:
		  switch (Event.Command.Code)
		  {

			  //a new connection has been opened
		  case 66600:
			  {
				  (void)new ConnectWindow(this);
				  return true;
			  }

			  //get a Certain Object dialog is opened

		  case 66601:
			  {
				  (void)new GetObjectWindow(this);
				  return true;
			  }

			  //a VEM console is requested
		  case 66602:
			  {
				  (void)new VEMWindow(this);
				  return true;
			  }

		//Siena settings need to be updated
        case 66701:
        {
			(void)new SienaWindow(this);
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

void ChimeApp::Refresh3D()
{
	((ChimeSystemDriver*)System)->Refresh3D();
}

void ChimeApp::Draw ()
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
  ChimeSystemDriver System;
  InfoStorer info;

  if (!System.Initialize (argc, argv, "/config/chime.cfg", &info))
    return -1;

  /*if (!System.Open ("CHIME (Columbia Hypermedia Immersion Environment)"))
    return -1;
	*/

  // Look for skin variant from config file
  DefaultSkin.Prefix = System.GetOptionCL ("skin");
  if (!DefaultSkin.Prefix)
    DefaultSkin.Prefix = System.GetConfig ()->GetStr ("CSWS.Skin.Variant", NULL);

  // Create our application object
  ChimeApp app (&System, DefaultSkin);

  if (app.Initialize ("/lib/csws/csws.cfg", &info))

	  //pass a reference of the app to the ChimeSystemDriver System
	  System.setCSApp(&app);

	  //pass a reference to the History window
	  System.SetHistoryWindow(app.historyWindow);

	  System.Loop ();

  return 0;
}
