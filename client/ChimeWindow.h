#ifndef CHIMEWINDOW_H
#define CHIMEWINDOW_H

#include <stdarg.h>
#include "cssys/sysdriv.h"
#include "csgeom/math2d.h"
#include "csgeom/math3d.h"
#include "csws/csws.h"


// Scroll bar class default palette
static int palette [] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};


//this is going to be the window that all chime windows which need to be on
//when animation occurs will have to extend
//it will be like a normal csWindow but will deal with events a little differently
class AlwaysVisibleWindow : public csWindow
{
	public:
		AlwaysVisibleWindow(csComponent *iParent, const char *iTitle, int iWindowStyle=CSWS_DEFAULTVALUE, csWindowFrameStyle iFrameStyle=cswfs3D);
		virtual bool HandleEvent(iEvent &Event);
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


class ChatArea : public csListBox
{
	int chars_per_line;
public:
	ChatArea (int chars_per_line, csComponent *iParent, int iStyle=CSLBS_DEFAULTVALUE, csListBoxFrameStyle iFrameStyle=cslfsThickRect);
	int GetCharsPerLine() { return chars_per_line; };
	void SetCharsPerLine(int chars_per_line) { ChatArea::chars_per_line = chars_per_line; };
};

class ChatAreaItem : public csComponent
{

public:
	ChatAreaItem( ChatArea *chat_area, const char *iText, int &iID, csListBoxItemStyle iStyle=cslisNormal);
	FindSpace(const char* str, int max_chars, int *earliest_break, int *latest_break);

};


//Chat Dialog
#define LOCAL_USER 0;
#define GLOBAL_USER 1;

class ChatWindow : public AlwaysVisibleWindow

{

  csInputLine *user_msg_line;
  ChatArea *chat_area;
  int last_ID;
  csListBox *local_users_lb;
  csListBox *global_users_lb;

  //void gui ();

public:

  ChatWindow(csComponent *iParent);
  virtual ~ChatWindow ();
  virtual bool HandleEvent (iEvent &Event);
  void SubmitMessage(const char* msg);
  void AddLocalUsers(csStrVector *users);
  void AddGlobalUsers(csStrVector *users);
  void AddLocalUser(char *userID);
  void AddGlobalUser(char *userID);
  void DeleteLocalUser(char *userID);
  void DeleteGlobalUser(char *userID);
  static bool DeleteMe(csComponent *item, void *userID);
  //virtual bool Initialize (const char *iConfigName);

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