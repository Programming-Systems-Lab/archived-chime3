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



//-- csEditFont ------------------------------------------------------------
csEditFont::csEditFont(csApp *iApp)
{
  app = iApp;
  filename = NULL;
  view = NULL;
  dirty = false;
  fontname = strnew("Untitled");
  startchar = 0;
  numchars = 256;
  fontwidth = 10;
  fontheight = 10;
  chars = new csEditChar* [numchars];
  for(int i=0; i<numchars; i++) chars[i] = new csEditChar();
}

csEditFont::csEditFont(csApp *iApp, const char *fromfile)
{
  int i;
  app = iApp;
  filename = strnew(fromfile);
  view = NULL;
  dirty = false;
  fontname = NULL;
  startchar = 0;
  numchars = 256;
  fontwidth = 10;
  fontheight = 10;
  chars = NULL;

  /// read the file
  /// taken from csfont plugin...

  /*  @@@ need to be able to get VFS names
  iVFS *VFS = QUERY_PLUGIN (System, iVFS);
  iDataBuffer *fntfile = VFS->ReadFile (fromfile);
  VFS->DecRef ();
  if (!fntfile)
  {
    System->Printf (MSG_WARNING, "Could not read font file %s.\n", fromfile);
    return;
  }

  char *data = **fntfile;
  */
  struct stat statbuf;
  stat(fromfile, &statbuf);
  int filelen = statbuf.st_size;
  FILE * in = fopen(fromfile, "rb");
  char *data = new char[filelen]; /// @@memleak
  fread(data, 1, filelen, in);
  fclose(in);



  if (data [0] != 'C' || data [1] != 'S' ||  data [2] != 'F')
  {
error:
    //fntfile->DecRef (); // @@@
    ///// create empty font
    fontname = strnew("LoadingError");
    chars = new csEditChar* [numchars];
    for(i=0; i<numchars; i++) chars[i] = new csEditChar();
    return;
  }

  /// the new fontdef to store info into

  numchars = 256;

  char *end = strchr (data, '\n');
  char *cur = strchr (data, '[');
  if (!end || !cur)
    goto error;

  char *binary = end + 1;
  while ((end > data) && ((end [-1] == ' ') || (end [-1] == ']')))
    end--;

  cur++;
  while (cur < end)
  {
    while ((cur < end) && (*cur == ' '))
      cur++;

    char kw [20];
    size_t kwlen = 0;
    while ((cur < end) && (*cur != '=') && (kwlen < sizeof (kw) - 1))
      kw [kwlen++] = *cur++;
    kw [kwlen] = 0;
    if (!kwlen)
      break;

    cur = strchr (cur, '=');
    if (!cur) break;
    cur++;

    if (!strcmp (kw, "Font"))
    {
      char *start = cur;
      while ((cur < end) && (*cur != ' '))
        cur++;
      fontname = new char [cur - start + 1];
      memcpy (fontname, start, cur - start);
      fontname[cur - start] = 0;
    }
    else
    {
      char val [20];
      size_t vallen = 0;
      while ((cur < end) && (*cur != ' ') && (vallen < sizeof (val) - 1))
        val [vallen++] = *cur++;
      val [vallen] = 0;
      int n = atoi (val);

      if (!strcmp (kw, "Width"))
        fontwidth = n;
      else if (!strcmp (kw, "Height"))
        fontheight = n;
      else if (!strcmp (kw, "First"))
        startchar = n;
      else if (!strcmp (kw, "Glyphs"))
        numchars = n;
    }
  }

#if defined(CS_DEBUG)
  printf("Reading Font %s, width %d, height %d, First %d, %d Glyphs.\n",
    fontname, fontwidth, fontheight, startchar, numchars);
#endif

  uint8* IndividualWidth = new uint8 [numchars];
  memcpy (IndividualWidth, binary, numchars);

  // Compute the font size
  int fontsize = 0;
  for (int c = 0; c < numchars; c++)
    fontsize += ((IndividualWidth [c] + 7) / 8) * fontheight;

  // allocate memory and copy the font
  uint8* FontBitmap = new uint8 [fontsize];
  memcpy (FontBitmap, binary + numchars, fontsize);

  //fntfile->DecRef (); // @@@

  ///// further processing of InividualWidth and FontBitmap
  /// create characters
  uint8 *curbitmap = FontBitmap;
  uint8 **GlyphBitmap = new uint8 * [numchars];
  for(i = 0; i < numchars; i++)
  {
    GlyphBitmap [i] = curbitmap;
    curbitmap += ((IndividualWidth [i] + 7) / 8) * fontheight;
  }

  chars = new csEditChar* [numchars];
  for(i=0; i<numchars; i++)
  {
    chars[i] = new csEditChar(IndividualWidth[i], fontheight,
      GlyphBitmap[i]);
  }
  delete[] GlyphBitmap;
  delete[] IndividualWidth;
  delete[] FontBitmap;
}

csEditFont::~csEditFont()
{
  if(dirty)
  {
    if(csMessageBox(app, "Save changes?", "There are unsaved changes. "
      "Do you wish to save before continuing?",
      CSMBS_QUESTION | CSMBS_IGNORE | CSMBS_OK) == cscmdOK)
        Save();
  }
  /// delete font from memory
  ((CsfEdit*)app)->SetEditFont(NULL);
  if(view) delete view;
  if(filename) delete[] filename;
  if(fontname) delete[] fontname;
  if(chars) {
    for(int i=0; i<numchars; i++) delete chars[i];
    delete[] chars;
  }
}


void csEditFont::Save()
{
  if(!filename)
  {
    /// ask a filename
    csWindow *d = csFileDialog (app, "Select a .csf file", "./", "~Save");
    if (d)
    {
      if (app->Execute (d) == cscmdCancel)
      {
        delete d;
        return;
      }
      char fname [MAXPATHLEN + 1];
      csQueryFileDialog (d, fname, sizeof (fname));
      delete d;
      filename = strnew(fname);
      if(view) view->Invalidate();
    }
  }
  printf("saving font\n");
  dirty = false;


  int i, c, w, h;
  int maxwidth = fontwidth;
  int maxheight = fontheight;
  int first = startchar;
  int glyphs = numchars;
  int lastglyph = first + glyphs;
  bool sourcecode = false; //// output for inclusion in sourcecode

  FILE *out = fopen (filename, sourcecode ? "w" : "wb");
  if (!out)
  {
    printf ("Could not open output file %s\n", filename);
    return;
  }

  if (sourcecode)
  {
    /// make a text version
    fprintf (out, "// %s %dx%d font\n", fontname, maxwidth, maxheight);
    fprintf (out, "// FontDef: { \"%s\", %d, %d, %d, %d, font_%s, "
	"width_%s }\n",
      fontname, maxwidth, maxheight, first, glyphs, fontname, fontname);
    fprintf (out, "\n");
  }
  else
    fprintf (out, "CSF [Font=%s Width=%d Height=%d First=%d Glyphs=%d]\n",
      fontname, maxwidth, maxheight, first, glyphs);

  int arrsize = 0;
  uint8 width [256];
  for (c = first; c < lastglyph; c++)
  {
    width[c] = GetChar(c)->GetWidth();
    arrsize += ((width [c] + 7) / 8) * GetChar(c)->GetHeight();
  }

  // Character widths go first
  if (sourcecode)
  {
    fprintf (out, "unsigned char width_%s [%d] =\n{\n  ",
      fontname, glyphs);
    for (i = first; i < lastglyph; i++)
    {
      fprintf (out, "%2d%s", width [i], (i < lastglyph - 1) ? "," : "");
      if ((i & 15) == 15)
      {
        fprintf (out, "\t// %02x..%02x\n", i - 15, i);
        if (i < lastglyph - 1)
          fprintf (out, "  ");
      }
    }
    if (glyphs & 15)
      fprintf (out, "\n");
    fprintf (out, "};\n\n");
    fprintf (out, "unsigned char font_%s [%d] =\n{\n",
      fontname, arrsize);
  }
  else
    fwrite (width + first, glyphs, 1, out);

  // Output every character in turn
  for (c = first; c < lastglyph; c++)
  {
    // get bitmap
    w = GetChar(c)->GetWidth();
    h = GetChar(c)->GetHeight();

    int bpc = ((width [c] + 7) / 8) * h;
    int bitmap;

    if (GetChar(c))
      if (sourcecode)
      {
        fprintf (out, "  ");
        for (i = 0; i < bpc; i++)
	{
	  bitmap = GetChar(c)->GetBitmap(i);
          fprintf (out, "0x%02x%s", bitmap, (i >= bpc - 1) &&
	    (c >= lastglyph - 1) ? "" : ",");
	}
        fprintf (out, "\t// %02x\n", c);
      }
      else if (width [c])
      {
        for (i = 0; i < bpc; i++)
	{
	  bitmap = GetChar(c)->GetBitmap(i);
          fwrite (&bitmap, 1, 1, out);
	}
      }
  }

  fprintf (out, "};\n\n");
  fclose (out);

}


void csEditFont::SetFontName(const char *val)
{
  if(fontname) delete[] fontname;
  fontname = strnew(val);
}

void csEditFont::SetStartNum(int start, int num)
{
  if(start+num > 256) return;
  if( (start<0) || (num<=0) ) return;
  ////move characters
  startchar = start;
  csEditChar **newchars = new csEditChar *[num];
  int i;
  int max = num;
  if(numchars>num) max = numchars;
  for(i=0; i<max; i++)
  {
    if((i<numchars) && (i<num)) newchars[i] = chars[i];
    else
    {
      if(i<numchars) delete chars[i];
      if(i<num)
      {
      newchars[i] = new csEditChar();
      newchars[i]->SetWidth(fontwidth);
      newchars[i]->SetHeight(fontheight);
      }
    }
  }
  /// swap new list of editchars in.
  delete[] chars;
  chars = newchars;
  numchars = num;
}

void csEditFont::SetWidth(int w)
{
  for(int c=0; c<numchars; c++)
    chars[c]->SetWidth(w);
  fontwidth = w;
}

void csEditFont::SetHeight(int h)
{
  for(int c=0; c<numchars; c++)
    chars[c]->SetHeight(h);
  fontheight = h;
}

void csEditFont::RecalcWidth()
{
  int w= 0;
  for(int c=0; c<numchars; c++)
    if(chars[c]->GetWidth() > w)
      w = chars[c]->GetWidth();
  fontwidth = w;
}

int csEditFont::GetCharNumber(csEditChar *editchar)
{
  for(int c=0; c<numchars; c++)
    if(chars[c] == editchar)
      return c + startchar;
  printf("Error: no such character in font, ignoring...\n");
  return startchar + 0;
}

