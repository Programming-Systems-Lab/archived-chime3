#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "csfedit.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include <sys/stat.h>
#include "csutil/csrect.h"


//-- Gui -------------------------------------------------------------------

static bool printable(char c)
{
  return ( (32<=c) && (c<=126) );
}


void csEditCharView::Reevaluate() //// number of chars in font and so on.
  {
    stats.Set(BorderWidth+inset, BorderHeight+inset+TitlebarHeight,
      BorderWidth+inset + 100, bound.Height()-BorderHeight-inset);
    content.Set(stats.xmax+inset*2, BorderHeight+inset*2+TitlebarHeight,
      bound.Width() - BorderWidth - scrsize, bound.Height() -
      BorderHeight - scrsize);

    scrhor->SetRect(content.xmin-inset, content.ymax,
      content.xmax, content.ymax+scrsize);
    struct csScrollBarStatus status;
    status.value = (int)scrhor->SendCommand(cscmdScrollBarQueryValue, 0);
    status.maxvalue = celsize*editchar->GetWidth() - content.Width();
    if(status.maxvalue<0) status.maxvalue = 0;
    status.size = scrsize;
    status.maxsize = scrhor->bound.Width();
    status.step = 1;
    status.pagestep = content.Width()/2;
    scrhor->SendCommand(cscmdScrollBarSet, (void*)&status);
    scrvert->SetRect(content.xmax, content.ymin-inset,
      content.xmax+scrsize, content.ymax);
    status.value = (int)scrvert->SendCommand(cscmdScrollBarQueryValue, 0);
    status.maxvalue = celsize*editchar->GetHeight() - content.Height();
    if(status.maxvalue<0) status.maxvalue = 0;
    status.size = scrsize;
    status.maxsize = scrvert->bound.Height();
    status.step = 1;
    status.pagestep = content.Height()/2;
    scrvert->SendCommand(cscmdScrollBarSet, (void*)&status);

  }

csEditCharView::csEditCharView(csComponent *iParent, csEditFont *fnt,
    csEditChar *chr)
  : csWindow(iParent, "Edit Character", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {
    isdrawing = false;
    drawcolour = 1;
    font = fnt;
    editchar = chr;
    editchar->SetView(this);

    offx = 0; offy = 0;
    inset = 5; // inset border
    celsize = 10;
    scrsize = CSSB_DEFAULTSIZE;

    palstart = palettesize;
    int* newpalette = new int[palettesize + 6];
    memcpy(newpalette, palette, palettesize*sizeof(int));
    SetPalette(newpalette, palettesize+6);
    SetColor(palstart+0, cs_Color_White);
    SetColor(palstart+1, cs_Color_Black);
    SetColor(palstart+2, cs_Color_Gray_L);
    SetColor(palstart+3, cs_Color_Gray_D);
    SetColor(palstart+4, cs_Color_Blue_M);
    SetColor(palstart+5, cs_Color_Blue_L);
    SetFont(((CsfEdit*)app)->GetMainFont());

    scrhor = new csScrollBar(this);
    scrvert = new csScrollBar(this);
    SetSize(500,500);
    Reevaluate();
    Center();

    int stath; /// for drawing font
    GetTextSize("", &stath);
    csButton *but = new csButton(this, 66700);
    but->SetText("Settings...");
    but->SetPos(stats.xmin+inset/2, stats.ymin+inset+stath*5);
    but->SetSuggestedSize(inset, inset);
    app->HintAdd("Edit the settings for this character.", but);

    but = new csButton(this, cscmdClose);
    but->SetText("Done");
    but->SetPos(stats.xmin+inset/2, stats.ymin+inset+stath*9);
    but->SetSuggestedSize(inset, inset);
    app->HintAdd("Close this window.", but);
  }

bool csEditCharView::SetRect(int xmin, int ymin, int xmax, int ymax)
  {
    if(!csWindow::SetRect(xmin, ymin, xmax, ymax)) return false;
    Reevaluate();
    return true;
  }

csEditCharView::~csEditCharView()
  {
    editchar->SetView(0);
    delete scrhor;
    delete scrvert;
  }

void csEditCharView::Draw()
  {
    SetFont(((CsfEdit*)app)->GetMainFont());
    csWindow::Draw();

    Rect3D(stats.xmin - inset, stats.ymin, stats.xmax, stats.ymax,
      palstart+3, palstart+0);
    int fontheight = 10; /// for drawing font
    char buf[256];
    GetTextSize("", &fontheight);
    int y = stats.ymin + inset + fontheight;
    sprintf(buf, "Font: %s", font->GetFontName()?font->GetFontName():"");
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    sprintf(buf, "Char %d %c", font->GetCharNumber(editchar),
      printable(font->GetCharNumber(editchar))?
      font->GetCharNumber(editchar):' ');
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    sprintf(buf, "Size %dx%d", editchar->GetWidth(), editchar->GetHeight());
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    y += 7*(fontheight+2);
    csRect prev;
    prev.SetPos(stats.xmin, y);
    prev.SetSize(editchar->GetWidth(), editchar->GetHeight());
    Box(prev.xmin, prev.ymin, prev.xmax, prev.ymax, palstart+0);
    Rect3D(prev.xmin-1, prev.ymin-1, prev.xmax+1, prev.ymax+1,
      palstart+2, palstart+3);
    editchar->Draw(this, prev.xmin, prev.ymin, palstart+1);

    Box( content.xmin-inset, content.ymin-inset, content.xmax, content.ymax,
      palstart+0);
    Rect3D( content.xmin-inset, content.ymin-inset, content.xmax, content.ymax,
      palstart+2, palstart+3);
    SetClipRect( content.xmin-inset, content.ymin-inset, content.xmax,
      content.ymax);
    csRect cel;
    int pixcolor;
    int makeborder = 1; /// 1 makes a border
    if(celsize<=1) makeborder=0;
    for(int cely = 0; cely<editchar->GetHeight(); cely++)
      for(int celx = 0; celx<editchar->GetWidth(); celx++)
      {
        if(editchar->GetPixel(celx, cely))
	  pixcolor = 1 + palstart;
	else pixcolor = 0 + palstart;
        cel.Set(content.xmin+celx*celsize, content.ymin+cely*celsize,
          content.xmin+celx*celsize + celsize - makeborder,
	  content.ymin+cely*celsize + celsize - makeborder);
	cel.Move(-offx, -offy);
	Box( cel.xmin, cel.ymin, cel.xmax, cel.ymax, pixcolor);
      }
    Rect3D( content.xmin, content.ymin,
      content.xmin+celsize*editchar->GetWidth(),
      content.ymin+celsize*editchar->GetHeight(), palstart+3, palstart+2);
    SetClipRect(); // disable clipping
    SetFont(((CsfEdit*)app)->GetMainFont());
  }

void csEditCharView::EditSettings()
  {
    csWindow *w = new csWindow(app,  "Edit Character Settings", 0);
    w->SetFont(((CsfEdit*)app)->GetMainFont());
    csDialog *d = new csDialog(w);
    w->SetDragStyle (w->GetDragStyle () & ~CS_DRAG_SIZEABLE);

    w->SetSize(500,500);
    w->Center();

    int px = 15, py = 20;
    int labelw = 150;

    csButton *but = new csButton(d, cscmdOK, CSBS_DEFAULTVALUE |
      CSBS_DISMISS | CSBS_DEFAULT);
    but->SetText("OK");
    but->SetSuggestedSize(16,8);
    but->SetPos(30, 450);
    but = new csButton(d, cscmdCancel, CSBS_DEFAULTVALUE | CSBS_DISMISS);
    but->SetText("Cancel"); but->SetSuggestedSize(16,8);
    but->SetPos(130, 450);

    /// display character number
    char buf[100];
    sprintf(buf, "Character %d %c", font->GetCharNumber(editchar),
      printable(font->GetCharNumber(editchar))?
      font->GetCharNumber(editchar):' ');
    csStatic *stat = new csStatic(d, csscsText);
    stat->SetText(buf);
    stat->SetRect(px, py, px+labelw,py+30);
    py += stat->bound.Height();

    /// width
    csSpinBox *enter_width = new csSpinBox(d);
    enter_width->SetSuggestedSize(100,20);
    enter_width->SetPos(px+labelw,py);
    enter_width->SetLimits(1,1000);
    enter_width->SetValue( editchar->GetWidth() );
    stat = new csStatic(d, enter_width, "~Width:");
    stat->SetRect(px, py, px+labelw,py+enter_width->bound.Height());
    py += enter_width->bound.Height();

    /// zoom factor === celsize
    csSpinBox *enter_zoom = new csSpinBox(d);
    enter_zoom->SetSuggestedSize(100,20);
    enter_zoom->SetPos(px+labelw,py);
    enter_zoom->SetLimits(1,100);
    enter_zoom->SetValue( celsize );
    stat = new csStatic(d, enter_zoom, "~Zoom factor:");
    stat->SetRect(px, py, px+labelw,py+enter_zoom->bound.Height());
    py += enter_zoom->bound.Height();

    if(app->Execute(w) == cscmdOK)
    {
      //// see what changed
      int neww = (int)enter_width->SendCommand(cscmdSpinBoxQueryValue,0);
      int newzoom = (int)enter_zoom->SendCommand(cscmdSpinBoxQueryValue,0);
      if(newzoom != celsize)
      {
        celsize = newzoom;
	Reevaluate();
	Invalidate();
      }
      if(neww != editchar->GetWidth())
      {
	editchar->SetWidth(neww);
	font->MakeDirty();
	font->RecalcWidth();
	font->GetView()->Invalidate();
	Reevaluate();
	Invalidate();
      }
    }
    delete w;
  }

bool csEditCharView::HandleEvent(iEvent &Event)
  {
    int selx, sely;
    if((Event.Type == csevCommand ) &&
      (Event.Command.Code == 66700))
    {
      /// edit character settings dialog
      EditSettings();
      return true;
    }
    if((Event.Type == csevCommand ) &&
      (Event.Command.Code == cscmdClose))
    {
      delete this; /// delete me - sets view to NULL in editchar
      return true;
    }
    if((Event.Type == csevMouseMove) &&
      (!content.Contains(Event.Mouse.x, Event.Mouse.y)))
      isdrawing = false;
    if((Event.Type == csevMouseMove) &&
      (content.Contains(Event.Mouse.x, Event.Mouse.y)))
    {
      SetMouse(csmcArrow);
      if(!isdrawing) return true;

      selx = (Event.Mouse.x - content.xmin + offx ) / celsize;
      sely = (Event.Mouse.y - content.ymin + offy) / celsize;
      csRect inv(content);
      inv.xmin -= inset;
      inv.ymin -= inset;
      if( (selx<0) || (selx>=editchar->GetWidth()) ||
          (sely<0) || (sely>=editchar->GetHeight()))
      {
        return true;
      }
      Invalidate(inv);
      Invalidate(stats);
      font->GetView()->Invalidate(); /// see the change in the font view too
      font->MakeDirty();
      editchar->SetPixel(selx, sely, drawcolour);
      return true;
    }
    if((Event.Type == csevMouseMove) &&
      (stats.Contains(Event.Mouse.x, Event.Mouse.y)))
    {
        SetMouse(csmcArrow);
      return true;
    }

    if((Event.Type == csevCommand ) &&
      (Event.Command.Code == cscmdScrollBarValueChanged))
    {
      int newoff = (int) ((csScrollBar*)Event.Command.Info)->SendCommand(
        cscmdScrollBarQueryValue, NULL);
      if(Event.Command.Info == scrhor) offx = newoff;
      if(Event.Command.Info == scrvert) offy = newoff;
      Invalidate();
      return true;
    }
    if((Event.Type == csevMouseDown) && (Event.Mouse.Button == 1) &&
      content.Contains(Event.Mouse.x, Event.Mouse.y))
    {
      SetMouse(csmcArrow);
      isdrawing = true;
      selx = (Event.Mouse.x - content.xmin + offx ) / celsize;
      sely = (Event.Mouse.y - content.ymin + offy) / celsize;
      if( (selx<0) || (selx>=editchar->GetWidth()) ||
          (sely<0) || (sely>=editchar->GetHeight()))
      {
        drawcolour = 1;
        return true;
      }
      if(editchar->GetPixel(selx, sely))
        drawcolour = 0;
      else drawcolour = 1;

      csRect inv(content);
      inv.xmin -= inset;
      inv.ymin -= inset;
      Invalidate(inv);
      Invalidate(stats);
      font->GetView()->Invalidate(); /// see the change in the font view too
      font->MakeDirty();
      editchar->SetPixel(selx, sely, drawcolour);
      return true;
    }
    if((Event.Type == csevMouseUp) && (Event.Mouse.Button == 1) &&
      content.Contains(Event.Mouse.x, Event.Mouse.y))
    {
      SetMouse(csmcArrow);
      isdrawing = false;
      return true;
    }

    if(IS_MOUSE_EVENT(Event) &&
      content.Contains(Event.Mouse.x, Event.Mouse.y))
    {
      SetMouse(csmcArrow);
      return true;
    }

    if(csWindow::HandleEvent(Event)) return true;

    return false;
  }




