
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
#include "ChimeInfoEvent.h"


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
    (void)new csMenuItem (submenu, "~New Connection\tCtrl+N", CONNECT_WINDOW);

  ////////////////////////////////SUBMENU of FIRST COLUMN//////////////////////////
  (void)new csMenuItem(submenu);

    //get the object menu
	(void)new csMenuItem (submenu, "~Get Object\tCtrl+G", GET_OBJECT_WINDOW);
    ka->Command ('g', CSMASK_CTRL, GET_OBJECT_WINDOW);

	//the VEM console
	(void)new csMenuItem (submenu, "~VEM Console\tCtrl+C", VEM_WINDOW);
	ka->Command ('v', CSMASK_CTRL, VEM_WINDOW);

  ////////////////////////////////LAST SUBMENU OF FIRST COLUMN////////////////////////

	(void)new csMenuItem(submenu);

		csMenuItem *mi = new csMenuItem (submenu, "~Quit\tCtrl+Q", QUIT_CHIME);
		HintAdd ("Choose this menu item to quit the program", mi);
		ka->Command ('q', CSMASK_CTRL, QUIT_CHIME);

  /////////////////////////////CREATE SECOND COLUMN/////////////////////////////////
  submenu = new csMenu (NULL);
  (void)new csMenuItem (menu, "~Settings", submenu);

	//some crystal space specific setting if needed
    (void)new csMenuItem (submenu, "C~rystal Space Settings\tCtrl+R", CRYSTAL_SPACE_WINDOW);
	 ka->Command ('r', CSMASK_CTRL, CRYSTAL_SPACE_WINDOW);

	 //some siena settings
	(void)new csMenuItem (submenu, "~Siena Settings\tCtrl+S", SIENA_WINDOW);
	ka->Command ('s', CSMASK_CTRL, SIENA_WINDOW);


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


//start 3D rendering
void ChimeApp::Start3D() {
	((ChimeSystemDriver*)System)->Start3D();
}

//stop 3D rendering
void ChimeApp::Stop3D() {
	((ChimeSystemDriver*)System)->Stop3D();
}

//this handles all events
bool ChimeApp::HandleEvent (iEvent &Event)
{
 
  //handle an event from the application
  if (csApp::HandleEvent (Event))
    return true;

  //see if our system can take care of this...
  if (Event.Type == csevCommand && Event.Command.Code > 88800 && ((ChimeSystemDriver*)System)->HandleMenuEvent(Event))
  	  return true;

  
  switch (Event.Type)
  {
	  case csevKeyDown:

		  if(Event.Key.Code == CSKEY_ESC)
		  {
			  Stop3D();
			  Invalidate(true);
			  return true;
		  }

		  if(Event.Key.Code == 'w')
		  {
			  Start3D();
			  Invalidate(true);
			  return true;
		  }

	  case csevCommand:
		  switch (Event.Command.Code)
		  {

		  case CHIME_EVENT:
			  {
				 int i = 1;
				 return ((ChimeSystemDriver*)System)->HandleEventFromOtherWindow(Event);
			  }
		  
		  //a new connection has been opened
		  case CONNECT_WINDOW:
			  {
				  (void)new ConnectWindow(this);
				  return true;
			  }

		  //get a Certain Object dialog is opened
		  case GET_OBJECT_WINDOW:
			  {
				  (void)new GetObjectWindow(this);
				  return true;
			  }

			  //a VEM console is requested
		  case VEM_WINDOW:
			  {
				  (void)new VEMWindow(this);
				  return true;
			  }

		//Siena settings need to be updated
        case SIENA_WINDOW:
        {
			(void)new SienaWindow(this);
			return true;
        }

        case QUIT_CHIME:
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
