#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"
#include "InfoStorer.h"
#include "ChimeApp.h"

// Scroll bar class default palette
ConnectWindow::~ConnectWindow() {}


ConnectWindow::ConnectWindow(csComponent *iParent)
  : ChimeWindow(iParent, "Connection Box", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  this->SetSize (500, 250);
  this->Center ();

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////

  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);

   //////////////////////Username field//////////////////////////////

  username = new csInputLine(d);
  username->SetSize(300,30);
  username->SetPos(px+labelw,py);


  //set the text to the previous value
  //not implemented yet
  //username->SetText(font->GetFontName());


  csStatic *usr_lbl = new csStatic(d, username, "~Username:");
  usr_lbl->SetRect(px, py, px+labelw,py+username->bound.Height());
  py += username->bound.Height();


  /////////////////////Password Field////////////////////////////////

  password = new csInputLine(d);
  password->SetSize(300,30);
  password->SetPos(px+labelw,py);



  //set the text to the previous value
  //not implemented yet
  //password->SetText(font->GetFontName());

  csStatic *pass_lbl = new csStatic(d, password, "~Password:");
  pass_lbl->SetRect(px, py, px+labelw,py+password->bound.Height());
  py += password->bound.Height();



  /////////////////////Chime Server////////////////////////////////
  chime_server = new csInputLine(d);
  chime_server->SetSize(300,30);
  chime_server->SetPos(px+labelw,py);

  //set the text to the previous value
  //not implemented yet
  //password->SetText(font->GetFontName());

  csStatic *serv_lbl = new csStatic(d, chime_server, "~Chime Server Name:");
  serv_lbl->SetRect(px, py, px+labelw,py+chime_server->bound.Height());
  py += chime_server->bound.Height();


  //setup the accept and cancel buttons
  csButton *butOK = new csButton(d, 66800);
  butOK->SetText("Login");
  butOK->SetSuggestedSize(16,8);
  butOK->SetPos(70, 175);


  //cancel button
  csButton *butCAN = new csButton(d, 66801);
  butCAN->SetText("Cancel");
  butCAN->SetSuggestedSize(16,8);
  butCAN->SetPos(300, 175);

}


bool ConnectWindow::HandleEvent (iEvent &Event)
{
  
  InfoStorer *info = NULL;

  if (csWindow::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {

    case csevCommand:
      switch (Event.Command.Code)
      {
	    //OK button was pressed
        case 66800:

			info = ((ChimeApp*) app)->GetInfo();

			if (info == NULL) {
				Close();
				return false;
			
			} else {
				info->SetUsername(username->GetText());
				info->SetPassword(password->GetText());
				info->SetSienaLocation(chime_server->GetText());
			}
			Close();
				
			return true;


		 //Cancel Button has been pressed
		case 66801:
		  Close();
		  return true;
	  }

      break;
  }

  return false;
}
