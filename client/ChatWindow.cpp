#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"


// Scroll bar class default palette
ChatWindow::~ChatWindow() {}


ChatWindow::ChatWindow(csComponent *iParent)
  : csWindow(iParent, "-- Chat --", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  SetRect (0, app->bound.Height() / 3, app->bound.Width() / 4, app->bound.Height());
  SetState(CSS_DISABLED, true);

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);

  //////////create the list box/////////////
  ChatArea *lb = new ChatArea (d, CSLBS_HSCROLL | CSLBS_VSCROLL, cslfsThickRect);
  lb->SetRect (bound.Width() / 10, bound.Height() / 5 - TitlebarHeight, bound.Width() / 10 * 9, bound.Height() / 5 * 4);

  
}


bool ChatWindow::HandleEvent (iEvent &Event)
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

//the chat are used for chatting
ChatArea::ChatArea( csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle) : csListBox(iParent, iStyle=CSLBS_DEFAULTVALUE, iFrameStyle=cslfsThickRect ) {
	//to be implemented
	csListBox::csListBox(iParent, iStyle, iFrameStyle);
}

//add an item to the chat area
ChatAreaItem::ChatAreaItem( csComponent *iParent, const char *iText, int iID, csListBoxItemStyle iStyle) : csListBoxItem (iParent, iText, iID=0, iStyle=cslisNormal) {
	//to be implemented
	csListBoxItem::csListBoxItem(iParent, iText, iID, iStyle);
}