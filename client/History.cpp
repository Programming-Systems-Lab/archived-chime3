#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "chimemenu.h"


// Scroll bar class default palette
HistoryWindow::~HistoryWindow() {}


HistoryWindow::HistoryWindow(csComponent *iParent)
  : csWindow(iParent, "-- History --", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  InitCount();
  

  SetRect (0, 15, app->bound.Width() / 4, app->bound.Height() / 3);
  SetState(CSS_DISABLED, true);

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);
  
  //////////create the list box/////////////
  lb = new csListBox (d, CSLBS_HSCROLL | CSLBS_VSCROLL, cslfsThinRect);
  lb->SetRect (bound.Width() / 10, bound.Height() / 5 - TitlebarHeight, bound.Width() / 10 * 9, bound.Height() / 5 * 4);

  AddItem("http://www.yahoo.com");
}

bool HistoryWindow::AddItem(char *item) {
	new csListBoxItem (lb, item, ItemCount());
	AddItem();
	return true;
}


bool HistoryWindow::HandleEvent (iEvent &Event)
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
