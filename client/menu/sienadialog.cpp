#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "chimemenu.h"



// Scroll bar class default palette
static int palette_VEM[] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};

SienaDialog::~SienaDialog() {}


SienaDialog::SienaDialog(csComponent *iParent)
  : csWindow(iParent, "Siena Settings", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  this->SetSize (500, 250);
  this->Center ();

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);


   //////////////////////Siena Location field//////////////////////////////
  csInputLine *siena_location = new csInputLine(d);
  siena_location->SetSize(300,30);
  siena_location->SetPos(px+labelw,py);

  //set the text to the previous value
  //not implemented yet
  //username->SetText(font->GetFontName());

  csStatic *siena_lbl = new csStatic(d, siena_location, "Siena SENP:");
  siena_lbl->SetRect(px, py, px+labelw,py+siena_location->bound.Height());
  py += siena_location->bound.Height();


  /////////////////////Setup the Chat Host//////////////////////////////////
  csInputLine *chat_host = new csInputLine(d);
  chat_host->SetSize(300,30);
  chat_host->SetPos(px+labelw,py);

  //set the text to the previous value
  //not implemented yet
  //username->SetText(font->GetFontName());

  csStatic *chat_lbl = new csStatic(d, chat_host, "Chat host:");
  chat_lbl->SetRect(px, py, px+labelw,py+siena_location->bound.Height());
  py += chat_lbl->bound.Height();


  /////////////////////Setup the port to use for Chat////////////////////////////////
  csInputLine *chat_port = new csInputLine(d);
  chat_port->SetSize(50,30);
  chat_port->SetPos(px+labelw,py);

  //set the text to the previous value
  //not implemented yet
  //password->SetText(font->GetFontName());

  csStatic *chat_port_lbl = new csStatic(d, chat_port, "Chat Port:");
  chat_port_lbl->SetRect(px, py, px+labelw,py+chat_port->bound.Height());
  py += chat_port->bound.Height();



  //setup the accept and cancel buttons
  csButton *butOK = new csButton(d, 66800);
  butOK->SetText("Save Settings");
  butOK->SetSuggestedSize(16,8);
  butOK->SetPos(70, 175);

  //cancel button
  csButton *butCAN = new csButton(d, 66801);
  butCAN->SetText("Cancel");
  butCAN->SetSuggestedSize(16,8);
  butCAN->SetPos(300, 175);


}



bool SienaDialog::HandleEvent (iEvent &Event)
{

  if (csWindow::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {
    case csevCommand:
      switch (Event.Command.Code)
      {
	    //OK button was pressed
        case 66800:
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
