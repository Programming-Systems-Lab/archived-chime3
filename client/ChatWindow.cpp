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
  //SetState(CSS_DISABLED, true);

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);

  //////////create the chat area box/////////////
  ChatArea *chat_area = new ChatArea (24, d, CSLBS_HSCROLL | CSLBS_VSCROLL, cslfsThickRect);
  chat_area->SetRect (5, 5 + TitlebarHeight, bound.Width() - 10, bound.Height() / 8 * 4);

  //////////create the chat line////////////////
  csInputLine *line = new csInputLine(d);
  line->SetRect (5, bound.Height() / 8 * 4 + py, bound.Width() - 10, bound.Height() / 8 * 4 + 3 * py);

  // Now create the notebook
  csNotebook *nb = new csNotebook (d, CSNBS_TABPOS_TOP| CSNBS_PAGEFRAME | CSNBS_PAGEINFO);
  nb->SetRect (0, bound.Height() / 8 * 4 + 3 * py + py, bound.Width() - 10, bound.Height() -10);

  //create the page that is associated with each entry
  csDialog *page = new csDialog (nb);
  page -> SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Brown_L);
  
  //make the first tab called "Local Users"
  nb->AddPrimaryTab (page, "~Local Users", "Users in Current Room");

   //add a listbox
  csListBox *page1_lb = new csListBox (page, CSLBS_VSCROLL, cslfsThinRect);
  page1_lb->SetSize (nb->bound.Width(), nb->bound.Height());
  
  
  //create the second page
  page = new csDialog (nb);
  nb->AddPrimaryTab (page, "~Global Users", "All Users of Chime");
  page->SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Blue_D);

   //add a listbox
  csListBox *page2_lb = new csListBox (page, CSLBS_VSCROLL, cslfsThinRect);
  page2_lb->SetSize (nb->bound.Width(), nb->bound.Height());
  
  (void*) new ChatAreaItem(chat_area, "Hello are you there this is a test. Maybe separating by chars wasn not the best idea", 0);
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
ChatArea::ChatArea(int chars_per_line, csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle) : csListBox(iParent, iStyle=CSLBS_DEFAULTVALUE, iFrameStyle=cslfsThickRect ) {
	SetCharsPerLine(chars_per_line);
	csListBox::csListBox(iParent, iStyle, iFrameStyle);
}

//add an item to the chat area
ChatAreaItem::ChatAreaItem(ChatArea *chat_area, const char *iText, int iID, csListBoxItemStyle iStyle) {//: csListBoxItem (chat_area, iText, iID=0, iStyle=cslisNormal) {
	int allowed_chars = chat_area->GetCharsPerLine();
	char *temp;
	char *here;

	temp = (char*) malloc(allowed_chars + 1);
	here = (char*) malloc(strlen(iText) + 1);

	//here points to the beginning of string
	strcpy(here, iText);

	//if there are elements to write
	while (here != NULL) {

		//if there are less chars to write than allowed then just write them
		if (strlen(here) < allowed_chars) {
			(void *) new csListBoxItem(chat_area, here, iID, iStyle);
			iID++;
			here = NULL; //that's it - enough
		}

		else {
			strncpy(temp, here, allowed_chars);
			(void *) new csListBoxItem(chat_area, temp, iID, iStyle);
			iID++;
			here += allowed_chars;
		}
	}

}