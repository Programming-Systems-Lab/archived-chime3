#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "ChimeWindow.h"
#include "ChimeSystemDriver.h"

StartStopWindow::~StartStopWindow() {}


StartStopWindow::StartStopWindow(csComponent *iParent)
  : AlwaysVisibleWindow(iParent, "Start/Stop", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  this->SetRect (app->bound.Width() - 100, app->bound.Height() - 120, app->bound.Width(),  app->bound.Height());


  //////////create the dialog///////////////
  csDialog *d = new csDialog(this);
  this->SetDragStyle (this->GetDragStyle () & ~CS_DRAG_SIZEABLE);

 
  //setup the go button
  csButton *butGo = new csButton(d, GO_PRESSED);
  butGo->SetText("Go");
  butGo->SetSuggestedSize(16,8);
  butGo->SetPos(10, 10);

  //and stop button
  csButton *butStop = new csButton(d, STOP_PRESSED);
  butStop->SetText("Stop");
  butStop->SetSuggestedSize(16,8);
  butStop->SetPos(10, 50);

}


bool StartStopWindow::HandleEvent (iEvent &Event)
{

  if (AlwaysVisibleWindow::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {

    case csevCommand:
      switch (Event.Command.Code)
      {
	    //GO button was pressed
        case GO_PRESSED:
		  ((ChimeSystemDriver*)(app->System))->Start3D();
		  return true;

		 //Cancel Button has been pressed
		case STOP_PRESSED:
		   ((ChimeSystemDriver*)(app->System))->Stop3D();
		  return true;
	  }

      break;
  }

  return false;
}
