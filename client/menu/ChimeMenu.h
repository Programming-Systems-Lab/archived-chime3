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



/*  main app of csf font editor */
class ChimeMenu : public csApp
{

  /// reliable fonts, for display
  iFont *mainfont, *tinyfont;
  /// menu item to gray or not
  csMenuItem *saveitem, *closeitem;

public:
  /// Initialize maze editor
  ChimeMenu (iSystem *SysDriver, csSkin &Skin);

  /// Initialize maze editor
  virtual ~ChimeMenu ();

  ///
  virtual bool HandleEvent (iEvent &Event);

  virtual bool Initialize (const char *iConfigName);
};




//VEM stuff
class VEM : public csWindow
{
  csListBox *lbFile;
  csListBox *lbType;
  csListBox *lbSub;
  csListBox *lbObject;

  csInputLine *ilFile;
  csInputLine *ilType;
  csInputLine *ilSub;
  csInputLine *ilObject;

  //void gui ();

public:
  VEM(csComponent *iParent);

  virtual ~VEM ();
  virtual bool HandleEvent (iEvent &Event);
  //virtual bool Initialize (const char *iConfigName);

protected:
  void FillFile ();
  void FillType ();
  void FillSub  ();
  void FillObject ();
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

#endif // CSFEDIT_H

