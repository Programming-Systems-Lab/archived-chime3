#include "cssysdef.h"
#include "cssys/system.h"

#include "cssys/sysdriv.h"
#include "csws/csws.h"
//#include "chimemenu.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <sys/stat.h>
#include "csutil/csrect.h"

#include "ChimeWindow.h"
#include "ChimeSystemDriver.h"

#include "csengine/engine.h"
#include "csengine/csview.h"
#include "csengine/sector.h"
#include "csengine/polygon.h"
#include "csparser/csloader.h"
#include "csparser/crossbld.h"

#include "imspr3d.h"
#include "itxtmgr.h"
#include "icollide.h"
#include "icfgnew.h"
#include "ifontsrv.h"

#include <process.h>
#include "ChimeWorldView.h"

//****************************************************************
//*
//* Create a ChimeWorldView which is just a window in the CSWS
//*
//*****************************************************************
ChimeWorldView::ChimeWorldView (csComponent *iParent, ChimeSystemDriver *System)
  	: csComponent (iParent)
{

  // csView is a view encapsulating both a camera and a clipper.
  // You don't have to use csView as you can do the same by
  // manually creating a camera and a clipper but it makes things a little
  // easier.

  called_out = false;
  ChimeWorldView::System = System;
  
  System->setCoordinateConvertor(this);
  view = System->GetView();

  
  SetState (CSS_SELECTABLE, true);

  if (parent)
    parent->SendCommand (cscmdWindowSetClient, (void *)this);
}

//****************************************************************
//*
//* Destroy the ChimeWorld
//*
//*****************************************************************
ChimeWorldView::~ChimeWorldView ()
{
  delete view;
  ChimeApp* lapp = (ChimeApp*)app;
  lapp->engine_views.Delete (lapp->engine_views.Find (this));
}

//****************************************************************
//*
//* Resize the Window Encapsulating the ChimeWorld
//*
//*****************************************************************
bool ChimeWorldView::SetRect (int xmin, int ymin, int xmax, int ymax)
{
  bool rc = csComponent::SetRect (xmin, ymin, xmax, ymax);

  parent->LocalToGlobal (xmin, ymin);
  parent->LocalToGlobal (xmax, ymax);

  // Engine uses the upside down coordinate system
  ymin = app->bound.Height () - ymin;
  ymax = app->bound.Height () - ymax;
  view->SetRectangle (xmin, ymax, xmax - xmin, ymin - ymax);
  view->GetCamera ()->SetPerspectiveCenter (
  	(xmin + xmax) / 2, (ymin + ymax) / 2);

  return rc;
}

//****************************************************************
//*
//* Handle Any Events emanating from the ChimeWorld
//*
//*****************************************************************
bool ChimeWorldView::HandleEvent (iEvent &Event)
{
	//make the csComponent handle the event
	if (csComponent::HandleEvent(Event))
		return true;

    //make the ChimeSystemDriver handle all events
	if (app->System != NULL && !called_out) {
		called_out = true;

		//if (Event.Type == csevMouseDown)
		//	parent->LocalToGlobal(Event.Mouse.x, Event.Mouse.y);

  		((ChimeSystemDriver*)app->System)->HandleEvent(Event);
		called_out = false;
	}

	if (called_out)
		return false;
	else
		return true;
}


//****************************************************************
//*
//* Draw the View
//*
//*****************************************************************
void ChimeWorldView::Draw ()
{
  // Now its our time... Tell the engine to display 3D graphics
  //System->NextFrame();
  /*app->pplBeginDraw (System->engine->GetBeginDrawFlags ()
  	| CSDRAW_3DGRAPHICS);
  view->Draw ();
   */
  // Ok, now invalidate all the windows that are above us
  inv_struct is;
  is.stop_at = parent;
  is.rect.Set (bound);
  parent->LocalToGlobal (is.rect.xmin, is.rect.ymin);
  parent->LocalToGlobal (is.rect.xmax, is.rect.ymax);
  app->ForEach (do_invalidate, &is, true);

  // Also tell the graphics pipeline to update this rectangle
  app->pplInvalidate (is.rect);
}
