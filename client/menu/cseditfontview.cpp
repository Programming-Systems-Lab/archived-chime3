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




void csEditFontView::Reevaluate() //// number of chars in font and so on.
  {
    int tinyw = 5, tinyh = 5;
    ((CsfEdit*)app)->GetTinyFont()->GetMaxSize(tinyw, tinyh);
    celw = font->GetWidth() + inset;
    if(celw < 4*tinyw) celw = 4*tinyw;
    celh = font->GetHeight() + inset + tinyh;
    celperrow = (int)sqrt(font->GetNumChars());
    celpercol = (font->GetNumChars() + celperrow -1) / celperrow;

    stats.Set(BorderWidth+inset, BorderHeight+inset+TitlebarHeight,
      BorderWidth+inset + 100, bound.Height()-BorderHeight-inset);
    content.Set(stats.xmax+inset*2, BorderHeight+inset*2+TitlebarHeight,
      bound.Width() - BorderWidth - scrsize, bound.Height() -
      BorderHeight - scrsize);

    scrhor->SetRect(content.xmin-inset, content.ymax,
      content.xmax, content.ymax+scrsize);
    struct csScrollBarStatus status;
    status.value = (int)scrhor->SendCommand(cscmdScrollBarQueryValue, 0);
    status.maxvalue = celw*celperrow + inset - content.Width();
    if(status.maxvalue<0) status.maxvalue = 0;
    status.size = scrsize;
    status.maxsize = scrhor->bound.Width();
    status.step = 1;
    status.pagestep = content.Width()/2;
    scrhor->SendCommand(cscmdScrollBarSet, (void*)&status);
    scrvert->SetRect(content.xmax, content.ymin-inset,
      content.xmax+scrsize, content.ymax);
    status.value = (int)scrvert->SendCommand(cscmdScrollBarQueryValue, 0);
    status.maxvalue = celh*celpercol + inset - content.Height();
    if(status.maxvalue<0) status.maxvalue = 0;
    status.size = scrsize;
    status.maxsize = scrvert->bound.Height();
    status.step = 1;
    status.pagestep = content.Height()/2;
    scrvert->SendCommand(cscmdScrollBarSet, (void*)&status);

  }

csEditFontView::csEditFontView(csComponent *iParent, csEditFont *fnt)
  : csWindow(iParent, fnt->GetFontName(), CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {
    font = fnt;
    font->SetView(this);

    selected = false;
    selx = 0; sely = 0;
    offx = 0; offy = 0;
    inset = 5; // inset border
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
    but->SetPos(stats.xmin+inset/2, stats.ymin+inset+stath*10);
    but->SetSuggestedSize(inset, inset);
    app->HintAdd("Edit the font settings or change the number or size "
      "of all the characters in the font.", but);
  }

bool csEditFontView::SetRect(int xmin, int ymin, int xmax, int ymax)
  {
    if(!csWindow::SetRect(xmin, ymin, xmax, ymax)) return false;

    Reevaluate();

    return true;
  }

csEditFontView::~csEditFontView()
  {
    font->SetView(0);
    delete scrhor;
    delete scrvert;
  }

void csEditFontView::Draw()
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
    sprintf(buf, "File: %s", font->GetFileName()?font->GetFileName():"<none>");
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    sprintf(buf, "First: %d", font->GetStartChar());
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    sprintf(buf, "Number: %d", font->GetNumChars());
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;
    sprintf(buf, "Size %dx%d", font->GetWidth(), font->GetHeight());
    Text(stats.xmin, y, palstart+1, -1, buf);
    y += fontheight+2;

    SetFont(((CsfEdit*)app)->GetTinyFont());
    int tinyw = 5, tinyh = 5;
    ((CsfEdit*)app)->GetTinyFont()->GetMaxSize(tinyw, tinyh);
    GetTextSize("", &fontheight);
    Box( content.xmin-inset, content.ymin-inset, content.xmax, content.ymax,
      palstart+0);
    Rect3D( content.xmin-inset, content.ymin-inset, content.xmax, content.ymax,
      palstart+2, palstart+3);
    SetClipRect( content.xmin-inset, content.ymin-inset, content.xmax,
      content.ymax);
    csRect cel;
    int charnum;
    for(int cely = 0; cely<celpercol; cely++)
      for(int celx = 0; celx<celperrow; celx++)
      {
	charnum = cely*celperrow+ celx + font->GetStartChar();
	if(charnum >= font->GetStartChar()+font->GetNumChars() ) continue;
        cel.Set(content.xmin+celx*celw, content.ymin+cely*celh,
          content.xmin+celx*celw + font->GetChar(charnum)->GetWidth(),
	  content.ymin+cely*celh + font->GetChar(charnum)->GetHeight());
	cel.Move(-offx, -offy);
	font->GetChar(charnum)->Draw(this, cel.xmin, cel.ymin, palstart+1);
        Rect3D(cel.xmin-2, cel.ymin-2, cel.xmax+2, cel.ymax+2,
	  palstart+3, palstart+2);
        sprintf(buf, "%2.2x %c", charnum, printable(charnum)?charnum:' ');
	Text(cel.xmin-2, cel.ymax+3, palstart+1, -1, buf);
      }

    if(selected)
    {
      charnum = sely*celperrow+ selx + font->GetStartChar();
      if(charnum < font->GetStartChar()+font->GetNumChars() )
      {
       cel.Set(content.xmin+selx*celw, content.ymin+sely*celh,
         content.xmin+selx*celw + font->GetChar(charnum)->GetWidth(),
         content.ymin+sely*celh + font->GetChar(charnum)->GetHeight());
       cel.Move(-offx, -offy);
       Rect3D(cel.xmin-2, cel.ymin-2, cel.xmax+2, cel.ymax+2,
         palstart+4, palstart+5);
       Rect3D(cel.xmin-3, cel.ymin-3, cel.xmax+3, cel.ymax+3,
         palstart+4, palstart+5);
      }
    }

    SetClipRect(); // disable clipping
    SetFont(((CsfEdit*)app)->GetMainFont());
  }

void csEditFontView::EditSettings()
  {
    csWindow *w = new csWindow(app,  "Edit Font Settings", 0);
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

    /// fontname, first, number, size
    csInputLine *enter_name = new csInputLine(d);
    enter_name->SetSize(300,30);
    enter_name->SetPos(px+labelw,py);
    enter_name->SetText(font->GetFontName());
    csStatic *stat = new csStatic(d, enter_name, "~Font name:");
    stat->SetRect(px, py, px+labelw,py+enter_name->bound.Height());
    py += enter_name->bound.Height();

    /// first char
    csSpinBox *enter_first = new csSpinBox(d);
    enter_first->SetSuggestedSize(100,20);
    enter_first->SetPos(px+labelw,py);
    enter_first->SetLimits(0,255);
    enter_first->SetValue( font->GetStartChar() );
    stat = new csStatic(d, enter_first, "First ~Character:");
    stat->SetRect(px, py, px+labelw,py+enter_first->bound.Height());
    py += enter_first->bound.Height();

    /// number of  chars
    csSpinBox *enter_number = new csSpinBox(d);
    enter_number->SetSuggestedSize(100,20);
    enter_number->SetPos(px+labelw,py);
    enter_number->SetLimits(1,256);
    enter_number->SetValue( font->GetNumChars() );
    stat = new csStatic(d, enter_number, "~Number of chars:");
    stat->SetRect(px, py, px+labelw,py+enter_number->bound.Height());
    py += enter_number->bound.Height();

    /// number of  chars
    csSpinBox *enter_w = new csSpinBox(d);
    enter_w->SetSuggestedSize(100,20);
    enter_w->SetPos(px+labelw,py);
    enter_w->SetLimits(1,1000);
    enter_w->SetValue( font->GetWidth() );
    stat = new csStatic(d, enter_w, "~Width:");
    stat->SetRect(px, py, px+labelw,py+enter_w->bound.Height());
    py += enter_w->bound.Height();

    /// number of  chars
    csSpinBox *enter_h = new csSpinBox(d);
    enter_h->SetSuggestedSize(100,20);
    enter_h->SetPos(px+labelw,py);
    enter_h->SetLimits(1,1000);
    enter_h->SetValue( font->GetHeight() );
    stat = new csStatic(d, enter_h, "~Height:");
    stat->SetRect(px, py, px+labelw,py+enter_h->bound.Height());
    py += enter_h->bound.Height();

    if(app->Execute(w) == cscmdOK)
    {
      //// see what changed
      if(strcmp(enter_name->GetText(), font->GetFontName()) != 0)
      {
        font->SetFontName(enter_name->GetText());
	font->MakeDirty();
      }
      int newfirst = (int)enter_first->SendCommand(cscmdSpinBoxQueryValue,0);
      int newnum = (int)enter_number->SendCommand(cscmdSpinBoxQueryValue,0);
      int neww = (int)enter_w->SendCommand(cscmdSpinBoxQueryValue,0);
      int newh = (int)enter_h->SendCommand(cscmdSpinBoxQueryValue,0);
      if((newfirst != font->GetStartChar()) || (newnum != font->GetNumChars()))
	{
	  font->SetStartNum( newfirst, newnum );
	  font->MakeDirty();
	  Reevaluate();
	}
      if(neww != font->GetWidth())
	{
	  font->SetWidth(neww);
	  font->MakeDirty();
	  Reevaluate();
	}
      if(newh != font->GetHeight())
	{
	  font->SetHeight(newh);
	  font->MakeDirty();
	  Reevaluate();
	}
      Invalidate();
    }
    delete w;
  }


bool csEditFontView::HandleEvent(iEvent &Event)
  {
    if((Event.Type == csevCommand ) &&
      (Event.Command.Code == 66700))
    {
      /// edit font settings dialog
      EditSettings();
      return true;
    }
    if((Event.Type == csevCommand ) &&
      (Event.Command.Code == cscmdClose))
    {
      delete font; /// also deletes me
      return true;
    }
    if((Event.Type == csevMouseMove) &&
      (content.Contains(Event.Mouse.x, Event.Mouse.y)))
    {
      SetMouse(csmcArrow);
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
      selected = true;
      selx = (Event.Mouse.x - content.xmin + offx + inset) / celw;
      sely = (Event.Mouse.y - content.ymin + offy + inset/2) / celh;
      //// modified as follows: the text beneath each cell belongs to
      //// that cell. But the area left&right is divided evenly.
      csRect inv(content);
      inv.xmin -= inset;
      inv.ymin -= inset;
      Invalidate(inv);
      if( (selx<0) || (selx>=celperrow) || (sely<0) || (sely>=celpercol))
      {
        selected = false;
        return true;
      }
    }

    if((Event.Type == csevMouseClick) && (Event.Mouse.Button == 1) &&
      content.Contains(Event.Mouse.x, Event.Mouse.y))
    {
      selected = true;
      selx = (Event.Mouse.x - content.xmin + offx + inset) / celw;
      sely = (Event.Mouse.y - content.ymin + offy + inset/2) / celh;
      //// modified as follows: the text beneath each cell belongs to
      //// that cell. But the area left&right is divided evenly.
      csRect inv(content);
      inv.xmin -= inset;
      inv.ymin -= inset;
      Invalidate(inv);
      if( (selx<0) || (selx>=celperrow) || (sely<0) || (sely>=celpercol))
      {
        selected = false;
        return true;
      }
      /// open a character window
      int zecharnum = sely * celperrow + selx;
      if(!font->GetChar(zecharnum)) return true; /// robust
      csComponent *v = 0;
      if(font->GetChar(zecharnum)->GetView())
      {
        v = font->GetChar(zecharnum)->GetView();
      }
      else
      {
        v = new csEditCharView(app, font, font->GetChar(zecharnum));
      }
      app->SetZorder(v, this);
      v->Show();
      v->Invalidate();
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
