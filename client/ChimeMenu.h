/*
    Copyright (C) 1998-2000 by Jorrit Tyberghein
    Copyright (C) 2001 by W.C.A. Wijngaards

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CSFEDIT_H
#define CSFEDIT_H

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


/*  main app of menu */
class ChimeMenu : public csApp
{

  /// reliable fonts, for display
  iFont *mainfont, *tinyfont;
  /// menu item to gray or not
  csMenuItem *saveitem, *closeitem;

public:

  //encapsulate all the engine views
  csVector engine_views;

  /// Initialize maze editor
  ChimeMenu (iSystem *SysDriver, csSkin &Skin);

  /// Initialize maze editor
  virtual ~ChimeMenu ();

  //handle an event
  virtual bool HandleEvent (iEvent &Event);

  virtual bool Initialize (const char *iConfigName);

  void Refresh3D();

  virtual void Draw ();
};



//VEM stuff
class ChimeVEM : public csWindow
{
  public:
	ChimeVEM(csComponent *iParent);
	~ChimeVEM() {} ;
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

class ConnectDialog : public csWindow

{

  csInputLine *username;
  csInputLine *password;
  csInputLine *chime_server;

  //void gui ();

public:

  ConnectDialog(csComponent *iParent);
  virtual ~ConnectDialog ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);

};


//ObjectToRetrieve
class ObjectToRetrieveDialog : public csWindow
{
  csInputLine *object;
  csInputLine *protocol;
  //void gui ();

public:

  ObjectToRetrieveDialog(csComponent *iParent);
  virtual ~ObjectToRetrieveDialog ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);
};



//Siena Setting dialog
//Connect stuff
class SienaDialog : public csWindow
{
  csListBox *lbFile;
  csListBox *lbType;
  csListBox *lbSub;
  csListBox *lbObject;

  //void gui ();
public:

  SienaDialog(csComponent *iParent);
  virtual ~SienaDialog ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);
};


class InfoStorer
{
   //the username
  char *username;
  //the password
  char *password;
  //object to retrieve
  char *object;


  //protocol to use
  char *protocol;

  //siena location
  char *siena_location;

  //chat server to use
  char *chat_server;

  //chat port
  char *port;
};


//Chat Dialog
class ChatWindow : public csWindow

{

  csInputLine *username;
  csInputLine *password;
  csInputLine *chime_server;

  //void gui ();

public:

  ChatWindow(csComponent *iParent);
  virtual ~ChatWindow ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);

};


//History Dialog
class HistoryWindow : public csWindow

{

  csInputLine *username;
  csInputLine *password;
  csInputLine *chime_server;

  //void gui ();

public:

  HistoryWindow(csComponent *iParent);
  virtual ~HistoryWindow ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);

};



#endif // CSFEDIT_H

