#ifndef CHIMEAPP_H
#define CHIMEAPP_H

#include <stdarg.h>
#include "cssys/sysdriv.h"
#include "csgeom/math2d.h"
#include "csgeom/math3d.h"
#include "csws/csws.h"

#include "ChimeWindow.h"

/*  main app of menu */
class ChimeApp : public csApp
{

  /// reliable fonts, for display
  iFont *mainfont, *tinyfont;
  /// menu item to gray or not
  csMenuItem *saveitem, *closeitem;

public:

  //pointer to the history window
  HistoryWindow *historyWindow;

  //encapsulate all the engine views
  csVector engine_views;

  /// Initialize maze editor
  ChimeApp (iSystem *SysDriver, csSkin &Skin);

  /// Initialize maze editor
  virtual ~ChimeApp ();

  //handle an event
  virtual bool HandleEvent (iEvent &Event);

  virtual bool Initialize (const char *iConfigName);

  void Refresh3D();

  virtual void Draw ();
};



#endif // CHIMEAPP_H

