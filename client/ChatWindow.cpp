#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"
#include "ChimeApp.h"
#include "comm_client/ClientComm.h"

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
  nb = new csNotebook (d, CSNBS_TABPOS_TOP| CSNBS_PAGEFRAME | CSNBS_PAGEINFO);
  nb->SetRect (0, bound.Height() / 8 * 4 + 3 * py + py, bound.Width() - 10, bound.Height() -10);

  //create the page that is associated with each entry
  page1 = new csDialog (nb);
  page1 -> SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Brown_L);
  
  //make the first tab called "Local Users"
  nb->AddPrimaryTab (page1, "~Local Users", "Users in Current Room");

   //add a listbox
  local_users_lb = new UserListBox (page1, CSLBS_VSCROLL | CSLBS_MULTIPLESEL, cslfsThinRect);
  local_users_lb->SetSize (nb->bound.Width(), nb->bound.Height());
   
  //create the second page
  page2 = new csDialog (nb);
  nb->AddPrimaryTab (page2, "~Global Users", "All Users of Chime");
  page2->SetColor (CSPAL_DIALOG_BACKGROUND, cs_Color_Blue_D);

   //add a listbox
  global_users_lb = new UserListBox (page2, CSLBS_VSCROLL | CSLBS_MULTIPLESEL, cslfsThinRect);
  global_users_lb->SetSize (nb->bound.Width(), nb->bound.Height());

  last_ID = 0;
  (void*) new ChatAreaItem(chat_area, "", last_ID);

}

//**********************************************************************
//*
//* Handle All Events in the Chat Window
//*
//**********************************************************************
bool ChatWindow::HandleEvent (iEvent &Event)
{

 switch (Event.Type)
  {
	case csevKeyDown: 
		if(Event.Key.Code == CSKEY_ENTER) {
			//send message to everyone selected in the list of local users
			local_users_lb -> ForEachItem(SendMsg, (void*) this, true);

			//send message to everyone selected in the list of global users
			global_users_lb -> ForEachItem(SendMsg, (void*) this, true);

			//Show a copy of the message on the sender's screen - AOL IM style
			ShowMessage(user_msg_line->GetText());

			//clear the line
			user_msg_line->SetText("");

			return true;
		}

      break;
  }

  if (AlwaysVisibleWindow::HandleEvent (Event))
    return true;

  return false;
}

//**********************************************************************
//*
//* to send a message or not
//*
//**********************************************************************
bool ChatWindow::SendMsg(csComponent *item, void *chat_window_class) {
	if (item->GetState(CSS_LISTBOXITEM_SELECTED))
		((ChatWindow*) chat_window_class)->SendMessage(((UserListBoxItem*) item)->GetIPAddress(), ((ChatWindow*) chat_window_class)->user_msg_line->GetText());
	return false;
}

//**********************************************************************
//*
//* send a message to someone
//*
//**********************************************************************
void ChatWindow::SendMessage(const char *ip_address, const char *msg) {
	char temp[1000];
	sprintf(temp, "%s %s %s",((ChimeApp*) app)->GetInfo()->GetLocalIP(), ((ChimeApp*) app)->GetInfo()->GetUsername(), msg);
	((ChimeApp*) app)->GetInfo()->GetCommObject()->SendUDPFunction(ip_address, c_talk, temp);
}

//**********************************************************************
//*
//* show the message in the chat area box
//*
//**********************************************************************
void ChatWindow::ShowMessage(const char* msg) {
	char temp[1000];
	sprintf(temp, "%s: %s", ((ChimeApp*) app)->GetInfo()->GetUsername(), msg);
	new ChatAreaItem(chat_area, temp, last_ID);
}

//**********************************************************************
//*
//* show the message in the chat area box
//*
//**********************************************************************
void ChatWindow::ShowMessage(const char *username, const char* msg) {
	char temp[1000];
	sprintf(temp, "%s: %s", username, msg);
	new ChatAreaItem(chat_area, temp, last_ID);
}

//**********************************************************************
//*
//* add local users to the chat window
//*
//**********************************************************************
void ChatWindow::AddLocalUsers(csStrVector *user_list) {
	
	//first add yourself
	//AddLocalUser(((ChimeApp*) app)->GetInfo()->GetUsername(), ((ChimeApp*)app)->GetInfo()->GetMyIPAddress());

	for (int i = 0; i < user_list->Length(); i++) {
		char username[100];
		char ip_address[100];
		sscanf(user_list->Get(i), "%s %s", username, ip_address);
		AddLocalUser(username, ip_address);
	}
}

//**********************************************************************
//*
//* add a user to the local list
//*
//**********************************************************************
void ChatWindow::AddLocalUser(char *username, char *ip_address) {
	if (!UserThere(local_users_lb, username))
		(void) new UserListBoxItem (local_users_lb, username, ip_address, cslisEmphasized);
	
	AddGlobalUser(username, ip_address);		
}

//**********************************************************************
//*
//* is the user in the listbox?
//*
//**********************************************************************
bool ChatWindow::UserThere(UserListBox *user_list, char *username) {
	if (user_list -> ForEachItem(IsThere, username, false) == NULL)
		return false;
	else
		return true;
}

//**********************************************************************
//*
//* special method syntax used in ForEachItem(). Sees if the user is the
//* same as the current item in the listbox
//*
//**********************************************************************
bool ChatWindow::IsThere(csComponent *item, void *username) {
	if (strcmp( ((UserListBoxItem*)item)->GetUsername(), (char*) username) == 0)
		return true;
	else 
		return false;
}

//**********************************************************************
//*
//* add global users to the chat window
//*
//**********************************************************************
void ChatWindow::AddGlobalUsers(csStrVector *user_list) {
	for (int i = 0; i < user_list->Length(); i++) {
		char username[100];
		char ip_address[100];
		sscanf(user_list->Get(i), "%s %s", username, ip_address);
		AddGlobalUser(username, ip_address);
	}
}

//**********************************************************************
//*
//* add a user to the global list
//*
//**********************************************************************
void ChatWindow::AddGlobalUser(char *username, char *ip_address) {
	if (!UserThere(global_users_lb, username))
		(void) new UserListBoxItem (global_users_lb, username, ip_address, cslisEmphasized);
}

//**********************************************************************
//*
//* make the user ID out of a username and ip_address
//*
//**********************************************************************
char* ChatWindow::MakeUserID(const char *username, const char* ip_address) {
	if (!username) return NULL;
	if (!ip_address) return NULL;

	char* userID = new char[strlen(username) + strlen(ip_address) + 2];
	strcpy(userID, username);
	strcat(userID, " ");
	strcat(userID, ip_address);
	return userID;
}

//**********************************************************************
//*
//* delete a local user
//*
//**********************************************************************
void ChatWindow::DeleteLocalUser(char *username, char *ip_address) {
	csListBoxItem* to_delete = (csListBoxItem*) local_users_lb -> ForEachItem(DeleteMe, username, true);
	local_users_lb->Delete(to_delete);
}

//**********************************************************************
//*
//* delete all local users
//*
//**********************************************************************
void ChatWindow::DeleteAllLocalUsers() {
	local_users_lb -> DeleteAll();
}

//**********************************************************************
//*
//* delete all global users
//*
//**********************************************************************
void ChatWindow::DeleteAllGlobalUsers() {
	global_users_lb -> DeleteAll();
}

//**********************************************************************
//*
//* delete a global user
//*
//**********************************************************************
void ChatWindow::DeleteGlobalUser(char *username, char *ip_address) {

	csListBoxItem* to_delete = (csListBoxItem*) global_users_lb -> ForEachItem(DeleteMe, username, true);
	global_users_lb->Delete(to_delete);
}

//**********************************************************************
//*
//* delete a local user
//*
//**********************************************************************
void ChatWindow::DeleteLocalUser(char *userID) {
	csListBoxItem* to_delete = (csListBoxItem*) local_users_lb -> ForEachItem(DeleteMe, userID, true);
	local_users_lb->Delete(to_delete);
}

//**********************************************************************
//*
//* delete a global user
//*
//**********************************************************************
void ChatWindow::DeleteGlobalUser(char *userID) {
	csListBoxItem* to_delete = (csListBoxItem*) global_users_lb -> ForEachItem(DeleteMe, userID, true);
	global_users_lb->Delete(to_delete);
}

//**********************************************************************
//*
//* to delete or not to delete. Special method used in ForEachItem()
//*
//**********************************************************************
bool ChatWindow::DeleteMe(csComponent *item, void *userID) {
	if (strcmp(item->GetText(), (const char*) userID) == 0)
		return true;
	else
		return false;
}



//**********************************************************************
//*
//* Build the chat area which will be used for chatting
//*
//**********************************************************************
ChatArea::ChatArea(int chars_per_line, csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle) : csListBox(iParent, iStyle=CSLBS_DEFAULTVALUE, iFrameStyle=cslfsThickRect ) {
	SetCharsPerLine(chars_per_line);
	SetState(CSS_SELECTABLE, false);
	csListBox::csListBox(iParent, iStyle, iFrameStyle);
}

bool ChatArea::HandleEvent(csEvent &Event) {
	switch (Event.Type)
	{
	case csevCommand:
      switch (Event.Command.Code)
      {
		//no selection will be supported
        case cscmdListBoxItemSelected:
          return true;
          
        case cscmdListBoxItemDeselected:
        case cscmdListBoxItemClicked:
        case cscmdListBoxItemDoubleClicked:
          // resend command to parent
			if (csListBox::parent)
				csListBox::parent->HandleEvent (Event);
          return true;
	  }
	}

	return csListBox::HandleEvent(Event);
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

//no real reason for having this yet but knowing Suhit there will be one :-)
UserListBox::UserListBox(csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle) : csListBox(iParent, iStyle=CSLBS_VSCROLL | CSLBS_MULTIPLESEL, iFrameStyle=cslfsThinRect ) {
	last_ID = 0;
	csListBox::csListBox(iParent, iStyle, iFrameStyle);
}

UserListBox::GetID() {
	return last_ID++;
}

UserListBox::DeleteAll() {
	csListBox::DeleteAll();
	//Insert (new csComponent(this));

	//csListBox::csListBox(parent, CSLBS_VSCROLL | CSLBS_MULTIPLESEL, cslfsThinRect);
}

//this is an item that is present in the user list box
UserListBoxItem::UserListBoxItem(UserListBox *user_list, const char *username, const char* ip_address, csListBoxItemStyle iStyle) : csListBoxItem (user_list, username, iStyle=cslisNormal) {
	strcpy(UserListBoxItem::username, username);
	strcpy(UserListBoxItem::ip_address, ip_address);
	csListBoxItem::csListBoxItem(user_list, username, user_list->GetID(), iStyle);
}

