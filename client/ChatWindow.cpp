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
  : AlwaysVisibleWindow(iParent, "-- Chat --", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  SetRect (0, app->bound.Height() / 3, app->bound.Width() / 4, app->bound.Height());

  int px = 15, py = 20;
  int labelw = 150;

  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);

  //////////create the chat area box/////////////
  chat_area = new ChatArea (35, d, CSLBS_HSCROLL | CSLBS_VSCROLL, cslfsThickRect);
  chat_area->SetRect (5, 5 + TitlebarHeight, bound.Width() - 10, bound.Height() / 8 * 4);

  //////////create the chat line////////////////
  user_msg_line = new csInputLine(d);
  user_msg_line->SetRect (5, bound.Height() / 8 * 4 + py, bound.Width() - 10, bound.Height() / 8 * 4 + 3 * py);

  // Now create the notebook
  csNotebook *nb = new csNotebook (d, CSNBS_TABPOS_TOP| CSNBS_PAGEFRAME | CSNBS_PAGEINFO);
  nb->SetRect (0, bound.Height() / 8 * 4 + 3 * py + py, bound.Width() - 10, bound.Height() -10);

  //create the page that is associated with each entry
  csDialog *page = new csDialog (nb);
  page -> SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Brown_L);
  
  //make the first tab called "Local Users"
  nb->AddPrimaryTab (page, "~Local Users", "Users in Current Room");

   //add a listbox
  local_users_lb = new csListBox (page, CSLBS_VSCROLL | CSLBS_MULTIPLESEL, cslfsThinRect);
  local_users_lb->SetSize (nb->bound.Width(), nb->bound.Height());
   
  //create the second page
  page = new csDialog (nb);
  nb->AddPrimaryTab (page, "~Global Users", "All Users of Chime");
  page->SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Blue_D);

   //add a listbox
  global_users_lb = new csListBox (page, CSLBS_VSCROLL | CSLBS_MULTIPLESEL, cslfsThinRect);
  global_users_lb->SetSize (nb->bound.Width(), nb->bound.Height());

  last_ID = 0;
  (void*) new ChatAreaItem(chat_area, "", last_ID);
  //(void*) new ChatAreaItem(chat_area, "Hello are you there this is a test. Maybe separating by chars wasn not the best idea", last_ID);

}


bool ChatWindow::HandleEvent (iEvent &Event)
{

 switch (Event.Type)
  {
	case csevKeyDown: 
		if(Event.Key.Code == CSKEY_ENTER) {
			SubmitMessage(user_msg_line->GetText());
			user_msg_line->SetText("");
			return true;
		}

      break;
  }

  if (AlwaysVisibleWindow::HandleEvent (Event))
    return true;

  return false;
}

//submit the message to the chat area box
void ChatWindow::SubmitMessage(const char* msg) {
	new ChatAreaItem(chat_area, msg, last_ID);
}
	

//add local users to the chat window
void ChatWindow::AddLocalUsers(csStrVector *user_list) {
	for (int i = 0; i < user_list->Length(); i++) {
		AddLocalUser(user_list->Get(i));
	}
}


//add a user to the local list
void ChatWindow::AddLocalUser(char *userID) {
	(void) new csListBoxItem (local_users_lb, userID, 0, cslisEmphasized);
	AddGlobalUser(userID);
}

//add global users to the chat window
void ChatWindow::AddGlobalUsers(csStrVector *user_list) {
	for (int i = 0; i < user_list->Length(); i++) {
		AddGlobalUser(user_list->Get(i));
	}
}

//add a user to the global list
void ChatWindow::AddGlobalUser(char *userID) {
	(void) new csListBoxItem (global_users_lb, userID, 0, cslisEmphasized);
}

//delete a local user
void ChatWindow::DeleteLocalUser(char *userID) {
	csListBoxItem* to_delete = (csListBoxItem*) local_users_lb -> ForEachItem(DeleteMe, userID, true);
	local_users_lb->Delete(to_delete);
}

//delete a global user
void ChatWindow::DeleteGlobalUser(char *userID) {
	csListBoxItem* to_delete = (csListBoxItem*) global_users_lb -> ForEachItem(DeleteMe, userID, true);
	global_users_lb->Delete(to_delete);
}


//to delete or not to delete
bool ChatWindow::DeleteMe(csComponent *item, void *userID) {
	if (strcmp(item->GetText(), (const char*) userID) == 0)
		return true;
	else
		return false;
}

//the chat area used for chatting
ChatArea::ChatArea(int chars_per_line, csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle) : csListBox(iParent, iStyle=CSLBS_DEFAULTVALUE, iFrameStyle=cslfsThickRect ) {
	SetCharsPerLine(chars_per_line);
	SetState(CSS_SELECTABLE, false);
	csListBox::csListBox(iParent, iStyle, iFrameStyle);
}

//add an item to the chat area
ChatAreaItem::ChatAreaItem(ChatArea *chat_area, const char *iText, int &iID, csListBoxItemStyle iStyle) : csComponent (chat_area) {
	int allowed_chars = chat_area->GetCharsPerLine();
	int earliest_break;
	int latest_break;
	char *temp;
	char *here;

	temp = new char[allowed_chars + 1];
	here = new char[strlen(iText) + 1];

	//here points to the beginning of string
	strcpy(here, iText);

	//if there are elements to write
	while (here != NULL) {

		//if there are less chars to write than allowed then just write them
		if (strlen(here) < allowed_chars) {
			csListBoxItem *item = new csListBoxItem(chat_area, here, iID, iStyle);
			item->SetState(CSS_LISTBOXITEM_SELECTED, false);
			iID++;
			here = NULL; //that's it - enough
		}

		else {
			//if you can find a space then do it
			if (FindSpace(here, allowed_chars, &earliest_break, &latest_break)) {
				strncpy(temp, here, latest_break);
				here += latest_break;
				here++; //just eat up the space
			}

			else {
				strncpy(temp, here, allowed_chars);
				here += allowed_chars;
			}

			csListBoxItem *item = new csListBoxItem(chat_area, temp, iID, iStyle);
			item->SetState(CSS_LISTBOXITEM_SELECTED, false);
			iID++;
		
		}
	}

}

ChatAreaItem::FindSpace(const char* str, int max_chars, int *earliest_break, int *latest_break) {
	int i;

	//look through up until max_chars with 1/10 max_chars as leeway
	for (i = 0; i < max_chars; i++) {
		if (str[i] == ' ') {
			*earliest_break = *latest_break;
			*latest_break = i;
		}
	}

	if (*earliest_break != 0 || *latest_break != 0)
		return true;

	else return false;
}






