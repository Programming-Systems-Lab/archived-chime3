#ifndef CHIMEWINDOW_H
#define CHIMEWINDOW_H

#include <stdarg.h>
#include "cssys/sysdriv.h"
#include "csgeom/math2d.h"
#include "csgeom/math3d.h"
#include "csws/csws.h"
#include "WindowToolkit.h"

// Scroll bar class default palette
static int palette [] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};




//VEM stuff
class VEMWindow : public AlwaysVisibleWindow
{
  public:
	VEMWindow(csComponent *iParent);
	~VEMWindow() {} ;
 };


// Main VeM class that exends csDialog
class VeM : public csDialog
{
	csListBox *lbFile;
	csListBox *lbType;
	csListBox *lbSub;
	csListBox *lbObject;

	csInputLine *ilFile;
	csInputLine *ilType;
	csInputLine *ilSub;
	csInputLine *ilObject;

  public:
	VeM (csComponent *iParent) : csDialog (iParent) {}

	virtual void Gui (int user);
	virtual void Publish (char *host, short port);

	virtual void FillFiles ();
	virtual void FillListBox (char *which, csListBox *lstbx);

	virtual bool HandleEvent (iEvent &Event)
	{
		if (Event.Type == csevCommand)
		{
			switch (Event.Command.Code)
			{
				case 70000: // Apply Button
					Publish ("localhost", 1111); // Where do u get the ip and port from????
					return true;
					break;
				case 70001: // Clear Button
					this -> GetChild (1000) -> SetText (""); 
					this -> GetChild (1001) -> SetText ("");
					this -> GetChild (1002) -> SetText ("");
					this -> GetChild (1003) -> SetText ("");
					return true;
					break;
			}
		}

		return csDialog::HandleEvent (Event);
	}
};




//Connect stuff

class ConnectWindow : public csWindow

{

  csInputLine *username;
  csInputLine *password;
  csInputLine *chime_server;

  //void gui ();

public:

  ConnectWindow(csComponent *iParent);
  virtual ~ConnectWindow ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);

};


//ObjectToRetrieve
class GetObjectWindow : public csWindow
{
  csInputLine *object;
  csInputLine *protocol;
  //void gui ();

public:

  GetObjectWindow(csComponent *iParent);
  virtual ~GetObjectWindow ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);
};



//Siena Setting dialog
//Connect stuff
class SienaWindow : public csWindow
{
  csListBox *lbFile;
  csListBox *lbType;
  csListBox *lbSub;
  csListBox *lbObject;

  //void gui ();
public:

  SienaWindow(csComponent *iParent);
  virtual ~SienaWindow ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);
};




//Chat Dialog
#define LOCAL_USER 0;
#define GLOBAL_USER 1;

class ChatWindow : public AlwaysVisibleWindow

{

  csInputLine *user_msg_line;
  ChatArea *chat_area;
  int last_ID;
  UserListBox *local_users_lb;
  UserListBox *global_users_lb;
  csNotebook *nb;
  csDialog *page1;
  csDialog *page2;

  //void gui ();

public:

  ChatWindow(csComponent *iParent);
  virtual ~ChatWindow ();
  virtual bool HandleEvent (iEvent &Event);
  static bool SendMsg(csComponent *item, void *msg);
  void SendMessage(const char *ip_address, const char *msg);
  void ShowMessage(const char* msg);
  void ShowMessage(const char *username, const char* msg);
  void AddLocalUsers(csStrVector *users);
  void AddGlobalUsers(csStrVector *users);
  void AddLocalUser(char *username, char *ip_address);
  void AddGlobalUser(char *username, char *ip_address);
  void DeleteLocalUser(char *userID);
  void DeleteGlobalUser(char *userID);
  void DeleteLocalUser(char *username, char *ip_address);
  void DeleteGlobalUser(char *userID, char *ip_address);
  static bool DeleteMe(csComponent *item, void *userID);
  //virtual bool Initialize (const char *iConfigName);
  char* MakeUserID(const char *username, const char* ip_address);

  //is user in listbox?
  bool UserThere(UserListBox *user_list, char *username);

  //special method used in ForEachItem()
  static bool IsThere(csComponent *item, void *username);

  //delete all local users
  void DeleteAllLocalUsers();

  //delete all global users
  void DeleteAllGlobalUsers();

};


#define GO_THERE_PRESSED 7000

//History Window
class HistoryWindow : public AlwaysVisibleWindow
{

private:
	csListBox *lb;
	int count;
	csListBoxItem *last_item;

	void InitCount() {
		count = 1; };

	int ItemCount() {
		return count; };

	void AddItem() {
		count++; };

	void RemoveItem() {
		count--; };

	csVector history_list;
	
	char selected [50];

public:

  HistoryWindow(csComponent *iParent);
  bool AddItem(char *item);
  bool FindSector(char *text);
  void RemoveItem(char *item);
  virtual ~HistoryWindow ();
  virtual bool HandleEvent (iEvent &Event);

};



#define GO_PRESSED 10000
#define STOP_PRESSED 10001

class StartStopWindow : public AlwaysVisibleWindow
{

public:

  StartStopWindow(csComponent *iParent);
  virtual ~StartStopWindow ();
  virtual bool HandleEvent (iEvent &Event);
  
};

#endif //CHIME_WINDOW_H