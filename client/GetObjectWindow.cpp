#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"


// Scroll bar class default palette
GetObjectWindow::~GetObjectWindow() {}


GetObjectWindow::GetObjectWindow(csComponent *iParent)
  : csWindow(iParent, "Retrieve Object", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  this->SetSize (500, 250);
  this->Center ();

  int px = 15, py = 20;
  int labelw = 150;


  //////////create the dialog///////////////

  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);


   //////////////////////Object to Retrieve field//////////////////////////////

  object = new csInputLine(d);
  object->SetSize(300,30);
  object->SetPos(px+labelw,py);


  csStatic *obj_lbl = new csStatic(d, object, "~Object to Retrieve:");
  obj_lbl->SetRect(px, py, px+labelw,py+object->bound.Height());
  py += object->bound.Height();


  /////////////////////Protocol to Use Field////////////////////////////////

  protocol = new csInputLine(d);
  protocol->SetSize(300,30);
  protocol->SetPos(px+labelw,py);


  csStatic *prot_lbl = new csStatic(d, protocol, "~Protocol To Use:");
  prot_lbl->SetRect(px, py, px+labelw,py+protocol->bound.Height());
  py += protocol->bound.Height();


  //setup the accept and cancel buttons

  csButton *butOK = new csButton(d, 66800);
  butOK->SetText("Retrieve");
  butOK->SetSuggestedSize(16,8);
  butOK->SetPos(70, 175);


  //cancel button

  csButton *butCAN = new csButton(d, 66801);
  butCAN->SetText("Cancel");
  butCAN->SetSuggestedSize(16,8);
  butCAN->SetPos(300, 175);

}


bool GetObjectWindow::HandleEvent (iEvent &Event)
{
  if (csWindow::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {
    case csevCommand:
      switch (Event.Command.Code)
      {

	    //Retrieve button was pressed
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
