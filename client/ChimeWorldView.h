

/**
 * This is a view on the 3D engine. It is a subclass of csComponent
 * so that it behaves nicely in the CSWS framework.
 */
class ChimeWorldView : public csComponent
{
  csView *view;
  chimeBrowser *System;					//the ChimeSystem
  bool called_out;						//HandleEvent has called out on another Handle Event

public:
  ChimeWorldView (csComponent *iParent, chimeBrowser *System);
  virtual ~ChimeWorldView ();

  // Track movement of the window and update engine.
  virtual bool SetRect (int xmin, int ymin, int xmax, int ymax);
  // Redraw the engine view.
  virtual void Draw ();
  // Do motion etc.
  virtual bool HandleEvent (iEvent &Event);
  // Get the view.
  csView* GetView () { return view; }

  //convert from Local to Global coordinates
  bool ChimeWorldView::LocalToGlobal(csVector2 *screenCoord);

};

// private structure for internal communication with do_invalidate
struct inv_struct
{
  csRect rect;
  csComponent *stop_at;
};

static bool do_invalidate (csComponent *child, void *param)
{
  inv_struct *is = (inv_struct *)param;

  if (child == is->stop_at)
    return true;

  if (child->bound.Intersects (is->rect))
  {
    csRect rel (is->rect);
    rel.xmin -= child->bound.xmin;
    rel.ymin -= child->bound.ymin;
    rel.xmax -= child->bound.xmin;
    rel.ymax -= child->bound.ymin;
    child->Invalidate (rel, true);
  }

  return false;
};

