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



//-- csEditChar ------------------------------------------------------------
csEditChar::csEditChar()
{
  view = NULL;
  pixels = NULL;
  width=10; height=10;
  pixels = new uint8[width*height];
  for(int i=0; i<width*height; i++) pixels[i] = 0;
}


csEditChar::csEditChar(int w, int h, uint8 *bitmap)
{
  view = NULL;
  int i;
  width=w; height=h;
  pixels = new uint8[width*height];
  for(i=0; i<width*height; i++) pixels[i] = 0;

  for (int l = 0; l < h; l++)
  {
    uint8 *line = bitmap + l * ((w + 7) / 8);
    for (i = 0; i < w; i++)
      if(line [i / 8] & (0x80 >> (i & 7))) 
        SetPixel(i,l,1);
      else SetPixel(i,l,0);
  }

}

csEditChar::~csEditChar()
{
  if(view) delete view;
  if(pixels) delete[] pixels;
}


void csEditChar::Draw(csComponent *dest, int minx, int miny, int col)
{
  for(int y = 0; y < height ; y++)
    for(int x = 0; x<width ; x++)
    {
      if(GetPixel(x,y)) dest->Pixel(minx+x, miny+y, col);
    }
}


void csEditChar::SetWidth(int w)
{
  //// (clip / expand) to new width
  uint8* newpix = new uint8 [ w*height ];
  int x,y;
  for(y = 0; y < height ; y++)
    for(x = 0; x<w ; x++)
      newpix[y*w+x] = 0;
  /// copy overlap
  int overw = w;
  if(w>width) overw = width;
  for(y = 0; y < height ; y++)
    for(x = 0; x<overw ; x++)
      newpix[y*w+x] = GetPixel(x,y);
  // swap in
  delete[] pixels;
  pixels = newpix;
  width = w;
}

void csEditChar::SetHeight(int h)
{
  //// (clip / expand) to new height
  uint8* newpix = new uint8 [ width*h ];
  int x,y;
  for(y = 0; y < h; y++)
    for(x = 0; x<width ; x++)
      newpix[y*width+x] = 0;
  /// copy overlap
  int overh = h;
  if(h>height) overh = height;
  for(y = 0; y < overh ; y++)
    for(x = 0; x<width ; x++)
      newpix[y*width+x] = GetPixel(x,y);
  // swap in
  delete[] pixels;
  pixels = newpix;
  height = h;
}


int csEditChar::GetBitmap(int idx)
{
  int bpc = ((width + 7)/8) *height; /// bytes for char
  int bpl = (width + 7)/8; // bytes per line
  if( (idx<0) || (idx>bpc)) return 0;
  int y = idx / bpl;
  int xstart = (idx - y*bpl)*8;
  int res = 0;
  for(int x=xstart; x<xstart+8; x++)
  {
    res<<=1;
    if((x<width) && GetPixel(x,y)) res |=1;
  }
  return res;
}

