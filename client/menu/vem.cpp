#define SYSDEF_PATH
#include "cssysdef.h"
#include "cssys/sysdriv.h"
#include "csws/csws.h"
#include "version.h"
#include "ifontsrv.h"
#include "icfgnew.h"
#include "chimemenu.h"



// Scroll bar class default palette
static int palette_VEM[] =
{
  cs_Color_Gray_D,			// Application workspace
  cs_Color_Green_L,			// End points
  cs_Color_Red_L,			// lines
  cs_Color_White			// Start points
};

VEM::~VEM() {}

void VEM::FillFile ()
{}
void VEM::FillType ()
{}
void VEM::FillSub ()
{}
void VEM::FillObject ()
{}

VEM::VEM(csComponent *iParent)
  : csWindow(iParent, "VEM Console", CSWS_TITLEBAR | CSWS_BUTCLOSE |
    CSWS_BUTMAXIMIZE)
  {

  this->SetSize (500, 289);
  this->Center ();

  ///////////////////////  Notebook Begin  ///////////////////////////////////////////////////////////////

  csNotebook *nb = new csNotebook (this, CSNBS_TABPOS_LEFT | CSNBS_PAGEFRAME | CSNBS_PAGEINFO);
  this->SendCommand (cscmdWindowSetClient, (void *)nb);

  csComponent *page = new csDialog (nb);
  nb->AddPrimaryTab (page, "~Files", "Choose the file");
  nb->SetRect (7, 100, 493, 278);

  lbFile = new csListBox (page, CSLBS_VSCROLL, cslfsThickRect);
  lbFile->SetRect (155, 5, 375, 152);

  (void) new csListBoxItem (lbFile, "index.html", 0, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "page.html", 1, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "help.pdf", 2, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "animal.gif", 3, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "tonga.gif", 4, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "delta.bmp", 5, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "nerve.jpg", 6, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "elt-50.zip", 7, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "ender.exe", 8, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "sqasqu.htm", 9, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "merck.asp", 10, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "shifty.asp", 11, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "hammer.cgi", 12, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "oblivionunderthezoo.html", 13, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "single.xml", 14, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "serpentobliteration.asp", 15, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "hijaack.rar", 16, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "gund.ace", 17, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "hello.java", 18, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "siggy.cpp", 19, cslisEmphasized);
  (void) new csListBoxItem (lbFile, "orion.vbs", 20, cslisEmphasized);

  csRadioButton *rbOne = new csRadioButton (page, 9900);
  rbOne->SetPos (5, 5);
  rbOne->SetState (CSS_GROUP, true);
  rbOne->SendCommand (cscmdRadioButtonSet, (void *)true);
  csStatic *st = new csStatic (page, rbOne, "This File");
  st->SetPos (23, 7);

  csRadioButton *rbAll = new csRadioButton (page, 9901);
  rbAll->SetPos (5, 25);
  st = new csStatic (page, rbAll, "This File Type");
  st->SetPos (23, 27);

  csButton *butAcceptFile = new csButton (page, 70001);
  butAcceptFile->SetText ("Accept");
  butAcceptFile->SetRect (37, 132, 117, 152);

  page = new csDialog (nb);
  nb->AddPrimaryTab (page, "~Type", "Choose the type of file");

  lbType = new csListBox (page, CSLBS_VSCROLL, cslfsThickRect);
  lbType->SetRect (155, 5, 375, 152);

  (void) new csListBoxItem (lbType, "Container", 0, cslisEmphasized);
  (void) new csListBoxItem (lbType, "Link", 1, cslisEmphasized);
  (void) new csListBoxItem (lbType, "Object", 2, cslisEmphasized);

  csButton *butAcceptType = new csButton (page, 70002);
  butAcceptType->SetText ("Accept");
  butAcceptType->SetRect (37, 132, 117, 152);

  page = new csDialog (nb);
  nb->AddPrimaryTab (page, "~Sub-Type", "Choose the Sub-Type of file");

  lbSub = new csListBox (page, CSLBS_VSCROLL, cslfsThickRect);
  lbSub->SetRect (155, 5, 375, 152);

  for (int i = 1; i < 50; i++)
  {
	char tmp[20];
    sprintf (tmp, "Sub Type %d", i);
    (void)new csListBoxItem (lbSub, tmp, i);
  }

  csButton *butAcceptSub = new csButton (page, 70003);
  butAcceptSub->SetText ("Accept");
  butAcceptSub->SetRect (37, 132, 117, 152);

  page = new csDialog (nb);
  nb->AddPrimaryTab (page, "3D ~Object", "Choose the 3D object representing the file");

  lbObject = new csListBox (page, CSLBS_VSCROLL, cslfsThickRect);
  lbObject->SetRect (155, 5, 375, 152);

  st = new csStatic (page, NULL, "Preview");
  st->SetPos (50, 5);

  csStatic (page, NULL);
  st->SetPos (50, 17);

  for (i = 1; i < 100; i++)
  {
	char tmp [20];
    sprintf (tmp, "object_%d.3ds", i);
    (void)new csListBoxItem (lbObject, tmp, i);
  }

  csButton *butAcceptObject = new csButton (page, 70004);
  butAcceptObject->SetText ("Accept");
  butAcceptObject->SetRect (37, 132, 117, 152);

  ///////////////////////  Notebook End  /////////////////////////////////////////////////////////////////



  ///////////////////////  Top Frame Begin ///////////////////////////////////////////////////////////////

  st = new csStatic (this, NULL, "", csscsFrameLabel);
  st->SetRect (7, 27, 493, 93);

  st = new csStatic (this, NULL, "File: ");
  st->SetPos (12, 43);

  ilFile = new csInputLine (this, 40, csifsThinRect);
  ilFile->SetRect (52, 39, 152, 55);

  st = new csStatic (this, NULL, "Type: ");
  st->SetPos (12, 69);

  ilType = new csInputLine (this, 40, csifsThinRect);
  ilType->SetRect (52, 65, 152, 81);

  st = new csStatic (this, NULL, "Sub-Type: ");
  st->SetPos (165, 43);

  ilSub = new csInputLine (this, 40, csifsThinRect);
  ilSub->SetRect (240, 39, 340, 55);

  st = new csStatic (this, NULL, "3D Object: ");
  st->SetPos (165, 69);

  ilObject = new csInputLine (this, 40, csifsThinRect);
  ilObject->SetRect (240, 65, 340, 81);

  csButton *butAccept = new csButton (this, cscmdNothing);
  butAccept->SetText ("Apply");
  butAccept->SetRect (380, 37, 460, 57);

  csButton *butReset = new csButton (this, cscmdNothing);
  butReset->SetText ("Reset");
  butReset->SetRect (380, 63, 460, 83);

  ///////////////////////  Top Frame End //////////////////////////////////////////////////////////////////
}


bool VEM::HandleEvent (iEvent &Event)
{

  if (csWindow::HandleEvent (Event))
    return true;

  switch (Event.Type)
  {
    case csevCommand:
      switch (Event.Command.Code)
      {
        case 66620:
          //gui ();
          return true;
		  break;
		case 70001:
          FillFile ();
          return true;
		  break;
		case 70002:
          FillType ();
          return true;
		  break;
		case 70003:
          FillSub ();
          return true;
		  break;
		case 70004:
          FillObject ();
          return true;
		  break;
      }
      break;
  }

  return false;
}

//SKIN_DECLARE_DEFAULT (DefaultSkin);

/*
int main (int argc, char* argv[])
{
  SysSystemDriver System;

  if (!System.Initialize (argc, argv, "/config/cswstest.cfg"))
    return -1;

  if (!System.Open ("CHIME Client"))
    return -1;

  DefaultSkin.Prefix = System.GetOptionCL ("skin");

  if (!DefaultSkin.Prefix)
    DefaultSkin.Prefix = System.GetConfig ()->GetStr ("CSWS.Skin.Variant", NULL);

  VEM app (&System, DefaultSkin);

  if (app.Initialize ("/lib/csws/csws.cfg"))
    System.Loop ();

  return 0;
}
*/
