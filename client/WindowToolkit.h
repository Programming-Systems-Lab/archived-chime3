//this is going to be the window that all chime windows which need to be on
//when animation occurs will have to extend
//it will be like a normal csWindow but will deal with events a little differently
class AlwaysVisibleWindow : public csWindow
{
	public:
		AlwaysVisibleWindow(csComponent *iParent, const char *iTitle, int iWindowStyle=CSWS_DEFAULTVALUE, csWindowFrameStyle iFrameStyle=cswfs3D);
		virtual bool HandleEvent(iEvent &Event);
};

//this is just the plain chat area
class ChatArea : public csListBox
{
	int chars_per_line;
public:
	ChatArea (int chars_per_line, csComponent *iParent, int iStyle=CSLBS_DEFAULTVALUE, csListBoxFrameStyle iFrameStyle=cslfsThickRect);
	int GetCharsPerLine() { return chars_per_line; };
	void SetCharsPerLine(int chars_per_line) { ChatArea::chars_per_line = chars_per_line; };
	bool ChatArea::HandleEvent(csEvent &Event);
};

//this is just the plain chat area item
class ChatAreaItem : public csComponent
{

public:
	ChatAreaItem( ChatArea *chat_area, const char *iText, int &iID, csListBoxItemStyle iStyle=cslisNormal);
	FindSpace(const char* str, int max_chars, int *earliest_break, int *latest_break);

};


//no real reason for having this yet but knowing Suhit there will be one :-)
class UserListBox : public csListBox
{
	long last_ID;

public:
	UserListBox(csComponent *iParent, int iStyle, csListBoxFrameStyle iFrameStyle);
	GetID();  //Get the last ID
};

//this is an item that is present in the user list box
class UserListBoxItem : public csListBoxItem 
{
private:
	char username[100];
	char ip_address[50];

public:
	UserListBoxItem(UserListBox *user_list, const char *username, const char* ip_address, csListBoxItemStyle iStyle);
	const char* GetUsername() { return username; };
	const char* GetIPAddress() { return ip_address; };
};
