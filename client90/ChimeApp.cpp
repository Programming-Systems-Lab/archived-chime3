
#define SYSDEF_PATH
#include "cssysdef.h"
//#include "cssys/sysdriv.h"
#include "csws/csws.h"



#include "cssys/sysfunc.h"
#include "iutil/vfs.h"
#include "csutil/cscolor.h"
#include "cstool/csview.h"
#include "cstool/initapp.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/csinput.h"
#include "iutil/virtclk.h"
#include "iengine/sector.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/light.h"
#include "iengine/statlght.h"
#include "iengine/texture.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/material.h"
#include "imesh/thing/polygon.h"
#include "imesh/thing/thing.h"
#include "imesh/object.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivideo/txtmgr.h"
#include "ivideo/texture.h"
#include "ivideo/material.h"
#include "ivideo/fontserv.h"
#include "igraphic/imageio.h"
#include "imap/parser.h"
#include "ivaria/reporter.h"
#include "ivaria/stdrep.h"
#include "csutil/cmdhelp.h"

#include "csws/csws.h"
#include "cstool/initapp.h"
#include "csver.h"
#include "ivideo/fontserv.h"
#include "ivideo/graph2d.h"
#include "ivideo/natwin.h"
#include "iutil/cfgmgr.h"
#include "ivaria/reporter.h"
#include "iutil/cmdline.h"
#include "iutil/objreg.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"
#include "csutil/cmdhelp.h"
#include "csutil/util.h"

/*
#include "version.h"
#include "csver.h"
#include "csws/csskin.h"
#include "ivideo/fontserv.h"
#include <sys/stat.h>
#include "csgeom/csrect.h"
#include "cstool/initapp.h" 
#include "ivaria/reporter.h"
#include "igraphic/imageio.h"
#include "ivaria/reporter.h"
*/

#include "ChimeApp.h"
#include "ChimeSystemDriver.h"
#include "ChimeWorldView.h"
#include "ChimeSector.h"
#include "ChimeInfoEvent.h"


ChimeSystemDriver *Sys;
ChimeApp *app;

#define Gfx3D Sys->myG3D;
#define Gfx2D Sys->myG2D;

// Scroll bar class default palette
static int palette_CsfEdit[] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};

ChimeApp::ChimeApp (iObjectRegistry *registry, ChimeSystemDriver *System, csSkin &Skin) : csApp (registry, Skin)
{
  ChimeApp::System = System;
  int pal = csRegisterPalette (palette_CsfEdit, sizeof (palette_CsfEdit) / sizeof (int));
  SetPalette (pal);
}

ChimeApp::~ChimeApp ()
{
}



bool ChimeApp::Initialize (const char *iConfigName, InfoStorer *info)
{
  

  if (!csInitializer::SetupConfigManager (object_reg, iConfigName))
  {
		System->Report (CS_REPORTER_SEVERITY_ERROR, "Failed to init config!");
		return false;
  }


  if (!csApp::Initialize ())
    return false;

  ChimeApp::info = info;

  mainfont = LoadFont (CSFONT_LARGE);
  tinyfont = LoadFont (CSFONT_COURIER);
  SetFont(mainfont);

  // CSWS apps are a lot more performant with a single-buffered canvas
  GetG2D ()->DoubleBuffer (false);

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
  //(void) new StartStopWindow(this);

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
		  // AI2TV PLAYER SHUTDOWN
			if (Sys->myVideoPlayer) Sys->myVideoPlayer->ShutDown();
		  // END AI2TV PLAYER SHUTDOWN
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

void CreateSystem(void)
{
  // Create the system driver object
  Sys = new ChimeSystemDriver ();
}


void Cleanup ()
{
  csPrintf ("Cleaning up...\n");
  iObjectRegistry* object_reg = Sys->object_reg;
  delete Sys; Sys = NULL;
  csInitializer::DestroyApplication (object_reg);
}


// Define the skin for windowing system
CSWS_SKIN_DECLARE_DEFAULT (DefaultSkin);


int main (int argc, char* argv[])
{
  // Initialize the random number generator
  srand (time (NULL));
  InfoStorer info;

  iObjectRegistry *object_reg = csInitializer::CreateEnvironment();

  if (!object_reg)
	return false;

  extern void CreateSystem(void);
  CreateSystem();

  // Initialize the main system. This will load all needed plugins
  // (3D, 2D, network, sound, ..., engine) and initialize them.
  if (!Sys->Initialize (argc, argv, "/config/chime.cfg", &info, object_reg))
  {
    Sys->Report (CS_REPORTER_SEVERITY_ERROR, "Error initializing system!");
    exit (-1);
  }

  // Create our application object
  ChimeApp app(object_reg, Sys, DefaultSkin);

  if (app.Initialize ("/lib/csws/csws.cfg", &info)) {

	  //pass a reference of the app to the ChimeSystemDriver System
	  Sys->setCSApp(&app);

	  //pass a reference to the History window
	  Sys->SetHistoryWindow(app.historyWindow);
    
	  csDefaultRunLoop(object_reg);
  }

  //Cleanup ();

  return 1;
}

